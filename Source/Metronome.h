/*
  ==============================================================================

    Metronome.h
    Created: 12 May 2020 2:30:24pm
    Author:  Joseph Cresswell

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//Simple Class to generate metronome at set tempo
class Metronome
{
public:
    Metronome();
    ~Metronome();

    void setSampleRate(float newSampleRate);
    void setTempo(float newBPM);

    void setNextBlock(const AudioSourceChannelInfo& bufferToFill, int startSample, int endSample);
    void getNextBlock(const AudioSourceChannelInfo& bufferToFill);
private:

    float sampleInterval;
    float sampleRate = 48000;
    int soundDuration = 2400;
    bool soundPlaying = false;
    int soundProgress = 0;
    float samplePos[5] = {0, 0, 0, 0, 0}; //Allow maximum of 5 sounds per buffer (way larger than expected)
    int numSoundsInBuff = 0;

    float samplesToNextSound = 0;

    Random noise;
};