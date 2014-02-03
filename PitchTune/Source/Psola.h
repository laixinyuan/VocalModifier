//
//  Psola.h
//  PitchTune
//
//  Created by Xinyuan Lai on 12/2/13.
//
//

#ifndef __PitchTune__Psola__
#define __PitchTune__Psola__

#include <vector>


class Psola
{
public:
    Psola(float* auIn, long auLength, double sampleRate);
    Psola(float* auIn, long auLength, double sampleRate, int blockSize, int hopSize);
    ~Psola();
    void halfAutoCorr(int strtPtr);
    int getZeroCrsIndex();
    void medianFilter(int* array,int arrayLen, int filterLen);
    void medianFilter(float* array,int arrayLen, int filterLen);
    void getPitches();
    void freq2MidiCents();
    void getPitchMarks();
    void analyse();
    std::vector<int>& getPitchBorders();
    std::vector<int>& getMidiNums();
    std::vector<int>& getCentDiviations();
    void hannGen(int winLen);
    float* resynthesise(std::vector<int> pitchShifts);
    
    float* audioOut;
private:
    float* audioIn;
    long nSamples;
    long nBlocks;
    static double SAMPLE_RATE;
    static int BLOCK_SIZE;
    static int HOP_SIZE;
    static float FREQUENCY_UPPER_LIMIT;
    static int MEDIAN_FILTER_LENGTH;
    float* acf;
    float* f0;
    int* p0;
    int* midi;
    int* cents;
    float* blockRMS;
    std::vector<int> pitchMarks;
    std::vector<int> intrMarkIntvls;
    std::vector<int> pitchBorders;
    std::vector<int> midiNums;
    std::vector<int> centDeviations;
    float* hanning;
    
};

#endif /* defined(__PitchTune__Psola__) */
