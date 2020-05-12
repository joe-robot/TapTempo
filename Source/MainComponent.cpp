/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include <android/log.h>
//==============================================================================
MainComponent::MainComponent()
{

    auto* button = tapThis.add(new TextButton("Tap Here"));
    button -> onStateChange = [this]() { tapTempo(); };
    addAndMakeVisible(button);

    button = tapThis.add(new TextButton("Start"));
    button -> onClick = [this]() { start = true; };
    addAndMakeVisible(button);

    button = tapThis.add(new TextButton("Stop"));
    button -> onClick = [this]() { start = false; };
    addAndMakeVisible(button);


    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    /*if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {*/
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    //}
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    currSampleRate = sampleRate;
    myMetro.setSampleRate(sampleRate);

}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    //Clearing buffer to stop random noise
    bufferToFill.clearActiveBufferRegion();
    //__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "loopy");



    if(tapped)
    {
        if(!start)
        {
            //PLay sound if tapped
            for (int channels = 0; channels < bufferToFill.buffer->getNumChannels(); channels++) {
                float *const buff = bufferToFill.buffer->getWritePointer(channels,
                                                                         bufferToFill.startSample);

                for (int sample = 0; sample < bufferToFill.numSamples; sample++) {
                    buff[sample] = ((noise.nextFloat()) - 0.5f);
                }
            }
        }
        //__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%i", currSampleRate);
        if(averageActive)
        {
            timeAVGArray[timeAVGArrayPos] = time; //Adding element to the array
            calculateAverage();
            timeAVGArrayPos++;
            timeAVGArrayPos = timeAVGArrayPos % 20;
            //__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%i", time);
            timeAVGNum = timeAVGNum < 19 ? timeAVGNum + 1: 19;

        }
        else
        {
            averageActive = true;
            samples = currSampleRate;
        }
        time = 0;
        tapped = false;
    }

    if(time < (samples * 3.0f) && averageActive) //NOT ENOUGH PRECISION!!!!!
    {
        time += bufferToFill.numSamples;
        //__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%i", time);
    }

    if(averageActive && time >= (samples * 3.0f)) //Resetting the array if the time bettween clicks is larger than 10s
    {
        //__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "reset");
        averageActive = false;
        //CALL RESET ARRAY HERE
        timeAVGNum = 0;
        timeAVGArrayPos = 0;
    }


    if(start) //If set to start metronome
    {
        if(!prevStart)
        {
            myMetro.setTempo(bpm);
            prevStart = true;
        }
        myMetro.getNextBlock(bufferToFill);
    }
    if(!start && prevStart)
    {
        prevStart = false;
    }



}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    //g.drawSingleLineText("Hello This is tap temp", getLocalBounds().getWidth() * 0.5f, getLocalBounds().getHeight()*0.5f, Justification::centred); //Pasting text
    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    float width = getLocalBounds().getWidth();
    float height = getLocalBounds().getHeight();

    for(int i = 0; i < tapThis.size(); ++i)
    {
        tapThis[i] ->setBounds(width * buttonPlaceArray[i * 4], height * buttonPlaceArray[i * 4 + 1], width * buttonPlaceArray[i * 4 + 2], height * buttonPlaceArray[i * 4 + 3]);
    }
}

void MainComponent::tapTempo()
{
    if(tapThis[0] -> getState() == Button::ButtonState::buttonDown) //Making sure button press a button down press
        tapped = true;
    //prevTime = clock.getMillisecondCounter();
}

void MainComponent ::calculateAverage()
{
    bpm = 0;
    int sumTimes = 0;

    for(int i = 0; i < timeAVGNum + 1; ++i)
    {
        sumTimes += timeAVGArray[i];
    }
    //__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%i", sumTimes);
    samples = (float)sumTimes / ((float) timeAVGNum + 1);
    float seconds = samples / (float)currSampleRate;

    bpm = 60.0f / seconds;

    if(start)
    {
        myMetro.setTempo(bpm); //Setting Metronome BPM
    }

    tapThis[0] -> setButtonText("BPM: " + String(bpm));
    tapThis[0] -> repaint();

    return;
}