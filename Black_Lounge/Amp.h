#pragma once

#include "NAM/dsp.h"
// #include "NoiseGate.h"
#include <deque>

class Amp
{

  public:
    explicit Amp(const char *ampData, const int ampDataSize);
    Amp() = delete;
    ~Amp();

    void prepare();
    void process(NAM_SAMPLE *input, NAM_SAMPLE *output, int numFrames);
    void setThreshold(float threshold);
    // std::shared_ptr<nam::DSP> getNamModel();
    // void process();
    // std::shared_ptr<Convolution> getIR();
    // std::shared_ptr<NoiseGate> getNoiseGate();

  private:
    // std::shared_ptr<nam::DSP> mNamModel;
    // std::shared_ptr<Convolution> mIR;
    // std::unique_ptr<NoiseGate> mNoiseGate;
    const char *mAmpData;
    const int mAmpDataSize;
    std::unique_ptr<nam::DSP> mNamModel;
};