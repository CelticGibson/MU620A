
#pragma once

#include <iostream>
#include <cmath>

 class CircularBuffer
{
    
public:
    
    // Create the circular buffer
    CircularBuffer();
    // Destroy the circular buffer on close
    ~CircularBuffer();
    
    
    //Public member functions
    void Init(int sr, float max_delay_time_ms);
    void ClearBuffer();
    void BufferWrite(float writePtr);
    float BufferRead(float delay_time_ms);
    
    
    
    
private:
    
    
    //Initialise private variables 
    unsigned int WritePointer = 0;
    float SampleRate = 0.0f;
    unsigned int bufferLength = 0;
    
    int delayTimeSamples = 0;
    float delayFractionalSamples = 0.0f;
    
    float MaxDelayTime = 0.0f;
    
    // Unique ptr array to use as circular buffer, self deletes when goes out of scope on close.
    std::unique_ptr<float[]> delayBuffer = nullptr;
    

    // linear interpolation function for ensuring the 
    // delay time accounts for fractional sample delays rather 
    // than being restricted to whole sample steps in the buffer
    // output = (1.0−t)*a+t*b
    //
    // a = The first value (e.g., the current sample from the buffer)
    // b = The second value(e.g., the previous sample from the buffer)
    // t = The interpolation factor, typically ranging from 0.0 to 1.0, 
    // representing the fractional distance between the two values

    
    float linear_interp(float a, float b, float t)
    {
        return a * (1.0f - t) + (b * t);
    }
    
};
