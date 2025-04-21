#include <JuceHeader.h>

class RingBuffer
{
  public:
    RingBuffer(int size) : mAbstractFifo(size)
    {
        mAudioBuffer.setSize(1, size);
    }

    void setSize(int size)
    {
        mAbstractFifo.setTotalSize(size);
        mAudioBuffer.setSize(1, size);
    }

    template <typename T> void addToFifo(const T *data, int numSamples)
    {
        const auto scope = mAbstractFifo.write(numSamples);

        if (scope.blockSize1 > 0)
            mAudioBuffer.copyFrom(0, scope.startIndex1, data, scope.blockSize1);
        if (scope.blockSize2 > 0)
            mAudioBuffer.copyFrom(0, scope.startIndex2, data + scope.blockSize1, scope.blockSize2);
    }

    template <typename T> void readFromFifo(T *destination, int numSamples)
    {
        const auto scope = mAbstractFifo.read(numSamples);

        const float *buffer = mAudioBuffer.getReadPointer(0);

        if (scope.blockSize1 > 0)
            std::copy(buffer + scope.startIndex1, buffer + scope.startIndex1 + scope.blockSize1, destination);

        if (scope.blockSize2 > 0)
            std::copy(buffer + scope.startIndex2, buffer + scope.startIndex2 + scope.blockSize2,
                      destination + scope.blockSize1);
    }

    int getNumReady()
    {
        return mAbstractFifo.getNumReady();
    }

    juce::AbstractFifo mAbstractFifo;

  private:
    juce::AudioBuffer<float> mAudioBuffer;
};