#include <Audio.h> // used in equalizer.ino for FFT

#define MIC_GAIN 2
#define EQ_LEFT_STRIP_START_LED 6
#define EQ_RIGHT_STRIP_START_LED 0

// contains code from https://forum.pjrc.com/threads/45418-FFT-analysis-using-teensy-3-2

AudioInputAnalog         adc1(MIC_PIN);             //default for adc1 is A2
AudioAnalyzeFFT1024      fft1024; 
AudioConnection          patchCord1(adc1, fft1024);

// An array to hold the 16 frequency bands
float eq_levels[16];

void equalizer_setup() {
	// Audio connections require memory to work.
	AudioMemory(12);
	// Configure the window algorithm to use
	fft1024.windowFunction(AudioWindowHanning1024);

	for (int i=0; i<16; i++) eq_levels[i] = 0; // initialize levels to 0
}

void equalizer() {
	if (fft1024.available()) {
		// each time new FFT data is available, bin it into 16 channels
		// Note- response doesn't seem flat so each is normalized <experimenting!
		// Note- these calculations go very fast!!

		eq_levels[0] =  fft1024.read(0) * 1.21 * MIC_GAIN + eq_levels[0] / 2; // simple 2-term moving average
		eq_levels[1] =  fft1024.read(1) * 1.18 * MIC_GAIN + eq_levels[1] / 2;
		eq_levels[2] =  fft1024.read(2, 3) * 1.15 * MIC_GAIN + eq_levels[2] / 2;
		eq_levels[3] =  fft1024.read(4, 6) * 1.12 * MIC_GAIN + eq_levels[3] / 2;
		eq_levels[4] =  fft1024.read(7, 10) * 1.09 * MIC_GAIN + eq_levels[4] / 2;
		eq_levels[5] =  fft1024.read(11, 15) * 1.06 * MIC_GAIN + eq_levels[5] / 2;
		eq_levels[6] =  fft1024.read(16, 22) * 1.03 * MIC_GAIN + eq_levels[6] / 2;
		eq_levels[7] =  fft1024.read(23, 32) * 1.00 * MIC_GAIN + eq_levels[7] / 2;
		eq_levels[8] =  fft1024.read(33, 46) * 0.96 * MIC_GAIN + eq_levels[8] / 2;
		eq_levels[9] =  fft1024.read(47, 66) * 0.93 * MIC_GAIN + eq_levels[9] / 2;
		eq_levels[10] = fft1024.read(67, 93) * 0.90 * MIC_GAIN + eq_levels[10] / 2;
		eq_levels[11] = fft1024.read(94, 131) * 0.87 * MIC_GAIN + eq_levels[11] / 2;
		eq_levels[12] = fft1024.read(132, 184) * 0.84 * MIC_GAIN + eq_levels[12] / 2;
		eq_levels[13] = fft1024.read(185, 257) * 0.81 * MIC_GAIN + eq_levels[13] / 2;
		eq_levels[14] = fft1024.read(258, 359) * 0.78 * MIC_GAIN + eq_levels[14] / 2;
		eq_levels[15] = fft1024.read(360, 511) * 0.75 * MIC_GAIN + eq_levels[15] / 2;
	}

	/*Serial.printf("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", \
	eq_levels[0], eq_levels[1], eq_levels[2], eq_levels[3], eq_levels[4], eq_levels[5], \
	eq_levels[6], eq_levels[7], eq_levels[8], eq_levels[9], eq_levels[10], eq_levels[11], \
	eq_levels[12], eq_levels[13], eq_levels[14], eq_levels[15]);*/

	int left_strip_num_bins = min(LEFT_STRIP_NUM_LEDS - EQ_LEFT_STRIP_START_LED, 16);
	for (int i=0; i < left_strip_num_bins; i++) {
		int line = eq_levels[i] * 100;
		left_strip.setPixelColor(i+EQ_LEFT_STRIP_START_LED, 0, 0, 0, line / 128 * max_brightness);
	}

	int right_strip_num_bins = min(RIGHT_STRIP_NUM_LEDS - EQ_RIGHT_STRIP_START_LED, 16);
	for (int i=0; i < right_strip_num_bins; i++) {
		int line = eq_levels[i] * 100;
		right_strip.setPixelColor(i+EQ_RIGHT_STRIP_START_LED, 0, 0, 0, line / 128 * max_brightness);
	}

   left_strip.show();
   right_strip.show();
}