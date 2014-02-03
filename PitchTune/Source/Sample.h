//
//  Sample.h
//  PitchTune
//
//  Created by Xinyuan Lai on 11/13/13.
//
//

#ifndef __PitchTune__Sample__
#define __PitchTune__Sample__

#include "PTIncludes.h"

class Psola;

class Sample
{
public:
    Sample();
    ~Sample();
    
    void prepareRecord();
    void record(float *x, int numSamples);
    void stopRecording();
    bool isLoaded() { return (audio != NULL); }
    long getNumSamples() { return nSamples; }
    void play(float *x, int numSamples, Psola *psola, long offset);
    void startPlay();
    void stopPlay();
    void startRecording() { isRecording = true; }
    long getCursor() { return cursor; }
    double startPpq;
    double stopPpq;
    bool isPlaying;
    bool isRecording;
    float *getAudioBuffer() { return audio; }
    void updateAudio(float *newAudio);
private:
    float *audio;
    long cursor;
    long nSamples;
};

#endif /* defined(__PitchTune__Sample__) */
