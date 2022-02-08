#include <math.h>
#include <fstream>

#ifndef DECAY_H
#define DECAY_H

class Decay {
    public:
        float base;
        float power;
        float amplitude;
        Decay(float amp, float b, float p) : amplitude(amp), base(b), power(p) {
        }

        float process() {
            return amplitude / pow(base, power * ++offset);
        }

        float getBase() {
            return base;
        }
        float getPower() {
            return power;
        }
        float getAmplitude() {
            return amplitude;
        }

        void reset() {
            offset = 0;
        }
    private:
        int offset = 0;
};

#endif