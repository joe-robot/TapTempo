/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Metronome.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...

    void tapTempo();

    void calculateAverage();

    void updateTimeArray();

    OwnedArray<Button> tapThis;

    bool tapped = false;
    int currSampleRate = 48000;
    int timeAVGArray[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int timeAVGArrayPos = 0;
    int timeAVGNum = 0; //Is the number of values to include in the average
    bool averageActive = false;
    int time = 0;
    float bpm;
    float samples = 48000;

    std::atomic<bool> start = {false};
    bool prevStart = false;

    Metronome myMetro;

    Time clock;
    int64 prevTime = 0;

    Random noise;

    float buttonPlaceArray[12] = {0.025f, 0.025f, 0.95f, 0.7f,
                                  0.025f, 0.75f, 0.45f, 0.225f,
                                  0.525f, 0.75f, 0.45f, 0.225f};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
