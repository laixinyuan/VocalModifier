/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utility.h"
#include "Sample.h"
#include "PitchProcessor.h"
#include "Psola.h"


//==============================================================================
PitchTuneAudioProcessor::PitchTuneAudioProcessor()
{
    isRecording = false;
    sampleBeingRecorded = NULL;
    
    pitchProcessor = new PitchProcessor();
    bpm  = 0.0;
    lastBlockPpq = 0.0;
}

PitchTuneAudioProcessor::~PitchTuneAudioProcessor()
{
    for (int i = 0; i < samples.size(); ++i) {
        delete samples[i];
        samples[i] = NULL;
    }
    samples.clear();
    if (pitchProcessor)
        delete pitchProcessor;
}

//==============================================================================
const String PitchTuneAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int PitchTuneAudioProcessor::getNumParameters()
{
    return 0;
}

float PitchTuneAudioProcessor::getParameter (int index)
{
    return 0.0f;
}

void PitchTuneAudioProcessor::setParameter (int index, float newValue)
{
}

const String PitchTuneAudioProcessor::getParameterName (int index)
{
    return String::empty;
}

const String PitchTuneAudioProcessor::getParameterText (int index)
{
    return String::empty;
}

const String PitchTuneAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String PitchTuneAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool PitchTuneAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool PitchTuneAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool PitchTuneAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PitchTuneAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PitchTuneAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double PitchTuneAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PitchTuneAudioProcessor::getNumPrograms()
{
    return 0;
}

int PitchTuneAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PitchTuneAudioProcessor::setCurrentProgram (int index)
{
}

const String PitchTuneAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void PitchTuneAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PitchTuneAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    Utility::SAMPLE_RATE = (int)sampleRate;
}

void PitchTuneAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void PitchTuneAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    AudioPlayHead::CurrentPositionInfo posInfo;
    bool isHostGoing_ = false;
    if (getPlayHead() != 0 && getPlayHead()->getCurrentPosition(posInfo)) {
        isHostGoing_ = posInfo.isPlaying;//(posInfo.isPlaying || posInfo.isRecording);
        updateBpm(posInfo.bpm, posInfo.timeSigDenominator);
    }
    
    PitchTuneAudioProcessorEditor *e = (PitchTuneAudioProcessorEditor*)getActiveEditor();
    if (e) {
        if (e->isVisible()) {
            e->ppq = posInfo.ppqPosition;
            e->type = 1;
            e->triggerAsyncUpdate();
        }
    }

    if (isRecording) {
        if (isHostGoing_) {
            //record when host is playing
            if (sampleBeingRecorded && sampleBeingRecorded->getCursor() == 0) {
                // first time recording, store ppq
                sampleBeingRecorded->startPpq = posInfo.ppqPosition;
                //set recording flag
                sampleBeingRecorded->startRecording();
            }
            float* channelData = buffer.getSampleData (0);
            sampleBeingRecorded->record(channelData, buffer.getNumSamples());
        }
        else {
            if (sampleBeingRecorded && sampleBeingRecorded->isRecording) {
                //store stop ppq
                sampleBeingRecorded->stopPpq = posInfo.ppqPosition;
                //daw has stopped
                stopTransferring();
                //process pitch
                processPitch();
            }
        }
    }
    else {
        //playback the processed
        float* channelData = buffer.getSampleData (0);
        if (isHostGoing_) {
            int nClips = (int)samples.size();
            for (int i = 0; i < nClips; ++i) {
                Sample *curSample = samples[i];
                if (curSample ->startPpq >= posInfo.ppqPosition && !curSample ->isPlaying) {
                    //reach the start ppq
                    curSample ->startPlay();
                }
                
                if (posInfo.ppqPosition >= curSample ->stopPpq && curSample ->isPlaying) {
                    //reach the end ppq
                    curSample->stopPlay();
                }
                
                if (curSample ->isPlaying) {
                    curSample ->play(channelData, buffer.getNumSamples(), pitchProcessor->psola, (long)(posInfo.ppqPosition / Utility::numBeatsPerSample));
                }
            }
        }
        
    }

    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
    
    lastBlockPpq = posInfo.ppqPosition;
}

void PitchTuneAudioProcessor::processPitch()
{
    int index = (int)samples.size() - 1;
    if (index >= 0)
        pitchProcessor->process(samples[index]);
    
    drawThumbs();
}

void PitchTuneAudioProcessor::drawThumbs()
{
    PitchTuneAudioProcessorEditor *e = (PitchTuneAudioProcessorEditor*)getActiveEditor();
    
    Sample *lastSample = samples[(int)samples.size() - 1];
    if (e) {
        e->setThumbs(lastSample->getAudioBuffer(), lastSample->getNumSamples(), pitchProcessor);
        //e->type = 0;
        e->updateThumb = true;
        e->triggerAsyncUpdate();
    }
}

void PitchTuneAudioProcessor::startTransferring()
{
    assert(sampleBeingRecorded == NULL);
    
    sampleBeingRecorded = new Sample();
    isRecording = true;
}

void PitchTuneAudioProcessor::stopTransferring()
{
    sampleBeingRecorded->stopRecording();
    samples.push_back(sampleBeingRecorded);
    sampleBeingRecorded = NULL;
    isRecording = false;
}

void PitchTuneAudioProcessor::updateAudio(std::vector<int>& pitches)
{
    pitchProcessor->psola->resynthesise(pitches);
    //int index = (int)samples.size() - 1;
    //if (index >= 0) {
    samples[0]->updateAudio(pitchProcessor->psola->audioOut);
    //}
}

inline void PitchTuneAudioProcessor::updateBpm(double newBpm, int denominator)
{
    if (newBpm != bpm) {
        bpm = newBpm;
        Utility::numBeatsPerSample  = newBpm / 60.0 * 4.0 / denominator / Utility::SAMPLE_RATE;
    }
}


//==============================================================================
bool PitchTuneAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PitchTuneAudioProcessor::createEditor()
{
    return new PitchTuneAudioProcessorEditor (this);
}

//==============================================================================
void PitchTuneAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PitchTuneAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PitchTuneAudioProcessor();
}
