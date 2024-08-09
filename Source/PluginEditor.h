/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

using namespace juce;


//==============================================================================
/**
*/
class DelayPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DelayPluginAudioProcessorEditor (DelayPluginAudioProcessor&, 
        juce::AudioProcessorValueTreeState& apvts);
    ~DelayPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Slider MasterSlider;
    Label MasterLabel;
    Slider MixSlider;
    Label MixLabel;
    Slider FeedbackSlider;
    Label FeedbackLabel;
    Slider DelayTimeSlider;
    Label DelayTimeLabel;
    TextButton PingPongButton;
    Label PingPongLabel;
    TextButton SyncSelectorButton;
    Label SyncSelectorLabel;
    ComboBox SyncChoice;
    Label SyncChoiceLabel;
    Slider ChorusDepthSlider;
    Label ChorusDepthLabel;
    Slider ChorusRateSlider;
    Label ChorusRateLabel;
    Slider ChorusMixSlider;
    Label ChorusMixLabel;
    TextButton HighPassButton;
    Label HighPassLabel;
    Slider CutOffSlider;
    Label CutOffLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> pingPongAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> syncAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusDepthAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusRateAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusMixAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> highPassAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutOffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> syncChoiceAttachment;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayPluginAudioProcessor& Audioprocessor;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginAudioProcessorEditor)
};
