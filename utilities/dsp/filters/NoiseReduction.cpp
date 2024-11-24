#include "NoiseReduction.h"

NoiseReduction::NoiseReduction()
{
    // constexpr auto f1{45};
    // constexpr auto f2{115};
    // constexpr auto f3{450};
    constexpr auto f1{573};
    constexpr auto f2{2500};

    mLowBandLowPass.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    mLowBandAllPass1.setType(juce::dsp::LinkwitzRileyFilterType::allpass);
    mLowBandAllPass2.setType(juce::dsp::LinkwitzRileyFilterType::allpass);
    mLowBandLowPass.setCutoffFrequency(f1);
    mLowBandAllPass1.setCutoffFrequency(f2);
    // mLowBandAllPass2.setCutoffFrequency(f3);

    mMid1BandLowPass.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    mMid1BandHighPass.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    mMid1BandAllPass.setType(juce::dsp::LinkwitzRileyFilterType::allpass);
    mMid1BandLowPass.setCutoffFrequency(f2);
    mMid1BandHighPass.setCutoffFrequency(f1);
    // mMid1BandAllPass.setCutoffFrequency(f3);

    mMid2BandLowPass.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    mMid2BandHighPass.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    mMid2BandAllPass.setType(juce::dsp::LinkwitzRileyFilterType::allpass);
    // mMid2BandLowPass.setCutoffFrequency(f3);
    mMid2BandHighPass.setCutoffFrequency(f2);
    mMid2BandAllPass.setCutoffFrequency(f1);

    mHighBandHighPass.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    mHighBandAllPass1.setType(juce::dsp::LinkwitzRileyFilterType::allpass);
    mHighBandAllPass2.setType(juce::dsp::LinkwitzRileyFilterType::allpass);
    mHighBandHighPass.setCutoffFrequency(f2);
    mHighBandAllPass1.setCutoffFrequency(f1);
    mHighBandAllPass2.setCutoffFrequency(f2);
}

NoiseReduction::~NoiseReduction()
{
}

void NoiseReduction::prepare(const juce::dsp::ProcessSpec &spec)
{
    mLowBandLowPass.prepare(spec);
    mLowBandAllPass1.prepare(spec);
    mLowBandAllPass2.prepare(spec);

    mMid1BandLowPass.prepare(spec);
    mMid1BandHighPass.prepare(spec);
    mMid1BandAllPass.prepare(spec);

    mMid2BandLowPass.prepare(spec);
    mMid2BandHighPass.prepare(spec);
    mMid2BandAllPass.prepare(spec);

    mHighBandHighPass.prepare(spec);
    mHighBandAllPass1.prepare(spec);
    mHighBandAllPass2.prepare(spec);

    for (auto &buffer : filterBuffers)
    {
        buffer.setSize(spec.numChannels, spec.maximumBlockSize);
    }

    mLowBandGate.prepare(spec);
    mMidBandGate.prepare(spec);
    mHighBandGate.prepare(spec);
}

void NoiseReduction::process(juce::AudioBuffer<float> &buffer)
{
    for (auto &buf : filterBuffers)
    {
        buf = buffer;
    }

    auto block1 = juce::dsp::AudioBlock<float>(filterBuffers[0]);
    auto block2 = juce::dsp::AudioBlock<float>(filterBuffers[1]);
    auto block3 = juce::dsp::AudioBlock<float>(filterBuffers[2]);
    auto block4 = juce::dsp::AudioBlock<float>(filterBuffers[3]);
    auto context1 = juce::dsp::ProcessContextReplacing<float>(block1);
    auto context2 = juce::dsp::ProcessContextReplacing<float>(block2);
    auto context3 = juce::dsp::ProcessContextReplacing<float>(block3);
    auto context4 = juce::dsp::ProcessContextReplacing<float>(block4);

    mLowBandLowPass.process(context1);
    mLowBandAllPass1.process(context1);
    mLowBandGate.process(context1);
    // mLowBandAllPass2.process(context1);

    mMid1BandLowPass.process(context2);
    mMid1BandHighPass.process(context2);
    mMidBandGate.process(context2);
    // mMid1BandAllPass.process(context2);

    // mMid2BandLowPass.process(context3);
    // mMid2BandHighPass.process(context3);
    // mMid2BandAllPass.process(context3);

    mHighBandHighPass.process(context4);
    mHighBandAllPass1.process(context4);
    mHighBandGate.process(context4);
    // mHighBandAllPass2.process(context4);

    buffer.clear(); // Clear the original buffer to avoid accumulation

    for (int i = 0; i < buffer.getNumChannels(); ++i)
    {
        buffer.addFrom(i, 0, filterBuffers[0], i, 0, buffer.getNumSamples()); // Add low-band
        buffer.addFrom(i, 0, filterBuffers[1], i, 0, buffer.getNumSamples()); // Add high-band
        // buffer.addFrom(i, 0, filterBuffers[2], i, 0, buffer.getNumSamples()); // Add high-band
        buffer.addFrom(i, 0, filterBuffers[3], i, 0, buffer.getNumSamples()); // Add high-band
        // buffer.applyGain(i, 0, buffer.getNumSamples(), -1.0f);
    }
}

void NoiseReduction::setThreshold(float threshold)
{
    mLowBandGate.setThreshold(threshold);
    mMidBandGate.setThreshold(threshold);
    mHighBandGate.setThreshold(threshold);
}
