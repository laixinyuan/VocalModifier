//
//  PitchProcessor.h
//  PitchTune
//
//  Created by Shaoduo Xie on 11/19/13.
//
//

#ifndef __PitchTune__PitchProcessor__
#define __PitchTune__PitchProcessor__


class Psola;
class Sample;

class PitchProcessor
{
public:
    PitchProcessor();
    ~PitchProcessor();
    
    void process(Sample *sample);
    
    Psola *psola;
};

#endif /* defined(__PitchTune__PitchProcessor__) */
