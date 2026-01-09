#pragma once

#include "NAM/dsp.h"
#include <deque>

class Amp
{

  public:
    explicit Amp(const char *ampData, const int ampDataSize);
    Amp() = delete;
    ~Amp();

    void prepare();
    void process(float *input, float* output, int numSamples);

  private:
    const char *mAmpData;
    const int mAmpDataSize;
    std::unique_ptr<nam::DSP> mNamModel;
};