/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



#include <math.h>
//==============================================================================
DelayPluginAudioProcessor::DelayPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#else
    :
#endif
    parameters(*this, nullptr, juce::Identifier("DelayPlugin"),
        
        {std::make_unique<juce::AudioParameterFloat>("MASTER", "Master", 
            	juce::NormalisableRange{0.0f, 1.0f, 0.01f, 1.0f, false}, 0.5f),
        std::make_unique<juce::AudioParameterFloat>("MIX", "Mix", 
            	juce::NormalisableRange{0.0f, 1.0f, 0.01f, 1.0f, false}, 0.5f),
        std::make_unique<juce::AudioParameterFloat>("FEEDBACK", "Feedback",
            	juce::NormalisableRange{0.0f, 1.0f, 0.01f, 1.0f, false}, 0.4f),
        std::make_unique<juce::AudioParameterFloat>("DELAYTIMEMS", "Delay Time",
				juce::NormalisableRange{20.0f, 2000.0f, 1.0f, 1.0f, false}, 500.0f),
		std::make_unique<juce::AudioParameterFloat>("CHORUSDEPTH", "Chorus Depth",
				juce::NormalisableRange{0.0f, 1.0f, 0.01f, 1.0f, false}, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("CHORUSRATE", "Chorus Rate",
            	juce::NormalisableRange{0.0f, 10.0f, 0.01f, 1.0f, false}, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("CHORUSMIX", "Chorus Mix",
				juce::NormalisableRange{0.0f, 1.0f, 0.01f, 1.0f, false}, 0.5f),
        std::make_unique<juce::AudioParameterBool>("SYNCENABLED", "Sync", false),
		std::make_unique<juce::AudioParameterBool>("HIGHPASSSTATE", "Filter Mode", false),
        std::make_unique<juce::AudioParameterBool>("PINGPONGENABLED", "Ping Pong", false),
		std::make_unique<juce::AudioParameterFloat>("CUTOFFFREQ", "Cutoff Freq", 
				juce::NormalisableRange{360.f, 30000.f, 1.f, 0.2f, false}, 1000.f),
        std::make_unique<juce::AudioParameterChoice>("SYNCSETTING", "Sync Setting",
              StringArray{ "1/1", "1/1D", "1/1T", "1/2", "1/2D", "1/2T", "1/4", "1/4D", "1/4T", "1/8", "1/8D", "1/8T", "1/16", "1/16D", "1/16T", "1/32" }, 0)
		})
    {
        MasterValue = parameters.getRawParameterValue("MASTER");
        MixValue = parameters.getRawParameterValue("MIX");
        FeedbackValue = parameters.getRawParameterValue("FEEDBACK");
        DelayTimeValue = parameters.getRawParameterValue("DELAYTIMEMS");
        ChorusDepthValue = parameters.getRawParameterValue("CHORUSDEPTH");
        ChorusRateValue = parameters.getRawParameterValue("CHORUSRATE");
        ChorusMixValue = parameters.getRawParameterValue("CHORUSMIX");
        PingPongEnabledValue = parameters.getRawParameterValue("PINGPONGENABLED");
        SyncEnabledValue = parameters.getRawParameterValue("SYNCENABLED");
        HighPassStateValue = parameters.getRawParameterValue("HIGHPASSSTATE");
        CutOffFreqValue = parameters.getRawParameterValue("CUTOFFFREQ");
        SyncSettingValue = parameters.getRawParameterValue("SYNCSETTING");
}

DelayPluginAudioProcessor::~DelayPluginAudioProcessor()
{
}

//==============================================================================
const juce::String DelayPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayPluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DelayPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DelayPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    BufferL.Init(sampleRate, 3000.f);
    BufferR.Init(sampleRate, 3000.f);

    phase = 0.0f;
    phaseIncrement = 2.0f * juce::MathConstants<float>::pi / sampleRate;

}

void DelayPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DelayPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    auto sampleRate = getSampleRate();

    const auto Master = MasterValue->load();
    const auto Mix = MixValue->load();
	const auto Feedback = FeedbackValue->load();
	const auto DelayTimeMs = DelayTimeValue->load();
	const auto ChorusDepth = ChorusDepthValue->load();
	const auto ChorusRate = ChorusRateValue->load();
	const auto ChorusMix = ChorusMixValue->load();
	const auto PingPongEnabled = *PingPongEnabledValue < 0.5f ? false : true;
	const auto SyncEnabled = *SyncEnabledValue < 0.5f ? false : true;
	const auto highPassState = *HighPassStateValue < 0.5f ? false : true;
	const auto CutOffFreq = CutOffFreqValue->load();
    const auto SyncSetting = SyncSettingValue->load();
    

    // Clear the buffer
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples()); //clear the buffer for both channels

    // Get write pointers to audio buffers.
    float* channelDataL = buffer.getWritePointer(0); // get sample from channel 0 (left)
    float* channelDataR = buffer.getWritePointer(1); // get sample from channel 1 (right)

    //check to see if SyncEnabled is selected or not. If selected then calculate delay time relative to
    // time signature,
    //otherwise the delay time is set in milliseconds.

   
    FinalDelayTime = SyncEnabled ? CalcDelayTime(Playhead.bpm, SyncSetting) : DelayTimeMs; 

    // Chorus parameters
    float currentDepth = ChorusDepth * (sampleRate / 10000.0f); 
    float currentRate = ChorusRate;

    filtBuffer.resize(buffer.getNumChannels(), 0.0f);
    const auto sign = highPassState ? -1.0f : 1.0f; // 1.0f for high pass, -1.0f for low pass

    // Calculate allPass filter coefficients
    const auto tan = std::tan(juce::MathConstants<float>::pi * CutOffFreq / sampleRate); 
    const auto a1 = (tan - 1.0f) / (tan + 1.0f); 


    // Process buffer
    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {

        // LFO calculation
        float lfo = std::sin(phase);
        phase += 2.0f * juce::MathConstants<float>::pi * currentRate / sampleRate; //get current phase
        if (phase >= 2.0f * juce::MathConstants<float>::pi) // if phase is greater than 2pi then we need to wrap it
            phase -= 2.0f * juce::MathConstants<float>::pi;

        // Modulated delay times

        float randomModulationL = juce::Random::getSystemRandom().nextFloat() * 0.002f - 0.001f; // Random value between -0.001 and 0.001
        float randomModulationR = juce::Random::getSystemRandom().nextFloat() * 0.002f - 0.001f; // Random value between -0.001 and 0.001

        float modulatedDelayL = FinalDelayTime + (lfo * currentDepth)+randomModulationL;
        float modulatedDelayR = (PingPongEnabled) ? FinalDelayTime/2 - (lfo * currentDepth)+randomModulationR : FinalDelayTime - (lfo * currentDepth)+randomModulationR;


        // Check to see if input is mono and output is mono, then take the data from the left channel
        if (totalNumInputChannels == 1 && totalNumOutputChannels == 1)
        {

            // Get current input sample from left channel
            float inputMono = channelDataL[sample]; //read raw sample from input buffer

            float wetDelay = BufferL.BufferRead(modulatedDelayL); //read from buffer with modulated delay

            const auto allPassFilter = a1 * inputMono + filtBuffer[0];

            filtBuffer[0] = inputMono - a1 * allPassFilter;


            const auto filterOut = 0.5f * (inputMono + sign * allPassFilter);

            float bufferinput = (filterOut + Feedback * wetDelay); //apply feedback and mix with dry and wet


            BufferL.BufferWrite(bufferinput); //write to buffer

            float output = (inputMono * (1.f - Mix)) + (Mix * wetDelay); //apply mix between dry and wet

            channelDataL[sample] = output * Master; //apply master gain to output

        }


        // Check if our plugin is mono input and stereo output. If so, then take the data from the left channel
        //  and duplicate it for the right channel and apply the delay to stereo out left and right
        if (totalNumInputChannels == 1 && totalNumOutputChannels == 2)
        {
            // Get current input samples from buffer and duplicate for second channel.
            float inputL = channelDataL[sample]; //Get left channel data
            float inputR = inputL; // and create a copy of the left channel data for the right channel if mono

            float dryL = inputL; //create unaltered dry channels of input stream
            float dryR = inputR;

            float wetDelayL = BufferL.BufferRead(modulatedDelayL); //read from left buffer with modulated delay
            float wetDelayR = BufferR.BufferRead(modulatedDelayR); //read from right buffer with modulated delay

            const auto allPassFilterL = a1 * inputL + filtBuffer[0];
            const auto allPassFilterR = a1 * inputR + filtBuffer[1];

            filtBuffer[0] = inputL - a1 * allPassFilterL;
            filtBuffer[1] = inputR - a1 * allPassFilterR;

            const auto filterOutL = 0.5f * (inputL + sign * allPassFilterL);
            const auto filterOutR = 0.5f * (inputR + sign * allPassFilterR);


            float bufferinputL = (filterOutL + Feedback * wetDelayL); //apply feedback and mix with dry and wet
            float bufferinputR = (filterOutR + Feedback * wetDelayR);



            BufferL.BufferWrite(bufferinputL);
            BufferR.BufferWrite(bufferinputR);
            

            float outputL = (dryL * (1.f - Mix)) + (Mix * wetDelayL); //apply mix between dry and wet
            float outputR = (dryR * (1.f - Mix)) + (Mix * wetDelayR);


            channelDataL[sample] = outputL * Master; //apply master gain to output
            channelDataR[sample] = outputR * Master;

        }

        // Check both inputs and outputs are stereo. 
        if (totalNumInputChannels == 2 && totalNumOutputChannels == 2)
        {

            // Get current input samples from buffers.
            float inputL = channelDataL[sample]; //read raw sample from left input buffer
            float inputR = channelDataR[sample]; //read raw sample from right input buffer

            float dryL = inputL; //create unaltered dry channels of input stream
            float dryR = inputR;

            float wetDelayL = BufferL.BufferRead(modulatedDelayL); //read from left buffer with modulated delay
            float wetDelayR = BufferR.BufferRead(modulatedDelayR); //read from right buffer with modulated delay


            const auto allPassFilterL = a1 * inputL + filtBuffer[0];
            const auto allPassFilterR = a1 * inputR + filtBuffer[1];

            filtBuffer[0] = inputL - a1 * allPassFilterL;
            filtBuffer[1] = inputR - a1 * allPassFilterR;

            const auto filterOutL = 0.5f * (inputL + sign * allPassFilterL);
            const auto filterOutR = 0.5f * (inputR + sign * allPassFilterR);


            float bufferinputL = (filterOutL + Feedback * wetDelayL); //apply feedback and mix with dry and wet
            float bufferinputR = (filterOutR + Feedback * wetDelayR);


            BufferL.BufferWrite(bufferinputL);
            BufferR.BufferWrite(bufferinputR);
            //}

            float outputL = (dryL * (1.f - Mix)) + (Mix * wetDelayL);
            float outputR = (dryR * (1.f - Mix)) + (Mix * wetDelayR);


            channelDataL[sample] = outputL * Master;
            channelDataR[sample] = outputR * Master;        

        }

    }

}

//==============================================================================
bool DelayPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayPluginAudioProcessor::createEditor()
{
    //return new GenericAudioProcessorEditor(*this);
    
    
    return new DelayPluginAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void DelayPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayPluginAudioProcessor();
}
