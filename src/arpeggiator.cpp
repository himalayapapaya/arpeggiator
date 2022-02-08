#include <iostream>
#include <stdlib.h>
#include <string>
#include <iomanip>
#include <sstream>
#include "oscillator.h"
#include "decay.h"
#include <Windows.h>

int main(int argc, char* argv[]) {
    // wav file constants
    const int bitDepth = 16;
    const int numChannels = 1;

    // declaring attributes, frequencies, and decay envelope
    std::string quality, speed, mode;
    float freq, third, fifth, seventh, octave;
    Decay expDecay = Decay(1.0, 1.3, 0.0001);

    // user input
    std::cout << "Welcome to the arpeggiator!\n" << std::endl;

    std::cout << "Would you like a minor or major scale?" << std::endl;
    std::cin >> quality;
    if (quality != "major" && quality != "minor") {
        std::cout << "\nScale must be of minor or major type." << std::endl;
        return -1;
    }

    std::cout << "\nSelect an arpeggio speed (slow, medium, fast)." << std::endl;
    std::cin >> speed;
    if (speed != "slow" && speed != "medium" && speed != "fast") {
        std::cout << "\nSpeed must be of three types mentioned above." << std::endl;
        return -1;
    }

    std::cout << "\nSelect an arpeggio mode (ascending, descending, alternating, random)." << std::endl;
    std::cin >> mode;
    if (mode != "ascending" && mode != "descending" && mode != "alternating" && mode != "random") {
        std::cout << "\nMode must be of three types mentioned above." << std::endl;
        return -1;
    }

    std::cout << "\nSelect a starting frequency(between 0 and 5512 Hz)." << std::endl;
    std::cin >> freq;
    if (freq < 0 || freq > 5512) {
        std::cout << "\nStarting frequency must be in allowed range." << std::endl;
        return -1;
    }

    // setting up frequencies dependent on scale type
    if (quality == "major") {
        third = freq * pow(2, 5.0/12);
        seventh = freq * pow(2, 11.0/12);
    } else if (quality == "minor") {
        third = freq * pow(2, 4.0/12);
        seventh = freq * pow(2, 10.0/12);
    }
    fifth = freq * pow(2, 7.0/12);
    octave = freq * pow (2, 1);
    Oscillator* root = new Saw(freq, 1);
    Oscillator* root2 = new Saw(freq * 2, 1);
    Oscillator* third_wave = new Saw(third, 1);
    Oscillator* third_wave2 = new Saw(third * 2, 1);
    Oscillator* fifth_wave = new Saw(fifth, 1);
    Oscillator* fifth_wave2 = new Saw(fifth * 2, 1);
    Oscillator* seventh_wave = new Saw(seventh, 1);
    Oscillator* seventh_wave2 = new Saw(seventh * 2, 1);
    Oscillator* octave_wave = new Saw(octave, 1);
    Oscillator* octave_wave2 = new Saw(octave * 2, 1);
    Oscillator* bank[] = {root, root2, third_wave, third_wave2, fifth_wave, fifth_wave2, seventh_wave, seventh_wave2, octave_wave, octave_wave2};
    
    // initialzing speed modification and decay time based on arpeggio speed
    float speedMod;
    if (speed == "slow") {
        speedMod = 0.5;
        expDecay = Decay(1.0, 1.3, 0.0005);
    } else if (speed == "medium") {
        speedMod = 0.25;
        expDecay = Decay(1.0, 1.3, 0.001);
    } else if (speed == "fast") {
        speedMod = 0.125;
        expDecay = Decay(1.0, 1.3, 0.005);
    }

    // creating outfile and checking proper .wav file name
    std::string outFileID;
    std::cout << "\nEnter a name for your .wav file." << std::endl;
    std::cin >> outFileID;
    if (outFileID.length() < 5 || outFileID.substr(outFileID.length() - 4, 4) != ".wav") {
        std::cout << "\nFile name must end in '.wav' formatting" << std::endl;
        return -1;
    }
    std::string path = "../out/" + outFileID;
    std::ofstream outFile(path, std::ios::binary);

    // checking if outfile is open
    if(!outFile.is_open()) {
        std::cout << "unable to create outfile\n";
        return -1;  
    }

    // RIFF Chunk
    outFile << "RIFF"; // chunkID
    outFile << "----"; // chunk size
    outFile << "WAVE"; 

    // fmt chunk
    outFile << "fmt "; // subchunk1ID
    writeToFile(outFile, 16, 4); // subchunk1Size
    writeToFile(outFile, 1, 2);  // audio format
    writeToFile(outFile, 1, 2); // num channels
    writeToFile(outFile, sampleRate, 4); //sample rate
    writeToFile(outFile, sampleRate * bitDepth / 8, 4); //byte rate
    writeToFile(outFile, bitDepth / 8, 2); // block align
    writeToFile(outFile, bitDepth, 2); // bits per sample

    // data chunk
    outFile << "data"; // subchunk2ID
    outFile << "----"; // data

    // calculating parameters for sample output, and finding starting frequency from bank array
    float maxAmplitude = pow(2, bitDepth - 1) - 1;
    int subDur = speedMod * sampleRate;
    bool rising = true;
    int currWave;
    if (mode == "descending") {
        currWave = 9;
    } else {
        currWave = 0;
    }
    int prevWave = currWave;

    // marking initial wav file position
    int preAudioPosition = outFile.tellp();

    // writing to wav file
    int i = 0;
    while (i < 8 * sampleRate) {
        // calculating samples for current frequency and using decay as a scalar
        for (int j = 0; j < subDur; j++) {
            float sample = bank[currWave]->process();
            int intSample = static_cast<int>(sample * maxAmplitude * expDecay.process());
            writeToFile(outFile, intSample, 2);
            i++;
        }

        // selecting next frequency from bank depending on mode
        if (mode == "ascending") {
            currWave++;
            currWave %= 10;
        } else if (mode == "descending") {
            if (currWave == 0) {
                currWave = 9;
            } else {
                currWave--;
            }
        } else if (mode == "alternating") {
            if (rising == true) {
                currWave++;
            } else {
                currWave--;
            }
            currWave %= 10;
            if (currWave == 9) {
                rising = false;
            } else if (currWave == 0) {
                rising = true;
            }
        } else {
            while (currWave == prevWave) {
                currWave = rand() % 10;
            }
            prevWave = currWave;
        }

        // reseting decay envelope for next frequency
        expDecay.reset();
    }
    // marking final position in wave file
    int postAudioPosition = outFile.tellp();

    // finishing header
    outFile.seekp(preAudioPosition - 4);
    writeToFile(outFile, postAudioPosition - preAudioPosition, 4);
    outFile.seekp(4, std::ios::beg);
    writeToFile(outFile, postAudioPosition - 8, 4);

    // closing outfile
    outFile.close();
    std::cout << "\nwav file successfully created" << std::endl;

    // playing soundfile
    std::string audioPath = "../out/" + outFileID;
    PlaySound(audioPath.c_str(), NULL, SND_ALIAS);

    return 0;
}