/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CircularBuffer.h"

using namespace juce;

//==============================================================================
/**
*/
class DelayPluginAudioProcessor : public juce::AudioProcessor 
    
{
public:
    //==============================================================================
    DelayPluginAudioProcessor();
    ~DelayPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;


   // juce::AudioProcessorValueTreeState apvts;
   // juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    //foleys::MagicProcessorState magicState{ *this , apvts };


private:
    //==============================================================================

    float CalcDelayTime(float BPM, int syncSetting)
    {
        // Constants for note value multipliers
        constexpr float FULL_NOTE_MULTIPLIER = 4.0f;
        constexpr float DOTTED_MULTIPLIER = 1.5f;
        constexpr float TRIPLET_MULTIPLIER = 2.0f / 3.0f;
        constexpr float HALF_NOTE_MULTIPLIER = 2.0f;
        constexpr float EIGHTH_NOTE_DIVISOR = 2.0f;
        constexpr float SIXTEENTH_NOTE_DIVISOR = 4.0f;
        constexpr float THIRTY_SECOND_NOTE_DIVISOR = 8.0f;

        // Base delay for a quarter note
        float quarterNoteDelay = 60.0f / BPM;

        // Array of delay times for various note values
        float delayNotes[16] = {
            quarterNoteDelay * FULL_NOTE_MULTIPLIER,             // Full note
            quarterNoteDelay * FULL_NOTE_MULTIPLIER * DOTTED_MULTIPLIER, // Dotted full note
            quarterNoteDelay * FULL_NOTE_MULTIPLIER * TRIPLET_MULTIPLIER, // Full note triplet
            quarterNoteDelay * HALF_NOTE_MULTIPLIER,             // Half note
            quarterNoteDelay * HALF_NOTE_MULTIPLIER * DOTTED_MULTIPLIER, // Dotted half note
            quarterNoteDelay * HALF_NOTE_MULTIPLIER * TRIPLET_MULTIPLIER, // Half note triplet
            quarterNoteDelay,                                    // Quarter note
            quarterNoteDelay * DOTTED_MULTIPLIER,                // Dotted quarter note
            quarterNoteDelay * TRIPLET_MULTIPLIER,               // Quarter note triplet
            quarterNoteDelay / EIGHTH_NOTE_DIVISOR,              // Eighth note
            quarterNoteDelay / EIGHTH_NOTE_DIVISOR * DOTTED_MULTIPLIER, // Dotted eighth note
            quarterNoteDelay / EIGHTH_NOTE_DIVISOR * TRIPLET_MULTIPLIER, // Eighth note triplet
            quarterNoteDelay / SIXTEENTH_NOTE_DIVISOR,           // Sixteenth note
            quarterNoteDelay / SIXTEENTH_NOTE_DIVISOR * DOTTED_MULTIPLIER, // Dotted sixteenth note
            quarterNoteDelay / SIXTEENTH_NOTE_DIVISOR * TRIPLET_MULTIPLIER, // Sixteenth note triplet
            quarterNoteDelay / THIRTY_SECOND_NOTE_DIVISOR        // Thirty-second note
        };

        // Define buffer length (in seconds) and clamp the delay time to avoid exceeding buffer size
        float maxDelayTimeSeconds = 2.0f; // buffer length
        float clampedDelayTime = juce::jmin(delayNotes[syncSetting], maxDelayTimeSeconds);

        // Return delay time in milliseconds
        return clampedDelayTime * 1000.0f;
    }

    float FinalDelayTime = 0.0f;

    AudioPlayHead::CurrentPositionInfo Playhead;

    CircularBuffer BufferL, BufferR;

    std::vector<float> filtBuffer;

    float phase;
    float phaseIncrement;

    juce::AudioProcessorValueTreeState parameters;

    std::atomic<float>* MasterValue = nullptr;
	std::atomic<float>* MixValue = nullptr;
	std::atomic<float>* FeedbackValue = nullptr;
	std::atomic<float>* DelayTimeValue = nullptr;
	std::atomic<float>* PingPongEnabledValue = nullptr;
    std::atomic<float>* SyncEnabledValue = nullptr;
    std::atomic<float>* SyncSettingValue = nullptr;
    std::atomic<float>* ChorusDepthValue = nullptr;
	std::atomic<float>* ChorusRateValue = nullptr;
	std::atomic<float>* ChorusMixValue = nullptr;
    std::atomic<float>* HighPassStateValue = nullptr;
	std::atomic<float>* CutOffFreqValue = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginAudioProcessor)
};
