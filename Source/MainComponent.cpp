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

    auto* button = tapThis.add(new TextButton("yeahTapMe"));
    button -> onClick= [this]() { tapTempo(); };
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

    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    //__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "loopy");
    if(tapped)
    {
        //__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%i", currSampleRate);
        if(averageActive)
        {
            timeAVGArray[timeAVGArrayPos] = time; //Adding element to the array
            calculateAverage();
            timeAVGArrayPos++;
            timeAVGArrayPos = timeAVGArrayPos % 10;
            //__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%i", time);
            timeAVGNum = timeAVGNum < 9 ? timeAVGNum + 1: 9;

        }
        else
        {
            averageActive = true;
        }
        time = 0;
        tapped = false;
    }

    if(time < (currSampleRate * 6) && averageActive) //NOT ENOUGH PRECISION!!!!!
    {
        time += bufferToFill.numSamples;
        //__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%i", time);
    }

    if(averageActive && time >= (currSampleRate * 6)) //Resetting the array if the time bettween clicks is larger than 10s
    {
        //__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "reset");
        averageActive = false;
        //CALL RESET ARRAY HERE
        timeAVGNum = 0;
        timeAVGArrayPos = 0;
    }

    /*for(int channels=0; channels<bufferToFill.buffer->getNumChannels();channels++)
    {
        float* const buff = bufferToFill.buffer->getWritePointer(channels, bufferToFill.startSample);

        for(int sample=0; sample<bufferToFill.numSamples;sample++)
        {
            if(averageActive)
           {
                buff[sample] = (noise.nextFloat())-0.5f;
            } else
            {
                buff[sample] = 0;
            }
        }
    }*/

    //Clearing buffer to stop random noise
    //bufferToFill.clearActiveBufferRegion();
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

    for(auto* buttons : tapThis)
    {
        buttons -> setBounds(width * 0.025f, height * 0.025f, width*0.95, height*0.95f);
    }
}

void MainComponent::tapTempo()
{
    tapped = true;
}

void MainComponent ::calculateAverage()
{
    float bpm = 0;
    int sumTimes = 0;

    for(int i = 0; i < timeAVGNum + 1; ++i)
    {
        sumTimes += timeAVGArray[i];
    }
    //__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%i", sumTimes);
    float seconds = (float)sumTimes / ((float) currSampleRate *((float) timeAVGNum + 1));

    bpm = 60.0f / seconds;

    tapThis[0] -> setButtonText("BPM: " + String(bpm));
    tapThis[0] -> repaint();

    return;
}