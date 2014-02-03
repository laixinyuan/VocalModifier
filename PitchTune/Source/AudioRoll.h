//
//  AudioRoll.h
//  PitchTune
//
//  Created by Shaoduo Xie on 11/13/13.
//
//

#ifndef __PitchTune__AudioRoll__
#define __PitchTune__AudioRoll__

#include "../JuceLibraryCode/JuceHeader.h"
#include "PTIncludes.h"

class AudioRoll : public Component
{
public:
    AudioRoll();
    ~AudioRoll();
    
    void paint (Graphics& g);
    void resized();
    void setSamples(float *x, long numSamples, int midi, int cent, int startSample, int endSample, float max, int index);
    
    void mouseEnter (const MouseEvent &event);
    void mouseExit (const MouseEvent &event);
    void mouseDrag (const MouseEvent &event);
    void mouseDown (const MouseEvent &event);
    void mouseUp (const MouseEvent &event);
    
    float *samples;
    long numSamples;
    int midi;
    int cent;
    int startSample;
    int startDecimatedSample;
    int endSample;
    float normalizeMax;
    int lastY;
    int deltaCents;
    int originalY;
    int index;
};

#endif /* defined(__PitchTune__AudioRoll__) */
