#pragma once

class Tuner
{
  public:
    static float detectPitchAutocorrelation(const float *input, int bufferSize, double sampleRate)
    {
        float maxValue = 0.0f;
        int bestLag = -1;

        // Step 1: Remove DC offset
        float mean = 0.0f;
        for (int i = 0; i < bufferSize; ++i)
            mean += input[i];
        mean /= bufferSize;

        std::vector<float> centered(static_cast<unsigned long>(bufferSize));
        for (unsigned long i = 0; i < static_cast<unsigned long>(bufferSize); ++i)
            centered[i] = input[i] - mean;

        // Step 2: Autocorrelation
        std::vector<float> correlation(static_cast<unsigned long>(bufferSize) / 2); // Store for interpolation
        int minLag = static_cast<int>(sampleRate / 500.0f);                         // Max 500 Hz
        int maxLag = static_cast<int>(sampleRate / 40.0f);                          // Min 40 Hz
        maxLag = std::min(maxLag, bufferSize - 1);

        for (int lag = minLag; lag < maxLag; ++lag)
        {
            float sum = 0.0f;
            for (unsigned long i = 0; i < static_cast<unsigned long>(bufferSize - lag); ++i)
                sum += centered[i] * centered[i + static_cast<unsigned long>(lag)];

            correlation[static_cast<unsigned long>(lag)] = sum;

            if (sum > maxValue)
            {
                maxValue = sum;
                bestLag = lag;
            }
        }

        if (bestLag <= minLag || bestLag >= maxLag - 1)
            return 0.0f; // Can't interpolate at edges

        // After calculating maxValue and bestLag

        // Step 2.5: Check if the maximum correlation is strong enough
        float energy = 0.0f;
        for (unsigned long i = 0; i < static_cast<unsigned long>(bufferSize); ++i)
            energy += centered[i] * centered[i];

        // Simple threshold: if the maximum correlation is too small compared to energy
        if (maxValue < 0.8f * energy) // <-- You can tweak 0.1f (10%) to taste
            return 0.0f;

        // Step 3: Parabolic Interpolation
        float y1 = correlation[static_cast<unsigned long>(bestLag)];
        float y2 = correlation[static_cast<unsigned long>(bestLag) + 1];
        float y0 = correlation[static_cast<unsigned long>(bestLag) - 1];

        // Formula: https://ccrma.stanford.edu/~jos/sasp/Quadratic_Interpolation_Spectral_Peaks.html
        float shift = 0.5f * (y0 - y2) / (y0 - 2.0f * y1 + y2);
        float interpolatedLag = static_cast<float>(bestLag) + shift;

        // Step 4: Convert to frequency
        if (interpolatedLag > 0.0f)
            return static_cast<float>(sampleRate) / interpolatedLag;

        return 0.0f;
    }
};
