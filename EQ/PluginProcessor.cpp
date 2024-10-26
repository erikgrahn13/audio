#include "PluginProcessor.h"
#include "CustomAudioParameterFloat.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                         ),
      mParameters(*this, nullptr, juce::Identifier("Parameters"), createParameters())
{
    auto hpf =
        Filter::createFilterInstance<>(Filter::kHighPass, mParameters.getParameter("hpf_freq")->getDefaultValue());

    auto lpf =
        Filter::createFilterInstance<>(Filter::kLowPass, mParameters.getParameter("lpf_freq")->getDefaultValue());

    auto lowShelf =
        Filter::createFilterInstance<>(Filter::kLowShelf, mParameters.getParameter("LowShelfFreq")->getDefaultValue());

    auto highShelf = Filter::createFilterInstance<>(Filter::kHighShelf,
                                                    mParameters.getParameter("HighShelfFreq")->getDefaultValue());

    auto lowMid =
        Filter::createFilterInstance<>(Filter::kPeak, mParameters.getParameter("LowMidFreq")->getDefaultValue());

    auto highMid =
        Filter::createFilterInstance<>(Filter::kPeak, mParameters.getParameter("HighMidFreq")->getDefaultValue());

    auto tmp = std::optional<juce::RangedAudioParameter *>{std::nullopt};

    FilterTuple filter1(std::move(hpf), mParameters.getParameter("hpf_bypass"), mParameters.getParameter("hpf_freq"),
                        tmp, tmp, mParameters.getParameter("hpf_filterOrder"));
    FilterTuple filter2(std::move(lpf), mParameters.getParameter("lpf_bypass"), mParameters.getParameter("lpf_freq"),
                        tmp, tmp, mParameters.getParameter("lpf_filterOrder"));
    FilterTuple filter3(std::move(lowShelf), mParameters.getParameter("LowShelf_bypass"),
                        mParameters.getParameter("LowShelfFreq"), mParameters.getParameter("LowShelfGain"), tmp, tmp);
    FilterTuple filter4(std::move(highShelf), mParameters.getParameter("HighShelf_bypass"),
                        mParameters.getParameter("HighShelfFreq"), mParameters.getParameter("HighShelfGain"), tmp, tmp);
    FilterTuple filter5(std::move(lowMid), mParameters.getParameter("LowMid_bypass"),
                        mParameters.getParameter("LowMidFreq"), mParameters.getParameter("LowMidGain"),
                        mParameters.getParameter("LowMidQ"), tmp);
    FilterTuple filter6(std::move(highMid), mParameters.getParameter("HighMid_bypass"),
                        mParameters.getParameter("HighMidFreq"), mParameters.getParameter("HighMidGain"),
                        mParameters.getParameter("HighMidQ"), tmp);

    mFilters.push_back(filter1);
    mFilters.push_back(filter2);
    mFilters.push_back(filter3);
    mFilters.push_back(filter4);
    mFilters.push_back(filter5);
    mFilters.push_back(filter6);

    for (auto &filter : mFilters)
    {
        // bypass
        mParameters.addParameterListener(std::get<1>(filter)->getParameterID(), this);

        // freq
        mParameters.addParameterListener(std::get<2>(filter)->getParameterID(), this);

        if (std::get<3>(filter).has_value()) // check if gain parameter exists
        {
            mParameters.addParameterListener(std::get<3>(filter).value()->getParameterID(), this);
        }

        if (std::get<4>(filter).has_value()) // check if Q parameter exists
        {
            mParameters.addParameterListener(std::get<4>(filter).value()->getParameterID(), this);
        }
        if (std::get<5>(filter).has_value()) // check ff filter order parameter exists
        {
            mParameters.addParameterListener(std::get<5>(filter).value()->getParameterID(), this);
        }
    }

    mAudioBuffer.setSize(1, 4096 * 4);
    mRingBuffer.setTotalSize(4096 * 4);
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    for (auto &i : mFilters)
    {
        auto &[filter, bypass, freqParam, gainParam, QParam, filterOrder] = i;
        filter->prepare(sampleRate, samplesPerBlock, getMainBusNumInputChannels());
    }
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;

    if (requiresUpdate.load())
    {
        update();
    }

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        std::span<const float> input(buffer.getReadPointer(channel), static_cast<size_t>(buffer.getNumSamples()));
        std::span<float> output(buffer.getWritePointer(channel), static_cast<size_t>(buffer.getNumSamples()));

        for (auto &filter : mFilters)
        {
            if (dynamic_cast<juce::AudioParameterBool *>(std::get<1>(filter))->get())
            {
                std::get<0>(filter)->process(input, output, channel);
            }
        }
    }

    if (mRingBuffer.getFreeSpace() < buffer.getNumSamples())
        return;

    int start1, size1, start2, size2;
    mRingBuffer.prepareToWrite(buffer.getNumSamples(), start1, size1, start2, size2);
    mAudioBuffer.copyFrom(0, start1, buffer.getReadPointer(0), size1);

    if (size2 > 0)
        mAudioBuffer.copyFrom(0, start2, buffer.getReadPointer(0, size1), size2);

    if (size1 > 0)
        mAudioBuffer.addFrom(0, start1, buffer.getReadPointer(0), size1);
    if (size2 > 0)
        mAudioBuffer.addFrom(0, start2, buffer.getReadPointer(0, size1), size2);
    mRingBuffer.finishedWrite(size1 + size2);
    nextFFTBlockReady.store(true);
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor(*this, mParameters);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void AudioPluginAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    auto logRange = juce::NormalisableRange<float>(
        20.0, 20000.0,
        [](float min, float max, float value) {
            return std::exp(std::log(min) + value * (std::log(max) - std::log(min)));
        },
        [](float min, float max, float valuedB) {
            return (std::log(valuedB) - std::log(min)) / (std::log(max) - std::log(min));
        });

    // auto toString = [](float value) { return juce::String(value, 2); }; // 2 decimal places
    // auto fromString = [](const juce::String& text) { return text.getFloatValue(); };
    juce::AudioParameterFloatAttributes attributes;
    auto result = attributes.withStringFromValueFunction([](float value, int) { return juce::String(value, 2); });
    // attributes.withLabel("Hz");
    // attributes.
    parameters.push_back(std::make_unique<juce::AudioParameterBool>("hpf_bypass", "HPF_bypass", true));
    parameters.push_back(std::make_unique<juce::AudioParameterBool>("lpf_bypass", "LPF_bypass", true));
    parameters.push_back(std::make_unique<juce::AudioParameterBool>("LowShelf_bypass", "LowShelf_bypass", true));
    parameters.push_back(std::make_unique<juce::AudioParameterBool>("HighShelf_bypass", "HighShelf_bypass", true));
    parameters.push_back(std::make_unique<juce::AudioParameterBool>("LowMid_bypass", "LowMid_bypass", true));
    parameters.push_back(std::make_unique<juce::AudioParameterBool>("HighMid_bypass", "HighMid_bypass", true));

    parameters.push_back(std::make_unique<juce::AudioParameterChoice>(
        "hpf_filterOrder", "hpf_filterOrder", juce::StringArray{"6DB/OCT", "12DB/OCT", "18DB/OCT", "24DB/OCT"}, 1));
    parameters.push_back(std::make_unique<juce::AudioParameterChoice>(
        "lpf_filterOrder", "lpf_filterOrder", juce::StringArray{"6DB/OCT", "12DB/OCT", "18DB/OCT", "24DB/OCT"}, 1));

    // High Pass
    parameters.push_back(std::make_unique<CustomAudioParameterFloat>("hpf_freq", "HPF_Freq", logRange, 20.f, 1));

    // Low Pass
    parameters.push_back(std::make_unique<CustomAudioParameterFloat>("lpf_freq", "LPF_Freq", logRange, 20000.f, 6));

    // Low Shelving
    parameters.push_back(
        std::make_unique<CustomAudioParameterFloat>("LowShelfFreq", "LowShelfFreq", logRange, 100.f, 2));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        "LowShelfGain", "LowShelfGain", juce::NormalisableRange<float>(-18.f, 18.f, 0.1f), 0.f));

    // High Shelving
    parameters.push_back(
        std::make_unique<CustomAudioParameterFloat>("HighShelfFreq", "HighShelfFreq", logRange, 6000.f, 5));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        "HighShelfGain", "HighShelfGain", juce::NormalisableRange<float>(-18.f, 18.f, 0.1f), 0.f));

    // Low Mid Peak
    parameters.push_back(std::make_unique<CustomAudioParameterFloat>("LowMidFreq", "LowMidFreq", logRange, 600.f, 3));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        "LowMidGain", "LowMidGain", juce::NormalisableRange<float>(-18.f, 18.f, 0.1f), 0.f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        "LowMidQ", "LowMidQ", juce::NormalisableRange<float>(0.1f, 10.f, 0.1f), 1.f));

    // High Mid Peak
    parameters.push_back(
        std::make_unique<CustomAudioParameterFloat>("HighMidFreq", "HighMidFreq", logRange, 5000.f, 4));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        "HighMidGain", "HighMidGain", juce::NormalisableRange<float>(-18.f, 18.f, 0.1f), 0.f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        "HighMidQ", "HighMidQ", juce::NormalisableRange<float>(0.1f, 10.f, 0.1f), 1.f));

    return {parameters.begin(), parameters.end()};
}

void AudioPluginAudioProcessor::parameterChanged(const juce::String &parameter, float newValue)
{
    std::ignore = parameter;
    std::ignore = newValue;
    requiresUpdate.store(true);
}

void AudioPluginAudioProcessor::update()
{
    for (auto &i : mFilters)
    {
        auto &[filter, bypass, freqParam, gainParam, QParam, filterOrderParam] = i;

        filter->setFrequency(dynamic_cast<juce::AudioParameterFloat *>(freqParam)->get());

        if (gainParam)
        {
            filter->setGain(dynamic_cast<juce::AudioParameterFloat *>(*gainParam)->get());
        }

        if (QParam)
        {
            filter->setQ(dynamic_cast<juce::AudioParameterFloat *>(*QParam)->get());
        }
        if (filterOrderParam)
        {
            filter->setFilterOrder(
                static_cast<Filter::Order>(dynamic_cast<juce::AudioParameterChoice *>(*filterOrderParam)->getIndex()));
        }
    }

    requiresUpdate.store(false);
}
