
#include "CircularBuffer.h"

// Create a circular buffer
CircularBuffer::CircularBuffer(){};
// Destroy the circular buffer upon closing
CircularBuffer::~CircularBuffer(){};



// Function to clear the circular buffer
void CircularBuffer::ClearBuffer(){
    
    // iterate through buffer
    for (int i = 0; i < bufferLength; i++)
    {
        // set to zero
        delayBuffer[i] = 0.0f;
    } 
}

// Function to initialise the buffer using the sample rate times the maximum desired delay time.
void CircularBuffer::Init(int sampleRate, float max_delay_time_ms) {
    
    // Save variables into object
    SampleRate = sampleRate;
    MaxDelayTime = max_delay_time_ms;
    
    // Calcluate buffer length
    bufferLength = (unsigned int)((MaxDelayTime / 1000) * SampleRate + 1);
    
    // Calculate buffer length to next power of 2 to ensure that the buffer size is always 
    // the smallest power of 2 that is greater than or equal to buffer_length
    bufferLength = (unsigned int)(pow(2, ceil(log(bufferLength) / log(2))));
    
    // Set the delaybuffer array with new buffer length
    delayBuffer.reset(new float[bufferLength]);
    
    // Initialise array values to zero
    ClearBuffer();
    
    // Initialise buffer write pointer
    WritePointer = 0;
    
}

// Function that reads from the buffer with a desired amount of delay in ms. 
// Calculates delay in samples using sample rate. 
float CircularBuffer::BufferRead(float delayTime) {
        
    // Calculate both the normal and fractional delay in samples
    delayTimeSamples = (delayTime / 1000) * SampleRate;
    delayFractionalSamples = (delayTime / 1000) * SampleRate;
    
    // Calculate read pointer
    int ReadPointer = (WritePointer - 1) - delayTimeSamples;

    // Wrap around the read ppointer using modulus
    ReadPointer %=  bufferLength;
   
    // Calculate normal output
    float output = delayBuffer[ReadPointer];
      
    // read previous sample
    int prevReadPointer = ReadPointer - 1;

    // Wrap read pointer for next sample 
    prevReadPointer %= bufferLength;
        
    // calculate input into linear interpolator
    float a = output; //current sample
    float b = delayBuffer[prevReadPointer]; //previous sample
    float t = delayFractionalSamples - delayTimeSamples; //fractional delay for linear interpolator
        
    // calculate fractional delay sample with a linear interpolator
    output = linear_interp(a, b, t);
  
    return output;
}



// Function to write to buffer
void CircularBuffer::BufferWrite(float pointer) {
 
    // Increment write index
    WritePointer++;
    
    // Wrap write index with modulo
    WritePointer %= bufferLength;

    // Write sample to buffer
    delayBuffer[WritePointer] = pointer;
}




