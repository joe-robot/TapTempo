/*
  ==============================================================================

    Metronome.cpp
    Created: 12 May 2020 2:30:24pm
    Author:  Joseph Cresswell

  ==============================================================================
*/

#include "Metronome.h"
#include <android/log.h>

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

    sampleInterval = (60.0f * sampleRate)/ (newBPM);
    intervalProgress = 0; //Resetting sample to next sound
    soundPlaying = false;
}

void Metronome::getNextBlock(const AudioSourceChannelInfo &bufferToFill)
{
   /* numSoundsInBuff = 0;
    int bufferSamples = bufferToFill.numSamples;
    bool addSamples = true;
    while(addSamples)
    {
        if (samplesToNextSound - bufferSamples < 0) //If to play sound in this buffer then find start point up to 5 start points
        {
            samplePos[numSoundsInBuff] =
                    numSoundsInBuff > 0 ? samplePos[numSoundsInBuff - 1] + samplesToNextSound : samplesToNextSound;
            numSoundsInBuff++;
            bufferSamples -= samplesToNextSound;
            samplesToNextSound = sampleInterval;
        }
        else
        {
            samplesToNextSound = samplesToNextSound - bufferSamples;
            addSamples = false;
        }
    }

    if(soundPlaying)
    {
        soundPlaying = false;
        setNextBlock(bufferToFill, 0, soundProgress);
    }

    for(int i = 0; i < numSoundsInBuff; ++i)
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

        setNextBlock(bufferToFill, samplePos[i], numberSamplesPossible);
    }*/


   numSoundsInBuff = 0;
   int bufferSize = bufferToFill.numSamples;

   if(soundPlaying)
   {
       samplePos[0] = 0;
       if(soundProgress < bufferSize)
       {
           sampleEndPos[0] = soundProgress;
           soundPlaying = false;
       }
       else
       {
           sampleEndPos[0] = bufferSize;
           soundProgress = bufferSize - soundProgress;
       }

       numSoundsInBuff++;
   }


   int maxNumInterval = ceil(bufferSize/sampleInterval);

   for(int i =0; i< bufferSize; ++i)
   {
       if(i + intervalProgress < bufferSize)
       {
           samplePos[numSoundsInBuff] = i + intervalProgress;
           intervalProgress = sampleInterval;
           int endPoint = i + intervalProgress + soundDuration;
           if(endPoint < bufferSize)
           {
               sampleEndPos[numSoundsInBuff] = endPoint;
           }
           else
           {
               sampleEndPos[numSoundsInBuff] = bufferSize;
               soundProgress = soundDuration - (endPoint - bufferSize);
               soundPlaying = true;
           }
           i = i + intervalProgress;
           numSoundsInBuff++;
       }
       else
       {
           intervalProgress = (i + intervalProgress) - bufferSize;
           break;
       }
   }

   setNextBlock(bufferToFill);

}

void Metronome::setNextBlock(const AudioSourceChannelInfo &bufferToFill)
{
   for (int channels = 0; channels < bufferToFill.buffer->getNumChannels(); channels++)
    {

        float *const buff = bufferToFill.buffer->getWritePointer(channels, bufferToFill.startSample);


        //for (int sample = 0; sample < bufferToFill.numSamples; sample++)
        for(int i = 0; i < numSoundsInBuff; ++i)
        {
            for(int j = samplePos[i]; j < sampleEndPos[i]; j++)
            {
                buff[j] = buff[j] + noise.nextFloat()-0.5f;
            }
        }
    }
}