#include "BlackLoungeProcessor.h"
#include "BlackLoungeEditor.h"
#include "architecture.h"

//==============================================================================
BlackLoungeAudioProcessor::BlackLoungeAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
{
    // mAmp = std::make_unique<BlackLoungeAmp>();
    // mAmp = new BlackLoungeAmp();
    // mAmp = std::make_shared<BlackLoungeAmp>();
    // nam::activations::Activation::enable_fast_tanh();

    // auto test = "C:/Users/erikg/Development/audio/Black_Lounge/Fireball "
    //             "Ironmaster_v1_Fireball-IronMaster-v3_0_0-reamp.cm-St.nam";
    // auto path = std::filesystem::path(test);
    // // mNamModel = nam::get_dsp(path)

    // mModel = nam::get_dsp(path);
    // mModel->Reset(48000, 2048);

    mBlackLoungeAmp = std::make_unique<Amp>(BlackLoungeAmp::ironmaster_nam, BlackLoungeAmp::ironmaster_namSize);
}

BlackLoungeAudioProcessor::~BlackLoungeAudioProcessor()
{
}

//==============================================================================
const juce::String BlackLoungeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BlackLoungeAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool BlackLoungeAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool BlackLoungeAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double BlackLoungeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BlackLoungeAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int BlackLoungeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BlackLoungeAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String BlackLoungeAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void BlackLoungeAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void BlackLoungeAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    outputBuffer.setSize(1, samplesPerBlock * 4);
    outputBuffer.clear();

    // if (mAmp && mAmp->getNamModel())
    // {
    //     // mAmp->getNamModel()->ResetAndPrewarm(sampleRate, samplesPerBlock);
    // }

    // if (mModel)
    // {
    //     // mModel->ResetAndPrewarm(48000.0, 480);
    // }

    outputBuffer.setSize(1, samplesPerBlock, false, false, false);
    outputBuffer.clear();

    // if (mModel)
    // {
    //     // mModel->Reset(sampleRate, samplesPerBlock);
    // }
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void BlackLoungeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool BlackLoungeAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
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

void BlackLoungeAudioProcessor::doDualMono(juce::AudioBuffer<float> &mainBuffer, float **input)
{
    auto channelDataLeft = mainBuffer.getWritePointer(0);
    auto channelDataRight = mainBuffer.getWritePointer(1);

    for (int sample = 0; sample < mainBuffer.getNumSamples(); ++sample)
    {
        channelDataRight[sample] = input[0][sample];
        channelDataLeft[sample] = input[0][sample];
    }
}

void BlackLoungeAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    juce::ignoreUnused(midiMessages);
    std::ignore = buffer;

    if (mBlackLoungeAmp)
    {
        mBlackLoungeAmp->process(buffer.getWritePointer(0), buffer.getWritePointer(0), buffer.getNumSamples());
        if (buffer.getNumChannels() > 1)
            buffer.copyFrom(1, 0, buffer.getReadPointer(0), buffer.getNumSamples());
    }

    // auto *channelDataLeft = buffer.getWritePointer(0);
    // auto *channelDataRight = buffer.getWritePointer(1);
    // auto *outputData = outputBuffer.getWritePointer(0);

    // float **inputPointer = &channelDataLeft;
    // float **outputPointer = &outputData;
    // float **processedOutput;
    // float **triggerOutput = inputPointer;

    // if (mModel)
    // {
    //     mModel->process(*inputPointer, *outputPointer, buffer.getNumSamples());
    //     processedOutput = outputPointer;
    // }
    // else
    // {
    //     processedOutput = inputPointer;
    // }

    // doDualMono(buffer, processedOutput);

    // if (mModel)
    // {
    //     // Get pointers to the left channel input and the output buffer
    //     auto *inputData = buffer.getReadPointer(0);              // Left channel input
    //     auto *modelOutputData = outputBuffer.getWritePointer(0); // Model's output buffer

    //     // Process audio through the NAM model
    //     mModel->process(const_cast<float *>(inputData), modelOutputData, buffer.getNumSamples());

    //     // Copy processed audio back to main buffer (left and right channels)
    //     buffer.copyFrom(0, 0, modelOutputData, buffer.getNumSamples());
    //     if (buffer.getNumChannels() > 1)
    //         buffer.copyFrom(1, 0, modelOutputData, buffer.getNumSamples());
    // }
    // else
    // {
    //     buffer.clear();
    // }

    // juce::ignoreUnused(midiMessages);

    // auto totalNumInputChannels = getTotalNumInputChannels();
    // auto totalNumOutputChannels = getTotalNumOutputChannels();
    // for (int channel = totalNumInputChannels; channel < totalNumOutputChannels; ++channel)
    //     buffer.clear(channel, 0, buffer.getNumSamples());

    // if (mModel)
    // {
    //     mModel->process(buffer.getWritePointer(0), outputBuffer.getWritePointer(0), buffer.getNumSamples());
    //     mModel->finalize_(buffer.getNumSamples());
    //     //     // auto *processedData = outputBuffer.getReadPointer(0);

    //     //     // Copy to the left channel
    //     buffer.copyFrom(0, 0, outputBuffer.getReadPointer(0), buffer.getNumSamples());
    //     // buffer.applyGain(0.1f);
    // }

    // juce::ignoreUnused(midiMessages);

    // auto totalNumInputChannels = getTotalNumInputChannels();
    // auto totalNumOutputChannels = getTotalNumOutputChannels();

    // for (int channel = totalNumInputChannels; channel < totalNumOutputChannels; ++channel)
    //     buffer.clear(channel, 0, buffer.getNumSamples());

    // // Pass input directly to output
    // if (mAmp && mAmp->getNamModel())
    // {
    //     auto size1 = outputBuffer.getNumSamples();
    //     auto size2 = buffer.getNumSamples();
    //     std::ignore = size1;
    //     std::ignore = size2;

    //     mAmp->getNamModel()->process(buffer.getWritePointer(0), outputBuffer.getWritePointer(0),
    //                                  buffer.getNumSamples());

    //     // auto *processedData = outputBuffer.getReadPointer(0);

    //     // Copy to the left channel
    //     // buffer.copyFrom(0, 0, processedData, buffer.getNumSamples());
    //     // std::memcpy(buffer.getWritePointer(0), outputBuffer.getWritePointer(0), buffer.getNumSamples() *
    //     // sizeof(float));
    // }

    // if (mAmp)
    // {
    //     if (mAmp->getNamModel())
    //     {
    //         mAmp->getNamModel()->process(*inputPointer, *outputPointer, buffer.getNumSamples());
    //         for (int i = 0; i < buffer.getNumSamples(); ++i)
    //         {
    //             outputChannelDataLeft[i] = outputPointer[0][i];
    //             outputChannelDataRight[i] = outputPointer[0][i];
    //         }
    //     }
    // }

    // mAmp->getNamModel()->process(inputChannelData, outputData, buffer.getNumSamples());

    // mAmp->getNamModel()->finalize_(buffer.getNumSamples());

    // std::memcpy(inputChannelData, outputData, buffer.getNumSamples() * sizeof(float));

    // for (int i = 0; i < buffer.getNumChannels(); ++i)
    // {
    //     outputChannelDataLeft[i] = inputChannelData[i];
    // }
    // std::memcpy(outputChannelDataRight, inputChannelData, buffer.getNumSamples() * sizeof(float));

    // for (int channel = 0; channel < totalNumInputChannels; ++channel)
    // {
    //     auto *inputChannelData = buffer.getReadPointer(channel);
    //     auto *outputChannelData = buffer.getWritePointer(channel);

    //     // Check if the data is being copied
    //     std::memcpy(outputChannelData, inputChannelData, buffer.getNumSamples() * sizeof(float));
    // }
}

//==============================================================================
bool BlackLoungeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *BlackLoungeAudioProcessor::createEditor()
{
    auto *topLevel = juce::TopLevelWindow::getTopLevelWindow(0);
    if (topLevel)
    {
        topLevel->setUsingNativeTitleBar(true);
    }

    return new BlackLoungeAudioProcessorEditor(*this);
}

//==============================================================================
void BlackLoungeAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void BlackLoungeAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new BlackLoungeAudioProcessor();
}
