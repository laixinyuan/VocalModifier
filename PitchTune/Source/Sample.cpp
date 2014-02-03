//
//  Sample.cpp
//  PitchTune
//
//  Created by Xinyuan Lai on 11/13/13.
//
//

#include "Sample.h"
#include "Utility.h"
#include "Constants.h"
#include "Psola.h"

Sample::Sample()
:audio(NULL)
{
    prepareRecord();
}

Sample::~Sample()
{
    if (audio) {
        delete [] audio;
        audio = NULL;
    }
}

void Sample::record(float *x, int numSamples)
{
    memcpy(audio + cursor, x, numSamples * sizeof(float));
//    for (int i = 0; i < numSamples; ++i) {
//        audio[cursor + i] = x[i];
//    }
    cursor += numSamples;
    nSamples += numSamples;
}

void Sample::prepareRecord()
{
    assert(audio == NULL);
    
    cursor = 0;
    nSamples = 0;
    audio = new float[int(Utility::SAMPLE_RATE * MAX_LENGTH_PER_SAMPLE)];
    startPpq = 0.0;
    isPlaying = false;
    isRecording = false;
}

void Sample::stopRecording()
{
    assert(audio != NULL);
    
    cursor = 0;
    isPlaying = false;
    isRecording = false;
//    for (int i = 0; i < nSamples / 2; ++i) {
//        audio[i] *= 0.3;
//    }
}

void Sample::play(float *x, int numSamples, Psola *psola, long offset)
{
    cursor = offset;
    if (isPlaying && cursor <= nSamples) {
        memcpy(x, psola->audioOut + cursor, numSamples * sizeof(float));
//        memcpy(x, audio + cursor, numSamples * sizeof(float));

        cursor += numSamples;
    }
}

void Sample::startPlay()
{
    isPlaying = true;
}

void Sample::stopPlay()
{
    isPlaying = false;
    cursor = 0;
}

void Sample::updateAudio(float *newAudio)
{
    memcpy(audio, newAudio, nSamples * sizeof(float));
}

