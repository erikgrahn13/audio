#include <JuceHeader.h>
#include <limits>

#define CUTOFF 0.93
#define SMALL_CUTOFF 0.5
#define LOWER_PITCH_CUTOFF 80 // Hz

class PitchMPM
{
  public:
    PitchMPM(size_t bufferSize) : PitchMPM(44100, bufferSize)
    {
    }

    PitchMPM(int sampleRate, size_t bufferSize)
        : mSampleRate(sampleRate), mBufferSize(bufferSize),
          fftOrder(static_cast<int>(std::ceil(std::log2(2 * bufferSize)))), fftSize(1 << fftOrder), fft(fftOrder),
          fftBuffer(2 * fftSize, 0.0f), output(fftSize, 0.0f)
    {
    }

    float getPitch(const float *audioBuffer)
    {
        maxPositions.clearQuick();
        periodEstimates.clearQuick();
        ampEstimates.clearQuick();

        if (audioBuffer == nullptr)
        {
            DBG("audioBuffer NULL");
            return 0.0f;
        }

        auto nsdf = nsdfFrequencyDomain(audioBuffer);
        auto max_positions = peak_picking(nsdf);
        std::vector<std::pair<float, float>> estimates;

        float highestAmplitude = -std::numeric_limits<float>::max();

        for (auto tau : max_positions)
        {
            highestAmplitude = jmax(highestAmplitude, nsdf[static_cast<unsigned int>(tau)]);

            if (nsdf[static_cast<unsigned int>(tau)] > SMALL_CUTOFF)
            {
                auto x = parabolic_interpolation(nsdf, tau);
                estimates.push_back(x);
                highestAmplitude = std::max(highestAmplitude, std::get<1>(x));
            }
        }

        if (estimates.empty())
            return -1;

        float actualCutoff = CUTOFF * highestAmplitude;
        float period = 0;

        for (auto &estimate : estimates)
        {
            if (std::get<1>(estimate) >= actualCutoff)
            {
                period = std::get<0>(estimate);
                break;
            }
        }

        float pitchEstimate = (mSampleRate / period);
        return (pitchEstimate > LOWER_PITCH_CUTOFF) ? pitchEstimate : -1;
    }

    void setSampleRate(int newSampleRate)
    {
        mSampleRate = newSampleRate;
    }

    void setBufferSize(unsigned int newBufferSize)
    {
        mBufferSize = newBufferSize;
        fftOrder = static_cast<int>(std::ceil(std::log2(2 * mBufferSize)));
        fftSize = 1 << fftOrder;
        fft = juce::dsp::FFT(fftOrder);
        fftBuffer.resize(2 * fftSize, 0.0f);
        output.resize(fftSize, 0.0f);
    }

  private:
    int mSampleRate;
    size_t mBufferSize;
    int fftOrder;
    unsigned int fftSize;

    juce::dsp::FFT fft;
    std::vector<float> fftBuffer;
    std::vector<float> output;

    Array<int> maxPositions;
    Array<float> periodEstimates;
    Array<float> ampEstimates;

    std::vector<float> nsdfFrequencyDomain(const float *audioBuffer)
    {
        auto acf = autoCorrelation(audioBuffer);
        return acf;
    }

    std::vector<float> autoCorrelation(const float *audioBuffer)
    {
        std::fill(fftBuffer.begin(), fftBuffer.end(), 0.0f);

        for (size_t i = 0; i < mBufferSize; ++i)
            fftBuffer[2 * i] = audioBuffer[i]; // real part
        // Imaginary parts already zero

        // fft.perform(fftBuffer.data(), fftBuffer.data(), false);
        fft.performRealOnlyForwardTransform(fftBuffer.data());

        for (unsigned int i = 0; i < fftSize; ++i)
        {
            float real = fftBuffer[2 * i];
            float imag = fftBuffer[2 * i + 1];
            float magSquared = real * real + imag * imag;
            fftBuffer[2 * i] = magSquared;
            fftBuffer[2 * i + 1] = 0.0f;
        }

        // fft.perform(fftBuffer.data(), fftBuffer.data(), true);
        fft.performRealOnlyInverseTransform(fftBuffer.data());

        for (unsigned int i = 0; i < fftSize; ++i)
            output[i] = fftBuffer[2 * i]; // only real part is needed

        return output;
    }

    static std::pair<float, float> parabolic_interpolation(const std::vector<float> &array, float x)
    {
        unsigned int xi = static_cast<unsigned int>(x);
        if (xi <= 0 || xi >= static_cast<unsigned int>(array.size()) - 1)
            return {x, array[xi]};

        float den = array[xi + 1] + array[xi - 1] - 2 * array[xi];
        float delta = array[xi - 1] - array[xi + 1];

        constexpr float epsilon = 1e-6f;
        // if (den == 0.0f)
        if (std::abs(den) < epsilon)
            return {x, array[xi]};

        float px = xi + delta / (2 * den);
        float py = array[xi] - (delta * delta) / (8 * den);
        return {px, py};
    }

    static std::vector<unsigned int> peak_picking(const std::vector<float> &nsdf)
    {
        std::vector<unsigned int> max_positions;
        unsigned int size = static_cast<unsigned int>(nsdf.size());
        unsigned int pos = 0;
        unsigned int curMaxPos = 0;

        while (pos < (size - 1) / 3 && nsdf[pos] > 0)
            ++pos;
        while (pos < size - 1 && nsdf[pos] <= 0.0f)
            ++pos;

        if (pos == 0)
            pos = 1;

        while (pos < size - 1)
        {
            if (nsdf[pos] > nsdf[pos - 1] && nsdf[pos] >= nsdf[pos + 1])
            {
                if (curMaxPos == 0 || nsdf[pos] > nsdf[curMaxPos])
                    curMaxPos = pos;
            }

            ++pos;

            if (pos < size - 1 && nsdf[pos] <= 0.0f)
            {
                if (curMaxPos > 0)
                {
                    max_positions.push_back(curMaxPos);
                    curMaxPos = 0;
                }

                while (pos < size - 1 && nsdf[pos] <= 0.0f)
                    ++pos;
            }
        }

        if (curMaxPos > 0)
            max_positions.push_back(curMaxPos);

        return max_positions;
    }
};
