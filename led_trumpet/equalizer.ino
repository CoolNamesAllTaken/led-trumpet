#include <Audio.h> // used in equalizer.ino for FFT

#define MIC_GAIN 2

// contains code from https://forum.pjrc.com/threads/45418-FFT-analysis-using-teensy-3-2

AudioInputAnalog         adc1(MIC_PIN);             //default for adc1 is A2
AudioAnalyzeFFT1024      fft1024; 
AudioConnection          patchCord1(adc1, fft1024);

// An array to hold the 16 frequency bands
float eq_levels[16];

void equalizer_setup() {
	Serial.println("in equalizer_setup");
	// Audio connections require memory to work.
  AudioMemory(12);

  // Configure the window algorithm to use
  fft1024.windowFunction(AudioWindowHanning1024);
	Serial.println("finished equalizer_setup");

}

void equalizer() {
	if (fft1024.available()) {
		// each time new FFT data is available, bin it into 16 channels
		// Note- response doesn't seem flat so each is normalized <experimenting!
		// Note- these calculations go very fast!!

		eq_levels[0] =  fft1024.read(0) * 1.21 * MIC_GAIN;
		eq_levels[1] =  fft1024.read(1) * 1.18 * MIC_GAIN;
		eq_levels[2] =  fft1024.read(2, 3) * 1.15 * MIC_GAIN;
		eq_levels[3] =  fft1024.read(4, 6) * 1.12 * MIC_GAIN;
		eq_levels[4] =  fft1024.read(7, 10) * 1.09 * MIC_GAIN;
		eq_levels[5] =  fft1024.read(11, 15) * 1.06 * MIC_GAIN;
		eq_levels[6] =  fft1024.read(16, 22) * 1.03 * MIC_GAIN;
		eq_levels[7] =  fft1024.read(23, 32) * 1.00 * MIC_GAIN;
		eq_levels[8] =  fft1024.read(33, 46) * 0.96 * MIC_GAIN;
		eq_levels[9] =  fft1024.read(47, 66) * 0.93 * MIC_GAIN;
		eq_levels[10] = fft1024.read(67, 93) * 0.90 * MIC_GAIN;
		eq_levels[11] = fft1024.read(94, 131) * 0.87 * MIC_GAIN;
		eq_levels[12] = fft1024.read(132, 184) * 0.84 * MIC_GAIN;
		eq_levels[13] = fft1024.read(185, 257) * 0.81 * MIC_GAIN;
		eq_levels[14] = fft1024.read(258, 359) * 0.78 * MIC_GAIN;
		eq_levels[15] = fft1024.read(360, 511) * 0.75 * MIC_GAIN;
   }
   Serial.printf("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", \
   	eq_levels[0], eq_levels[1], eq_levels[2], eq_levels[3], eq_levels[4], eq_levels[5], \
   	eq_levels[6], eq_levels[7], eq_levels[8], eq_levels[9], eq_levels[10], eq_levels[11], \
   	eq_levels[12], eq_levels[13], eq_levels[14], eq_levels[15]);
}