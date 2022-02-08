# arpeggiator

## overview
This is a terminal program which can be used to create and play audio files from scratch. After navigating into the 'src' directory, simply press 'arpeggiator' and then enter and the application will begin. An arpeggio in music theory is simply a chord which is broken up into its subsequent notes. When played in succession, arpeggios can sound musical and  can highly varying characteristics. Two types of arpeggios can be generated, along with customizable attributes such as speed, randomness, and starting note. The .wav file which is created is made by using digital signal processing techniques, creating a saw wave with decay to create a pleasant synth sound. The outputed .wav file is approximately 8 seconds long and can be found in the 'out' folder. There are two examples already in the 'out' folder, with different types of scales and arpeggio speeds.

## techniques
The main processing done in this application is in writing the .wav file, which requires precise formatting of headers and types to be used properly. Signal processing techniques involving sample rate, bit depth, and audio channels are used to accurately create the audio file. Once the headers for the .wav file are complete, the actual audio data is written in, which uses the saw oscillator from the oscillator.h file. An oscillator is simply a periodic function which can be used in audio synthesis to create a note with a specific timbre. A plain saw wave can sound harsh however, so a decay envelope is used from the decay.h file to decrease the volume of the oscillator over time similar to how real instruments get quieter after a note is played. To construct the actual arpeggios required some simple music theory knowledge, which involves finding out the proper frequencies to construct based on the western tuning system.

## terminal program
![Screenshot](/images/terminal_footage.png?raw=true "Optional Title")
