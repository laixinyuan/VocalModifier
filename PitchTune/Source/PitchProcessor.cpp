//
//  PitchProcessor.cpp
//  PitchTune
//
//  Created by Shaoduo Xie on 11/19/13.
//
//

#include "PitchProcessor.h"
#include "Sample.h"
#include "Psola.h"
#include "Utility.h"

PitchProcessor::PitchProcessor()
{
    psola = NULL;
}

PitchProcessor::~PitchProcessor()
{
    if (psola) {
        delete psola;
        psola = NULL;
    }
}

void PitchProcessor::process(Sample *sample)
{
    psola = new Psola(sample->getAudioBuffer(), sample->getNumSamples(), Utility::SAMPLE_RATE);
    psola->analyse();
    //psola->getPitches();
    //psola->freq2MidiCents();
}