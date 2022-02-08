#include <math.h>
#include <fstream>

#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#ifndef M_PI
    #define M_PI 3.14159265
#endif

const int sampleRate = 44100;
const int bitDepth = 16;
const int numChannels = 1;

// abstract oscillator class
class Oscillator {
    public:
        float frequency;
        float amplitude;
        Oscillator(float freq, float amp) : frequency(freq), amplitude(amp) {
        }

        virtual float process() = 0;

        float getFrequency() {
            return frequency;
        }
        float getAmplitude () {
            return amplitude;
        }
};

// oscillator subclass for sine wave
class Sine : public Oscillator {
    public:
        Sine(float freq, float amp) : Oscillator {freq, amp} {
            offset = (2 * M_PI * freq) / sampleRate;
        }
        float process() {
            auto sample = amplitude * sin(angle);
            angle += offset;
            return sample;
        }
    private:
        float offset;
        float angle = 0.0;
};

// oscillator subclass for saw wave
class Saw : public Oscillator {
    public:
        Saw(float freq, float amp) : Oscillator {freq, amp} {
            mod = sampleRate / frequency;
            slope = (2 * amplitude) / mod;
        }
        float process() {
            int curr = time++ % mod;
            float sample;
            if (curr < (mod / 2)) {
                sample = (slope * curr);
            } else {
                curr %= (mod / 2);
                sample = (slope * curr) - amplitude;
            }
            return sample;
        }
    private:
        int mod;
        float slope;
        int time = 0;
};

// oscillator subclass for square wave
class Square : public Oscillator {
    public:
        Square(float freq, float amp) : Oscillator {freq, amp} {
            mod = sampleRate / frequency;
        }
        float process() {
            int curr = time++ % mod;
            float sample;
            if (curr < (mod / 2)) {
                sample = amplitude;
            } else {
                sample = -amplitude;
            }
            return sample;
        }
    private:
        int mod;
        int time = 0;
};

// helper method to write int of specified byte size to file
void writeToFile(std::ofstream &file, int value, int size) {
    file.write(reinterpret_cast<const char*>(&value), size);
}

#endif