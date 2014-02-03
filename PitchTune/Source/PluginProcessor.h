/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PTIncludes.h"
#include <vector>


class Sample;
class PitchProcessor;

//==============================================================================
/**
*/
class PitchTuneAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    PitchTuneAudioProcessor();
    ~PitchTuneAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

    //==============================================================================
    const String getName() const;

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;
    bool silenceInProducesSilenceOut() const;
    double getTailLengthSeconds() const;

    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

    //====================own========
    bool isTransferring() { return isRecording; }
    void startTransferring();
    void stopTransferring();
    PitchProcessor *getPitchProcessor() { return pitchProcessor; }
    
    void updateAudio(std::vector<int>& pitches);
private:
    void processPitch();
    inline void updateBpm(double newBpm, int denominator);
    void drawThumbs();
    
    bool isRecording;
    std::vector<Sample*> samples;
    Sample *sampleBeingRecorded;
    PitchProcessor *pitchProcessor;
    double bpm;
    double lastBlockPpq;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchTuneAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
