<h3> The Code </h3>

This code is the implementation of a delay plugin built using the JUCE framework. The plugin provides a flexible stereo delay effect with various features, such as modulation, feedback, filtering, and syncing capabilities. 

The plugin uses circular buffers to manage the delayed audio. It reads and writes to these buffers while applying the calculated delay time, feedback, and dry/wet mix. Without linear interpolation, accessing non-integer sample positions in a circular buffer could lead to audio artifacts such as clicks or pops. These artifacts occur because the signal would jump abruptly between sample values. By using linear interpolation, the transition between samples is smoothed out, which is crucial for maintaining the integrity of the audio signal, especially in real-time audio effects like delay, chorus, and flanging.


output = a*(1.0−t)+b*t

The function computes the weighted average of a and b based on the fractional value stored in t. If t = 0 then the first sample value is read only. If t = 1 then return the sample held in b only. If t is between 0 and 1, then the sample values are porportionate between a and b. This alows for fractional samples to ensure a smooth transition.

<b>Synchronised delay time:</b>
Along with the manual delay time which is selectable by the user, the plugin can also synchronize delay times to the host's Beats Per Minute, allowing for rhythmically synced delays. The plugin offers a variety of musical note divisions for the delay time, which are selectable via the Sync Delay button. These divisions include standard note values like quarter notes (1/4), eighth notes (1/8), sixteenth notes (1/16), as well as dotted and triplet variations (e.g., 1/4D for a dotted quarter note or 1/4T for a quarter note triplet) and are all accessible via a drop down selection box.

<b>Stereo/mono handling:</b>
The plugin supports different input/output configurations, including mono-to-mono, mono-to-stereo and stereo-to-stereo. If the input is mono and the output is stereo, the plugin duplicates the mono input across both stereo channels. If the input/output contain either mono/stereo or stereo/stereo then the ping pong effect can be used. A halving of the delay time of one of the channels will produce this effect. 

<b>Low/High Pass Filter:</b> The plugin offers two main filter modes: Low-Pass Filter and High-Pass Filter. These modes are selectable via the "Filter Mode" parameter, which allows you to choose between attenuating either the high frequencies (low-pass mode) or the low frequencies (high-pass mode).
When the low-pass filter mode is enabled, frequencies above a certain cutoff point are gradually attenuated. This means that the higher frequencies of the delayed signal are reduced, resulting in a warmer, darker more vintage delay sound.

When the high-pass filter mode is enabled, frequencies below a certain cutoff point are gradually attenuated. This reduces the lower frequencies of the delayed signal, allowing the delay to sound thinner and less boomy. The high-pass filter is particularly useful for removing unwanted low-end rumble or for making the delay effect more subtle in the mix. This is especially useful when applying delay to a drum track where you might not want the bass drum repeating.

This effect is achieved by utilising an allpass filter

fc = cutoff frequency
fs = sample rate

tan = π*fc/fs

a1 = tan−1/tan+1

and then combined with the original dry signal with either postive or negative phase dependent on whether Low Pass (-1.0f) or High Pass (1.0f) is selected. The cutoff frequency varies the centre point of the phase transition between dry and processed data and the output is the difference.



<b>Chorus/LFO:</b> The plugin modulates the delay time using a low-frequency oscillator (LFO). The LFO is a wrap around sine wave that varies the delay time smoothly for each channel by altering the phase over time, creating a chorus effect. Additionally, a small random modulation is applied to both channels to introduce subtle variations and make the effect more natural. 

<b>Parameters:</b>
<ul>
<li><b>Master:</b></li>Controls the overall output level.
<li><b>Mix:</b></li> Blends between the dry (unaltered) signal and the wet (delayed) signal.
<li><b>Feedback:</b></li> Adjusts the amount of the delayed signal that is fed back into the delay buffer.
<li><b>Delay Time:</b></li> Sets the delay time in milliseconds or, when sync is enabled, based on musical note divisions.
<li><b>Chorus Depth and Rate:</b></li> Controls the depth and rate of the LFO modulation applied to the delay time, creating a chorus effect.
<li><b>Chorus Mix:</b></li> Balances the mix between the original and modulated (chorus) signals.
<li><b>Ping Pong:</b></li> When enabled, makes the delay alternate between the left and right channels.
<li><b>Sync:</b></li> When enabled, syncs the delay time to the host tempo.
<li><b>Filter Mode:</b></li>Switches between high-pass and low-pass filtering of the delayed signal.
<li><b>Cutoff Frequency:</b></li>Sets the cutoff frequency for the filter applied to the delayed signal.
</ul>

The process Block method is the core engine of the plugin's audio processing. It handles the real-time input stream into the audio buffer, applying the delay effects with modulation and filtering and then sending the results to the output stream. 

If the plugin is used in a DAW, due to the <b>AudioProcessorValueTreeState</b>, which connects the parameters with each control surface, all parameters can be controlled in the DAW using automation envelopes. 
