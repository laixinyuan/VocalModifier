//
//  AudioRoll.cpp
//  PitchTune
//
//  Created by Shaoduo Xie on 11/13/13.
//
//

#include "AudioRoll.h"
#include "PluginEditor.h"

//1102 samples per drawn sample
#define SAMPLES_PER_DRAWN_SAMPLE    256
#define HALF_HEIGHT 15.0

AudioRoll::AudioRoll()
{
    samples = NULL;
    numSamples = 0;
    normalizeMax = 1;
    deltaCents = 0;
    index = -1;
}

AudioRoll::~AudioRoll()
{
    if (samples) {
        delete [] samples;
        samples = NULL;
    }
}

void AudioRoll::paint(Graphics& g)
{
    Colour cc(0xFFA488FF);
    g.fillAll(cc.withAlpha(0.5f));
    g.setColour(Colour(0xFFCC8444));
    int x = 0;
    int y = 0;
    float max = 0.0;
    for (int i = 0; i < numSamples; ++i) {
        if (fabsf(samples[i]) >= max)
            max = fabsf(samples[i]);
    }
    for (int i = 0; i < numSamples; ++i) {
        Rectangle<int> rect;
        float scaled = samples[i] / max;
        rect = Rectangle<int>(x + i, y + (1 - fabsf(scaled)) * HALF_HEIGHT, 1.0f, fabsf(scaled)  * HALF_HEIGHT * 2);
//        if (scaled >= 0) {
//            rect = Rectangle<int>(x + i, y + (1 - scaled) * HALF_HEIGHT, 1.0f, scaled * HALF_HEIGHT * 2);
//        }
//        else {
//           rect = Rectangle<int>(x + i, y + HALF_HEIGHT - 1, 1.0f, fabsf(scaled) * HALF_HEIGHT);
//        }
        g.fillRect(rect);
    }
}

void AudioRoll::setSamples(float *x, long numTotalSamples, int midi_, int cent_, int startSample_, int endSample_, float max_, int index_)
{
    midi = midi_;
    cent = cent_;
    startSample = startSample_;
    endSample = endSample_;
    normalizeMax = max_;
    
    if (samples) {
        delete [] samples;
        samples = NULL;
    }
    numSamples = (int)(numTotalSamples / (float)SAMPLES_PER_DRAWN_SAMPLE);
    samples = new float[numSamples];
    for (int i = 0; i < numSamples; ++i) {
        float sum = 0.0;
        for (int j = 0; j < SAMPLES_PER_DRAWN_SAMPLE; ++j) {
//            sum += x[i * SAMPLES_PER_DRAWN_SAMPLE + j];
            sum += x[startSample + i * SAMPLES_PER_DRAWN_SAMPLE + j];
        }
        float avg = sum / SAMPLES_PER_DRAWN_SAMPLE;
        samples[i] = avg;
        //samples[i] = x[i * SAMPLES_PER_DRAWN_SAMPLE];
    }
    
    //smooth
    for (int i = 0; i < numSamples; ++i) {
        float sum = 0.0;
        int count = 0;
        for (int j = 0; j < 5; ++j) {
            if (i + j < numSamples) {
                sum += samples[i + j];
                ++count;
            }
        }
        float avg = sum / count;
        samples[i] = avg;
    }
    
    startDecimatedSample = (startSample / (float)SAMPLES_PER_DRAWN_SAMPLE);
    
#define MID_MIDI 55
#define NOTE_HEIGHT 30
    //50 means midi note 50
    int drift = cent / 100.0 * NOTE_HEIGHT;
    originalY = 400 + NOTE_HEIGHT * (MID_MIDI - midi) - drift;
    
    index = index_;
}

void AudioRoll::mouseEnter(const juce::MouseEvent &event)
{
    setAlpha(0.6f);
}

void AudioRoll::mouseExit(const juce::MouseEvent &event)
{
    setAlpha(1.0f);
}

void AudioRoll::mouseDrag(const juce::MouseEvent &event)
{
    int yInParent = getY() + event.y;
    int deltaYInParent = yInParent - (originalY + lastY);
    setBounds(getX(), originalY + deltaYInParent, getWidth(), getHeight());
}

void AudioRoll::mouseDown(const juce::MouseEvent &event)
{
    lastY = event.y;
}

void AudioRoll::mouseUp(const juce::MouseEvent &event)
{
    int yInParent = getY() + event.y;
    int deltaYInParent = yInParent - (originalY + lastY);
    setBounds(getX(), originalY + deltaYInParent, getWidth(), getHeight());
    deltaCents = (-1) * (int)((float)deltaYInParent * (100.0 / NOTE_HEIGHT));
    
    if (deltaYInParent != 0) {
        PitchTuneAudioProcessorEditor *e = (PitchTuneAudioProcessorEditor*)getParentComponent();
        e->updateAudio(index);
    }
}

void AudioRoll::resized()
{
    
}