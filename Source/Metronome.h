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

    void getNextBlock(const AudioSourceChannelInfo& bufferToFill);
private:

    void setNextBlock(const AudioSourceChannelInfo& bufferToFill);

    float sampleRate = 48000;
    float sampleInterval;

    int soundDuration = 2400;
    bool soundPlaying = false;

    int soundProgress = 0;
    int intervalProgress = 0;

    float samplePos[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //Allow maximum of 5 sounds per buffer (way larger than expected)
    float sampleEndPos[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //Allow maximum of 5 sounds per buffer (way larger than expected)

    int numSoundsInBuff = 0;

    Random noise;
};