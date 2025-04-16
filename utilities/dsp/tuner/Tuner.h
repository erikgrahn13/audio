// #pragma once

// #include <JuceHeader.h>
// #include <limits>

// #define CUTOFF 0.93
// #define SMALL_CUTOFF 0.5
// #define LOWER_PITCH_CUTOFF 80 // Hz

// class PitchMPM
// {
//   public:
//     PitchMPM(size_t bufferSize) : PitchMPM(44100, bufferSize)
//     {
//     }

//     PitchMPM(int sampleRate, size_t bufferSize)
//         : mSampleRate(sampleRate), mBufferSize(bufferSize),
//           fftOrder(static_cast<int>(std::ceil(std::log2(2 * bufferSize)))), fftSize(1 << fftOrder), fft(fftOrder),
//           fftBuffer(2 * fftSize, 0.0f), output(fftSize, 0.0f)
//     {
//     }

//     float getPitch(const float *audioBuffer)
//     {
//         maxPositions.clearQuick();
//         periodEstimates.clearQuick();
//         ampEstimates.clearQuick();

//         if (audioBuffer == nullptr)
//         {
//             DBG("audioBuffer NULL");
//             return 0.0f;
//         }

//         auto nsdf = nsdfFrequencyDomain(audioBuffer);
//         auto max_positions = peak_picking(nsdf);
//         std::vector<std::pair<float, float>> estimates;

//         float highestAmplitude = -std::numeric_limits<float>::max();

//         for (auto tau : max_positions)
//         {
//             highestAmplitude = jmax(highestAmplitude, nsdf[static_cast<unsigned int>(tau)]);

//             if (nsdf[static_cast<unsigned int>(tau)] > SMALL_CUTOFF)
//             {
//                 auto x = parabolic_interpolation(nsdf, static_cast<float>(tau));
//                 estimates.push_back(x);
//                 highestAmplitude = std::max(highestAmplitude, std::get<1>(x));
//             }
//         }

//         if (estimates.empty())
//             return -1;

//         float actualCutoff = static_cast<float>(CUTOFF) * highestAmplitude;
//         float period = 0;

//         for (auto &estimate : estimates)
//         {
//             if (std::get<1>(estimate) >= actualCutoff)
//             {
//                 period = std::get<0>(estimate);
//                 break;
//             }
//         }

//         float pitchEstimate = (mSampleRate / period);
//         return (pitchEstimate > LOWER_PITCH_CUTOFF) ? pitchEstimate : -1;
//     }

//     void setSampleRate(int newSampleRate)
//     {
//         mSampleRate = newSampleRate;
//     }

//     void setBufferSize(unsigned int newBufferSize)
//     {
//         mBufferSize = newBufferSize;
//         fftOrder = static_cast<int>(std::ceil(std::log2(2 * mBufferSize)));
//         fftSize = 1 << fftOrder;
//         fft = juce::dsp::FFT(fftOrder);
//         fftBuffer.resize(2 * fftSize, 0.0f);
//         output.resize(fftSize, 0.0f);
//     }

//   private:
//     int mSampleRate;
//     size_t mBufferSize;
//     int fftOrder;
//     unsigned int fftSize;

//     juce::dsp::FFT fft;
//     std::vector<float> fftBuffer;
//     std::vector<float> output;

//     Array<int> maxPositions;
//     Array<float> periodEstimates;
//     Array<float> ampEstimates;

//     std::vector<float> nsdfFrequencyDomain(const float *audioBuffer)
//     {
//         auto acf = autoCorrelation(audioBuffer);
//         return acf;
//     }

//     std::vector<float> autoCorrelation(const float *audioBuffer)
//     {
//         std::fill(fftBuffer.begin(), fftBuffer.end(), 0.0f);

//         for (size_t i = 0; i < mBufferSize; ++i)
//             fftBuffer[2 * i] = audioBuffer[i]; // real part
//         // Imaginary parts already zero

//         // fft.perform(fftBuffer.data(), fftBuffer.data(), false);
//         fft.performRealOnlyForwardTransform(fftBuffer.data());

//         for (unsigned int i = 0; i < fftSize; ++i)
//         {
//             float real = fftBuffer[2 * i];
//             float imag = fftBuffer[2 * i + 1];
//             float magSquared = real * real + imag * imag;
//             fftBuffer[2 * i] = magSquared;
//             fftBuffer[2 * i + 1] = 0.0f;
//         }

//         // fft.perform(fftBuffer.data(), fftBuffer.data(), true);
//         fft.performRealOnlyInverseTransform(fftBuffer.data());

//         for (unsigned int i = 0; i < fftSize; ++i)
//             output[i] = fftBuffer[2 * i]; // only real part is needed

//         return output;
//     }

//     static std::pair<float, float> parabolic_interpolation(const std::vector<float> &array, float x)
//     {
//         unsigned int xi = static_cast<unsigned int>(x);
//         if (xi <= 0 || xi >= static_cast<unsigned int>(array.size()) - 1)
//             return {x, array[xi]};

//         float den = array[xi + 1] + array[xi - 1] - 2 * array[xi];
//         float delta = array[xi - 1] - array[xi + 1];

//         constexpr float epsilon = 1e-6f;
//         // if (den == 0.0f)
//         if (std::abs(den) < epsilon)
//             return {x, array[xi]};

//         float px = xi + delta / (2 * den);
//         float py = array[xi] - (delta * delta) / (8 * den);
//         return {px, py};
//     }

//     static std::vector<unsigned int> peak_picking(const std::vector<float> &nsdf)
//     {
//         std::vector<unsigned int> max_positions;
//         unsigned int size = static_cast<unsigned int>(nsdf.size());
//         unsigned int pos = 0;
//         unsigned int curMaxPos = 0;

//         while (pos < (size - 1) / 3 && nsdf[pos] > 0)
//             ++pos;
//         while (pos < size - 1 && nsdf[pos] <= 0.0f)
//             ++pos;

//         if (pos == 0)
//             pos = 1;

//         while (pos < size - 1)
//         {
//             if (nsdf[pos] > nsdf[pos - 1] && nsdf[pos] >= nsdf[pos + 1])
//             {
//                 if (curMaxPos == 0 || nsdf[pos] > nsdf[curMaxPos])
//                     curMaxPos = pos;
//             }

//             ++pos;

//             if (pos < size - 1 && nsdf[pos] <= 0.0f)
//             {
//                 if (curMaxPos > 0)
//                 {
//                     max_positions.push_back(curMaxPos);
//                     curMaxPos = 0;
//                 }

//                 while (pos < size - 1 && nsdf[pos] <= 0.0f)
//                     ++pos;
//             }
//         }

//         if (curMaxPos > 0)
//             max_positions.push_back(curMaxPos);

//         return max_positions;
//     }
// };

class Tuner
{
  public:
    float detectPitchAutocorrelation(const float *input, int bufferSize, float sampleRate)
    {
        float maxValue = 0.0f;
        int bestLag = -1;

        // Step 1: Remove DC offset
        float mean = 0.0f;
        for (int i = 0; i < bufferSize; ++i)
            mean += input[i];
        mean /= bufferSize;

        std::vector<float> centered(bufferSize);
        for (int i = 0; i < bufferSize; ++i)
            centered[i] = input[i] - mean;

        // Step 2: Autocorrelation
        std::vector<float> correlation(bufferSize / 2);     // Store for interpolation
        int minLag = static_cast<int>(sampleRate / 500.0f); // Max 500 Hz
        int maxLag = static_cast<int>(sampleRate / 40.0f);  // Min 40 Hz
        maxLag = std::min(maxLag, bufferSize - 1);

        for (int lag = minLag; lag < maxLag; ++lag)
        {
            float sum = 0.0f;
            for (int i = 0; i < bufferSize - lag; ++i)
                sum += centered[i] * centered[i + lag];

            correlation[lag] = sum;

            if (sum > maxValue)
            {
                maxValue = sum;
                bestLag = lag;
            }
        }

        if (bestLag <= minLag || bestLag >= maxLag - 1)
            return 0.0f; // Can't interpolate at edges

        // Step 3: Parabolic Interpolation
        float y1 = correlation[bestLag];
        float y0 = correlation[bestLag - 1];
        float y2 = correlation[bestLag + 1];

        // Formula: https://ccrma.stanford.edu/~jos/sasp/Quadratic_Interpolation_Spectral_Peaks.html
        float shift = 0.5f * (y0 - y2) / (y0 - 2.0f * y1 + y2);
        float interpolatedLag = static_cast<float>(bestLag) + shift;

        // Step 4: Convert to frequency
        if (interpolatedLag > 0.0f)
            return sampleRate / interpolatedLag;

        return 0.0f;
    }

    // float detectPitchAutocorrelation(const float *input, int bufferSize, float sampleRate)
    // {
    //     int maxLag = bufferSize / 2;
    //     float maxValue = 0.0f;
    //     int bestLag = -1;

    //     // Step 1: Normalize input to remove DC offset
    //     float mean = 0.0f;
    //     for (int i = 0; i < bufferSize; ++i)
    //         mean += input[i];
    //     mean /= bufferSize;

    //     // Center signal
    //     std::vector<float> centered(bufferSize);
    //     for (int i = 0; i < bufferSize; ++i)
    //         centered[i] = input[i] - mean;

    //     // Step 2: Autocorrelation
    //     for (int lag = 20; lag < maxLag; ++lag) // start at 20 to avoid low-frequency junk
    //     {
    //         float sum = 0.0f;

    //         for (int i = 0; i < bufferSize - lag; ++i)
    //             sum += centered[i] * centered[i + lag];

    //         if (sum > maxValue)
    //         {
    //             maxValue = sum;
    //             bestLag = lag;
    //         }
    //     }

    //     // Step 3: Convert lag to frequency
    //     if (bestLag > 0)
    //         return sampleRate / bestLag;

    //     return 0.0f;
    // }

    float getPitch(const float *audioBuffer)
    {
        // for (int l = 0; l < numSamples; ++l)
        // {

        //     if (count < recordSize)
        //     {

        //         // Record 2000 samples (only use left / first channel)
        //         recordedSamples[count] = audioBuffer[l];
        //         count++;
        //     }
        //     else
        //     {

        // After samples are recorded
        sum = 0;
        pdState = 0;
        int period = 0;

        for (i = 0; i < recordSize; i++)
        {

            // Autocorrelation
            sumOld = sum;
            sum = 0;

            for (k = 0; k < recordSize - i; k++)
            {
                sum += audioBuffer[k] * audioBuffer[k + i];
            }

            // Peak Detect State Machine
            if (pdState == 2 && (sum - sumOld) <= 0)
            {
                period = i;
                pdState = 3;
            }

            if (pdState == 1 && (sum > thresh) && (sum - sumOld) > 0)
            {
                pdState = 2;
            }

            if (!i)
            {
                thresh = sum * 0.5;
                pdState = 1;
            }
        }

        // Frequency identified in Hz
        if (thresh > 10)
        {
            frequency = sampleRate / period;
        }
        count = 0;
        // }
        return frequency;
    }

  private:
    float frequency = 0;
    float sampleRate;
    int recordSize = 1024;
    // float recordedSamples[1024] = {};
    int count = 0;
    float sum;
    float sumOld;
    int i, k;
    int pdState = 0;
    int thresh = 0;
};
