/*
  ==============================================================================

    Metronome.cpp
    Created: 12 May 2020 2:30:24pm
    Author:  Joseph Cresswell

  ==============================================================================
*/

#include "Metronome.h"

Metronome::Metronome()
{
    sampleInterval = 48000;
}

Metronome::~Metronome() {}

void Metronome::setSampleRate(float newSampleRate)
{
    sampleRate = newSampleRate > 0 ? newSampleRate : sampleRate; //Ensuring sample rate given > 0
    soundDuration = sampleRate * 0.05;
}

void Metronome::setTempo(float newBPM)
{
    newBPM  = newBPM > 0 ? newBPM : 120; //Check bpm in range

    sampleInterval = 60.0f / (newBPM * sampleRate);
}

void Metronome::getNextBlock(const AudioSourceChannelInfo &bufferToFill)
{
    int bufferSamples = bufferToFill.numSamples;
    while(samplesToNextSound - bufferSamples < 0) //If to play sound in this buffer then find start point up to 5 start points
    {
        samplePos[numSoundsInBuff] = samplesToNextSound;
        numSoundsInBuff++;
        samplesToNextSound = sampleInterval;
        bufferSamples -= samplesToNextSound;
    }

    if(soundPlaying)
    {
        setNextBlock(bufferToFill, 0, soundProgress);
    }

    for(int i = 0; numSoundsInBuff < 0; ++i)
    {
        float numberSamplesPossible = bufferToFill.numSamples - samplePos[i] - soundDuration;
        if(numberSamplesPossible > 0)
        {
            numberSamplesPossible = samplePos[i] + soundDuration;
        }
        else //If sound to continue after buffer
        {
            soundProgress = -1.0f * numberSamplesPossible;
            numberSamplesPossible = bufferToFill.numSamples;
            soundPlaying = true;
        }

        setNextBlock(bufferSamples, samplePos[i], numberSamplesPossible);
    }
}

void Metronome::setNextBlock(const AudioSourceChannelInfo &bufferToFill, int startSample, int endSample)
{
    for (int channels = 0; channels < bufferToFill.buffer->getNumChannels(); channels++)
    {

        float *const buff = bufferToFill.buffer->getWritePointer(channels, bufferToFill.startSample);

        for (int sample = startSample; sample < endSample; sample++)
        {
                buff[sample] = noise.nextFloat() - 0.5f; //Getting next float to play
        }
    }
}