#include <Audio.h> // used in spectrum_analyzer.ino for FFT

#define SA_MIC_GAIN 2
#define SA_LEFT_STRIP_START_LED 6
#define SA_RIGHT_STRIP_START_LED 0

#define SA_COLOR_STEP 15
#define SA_COLOR_THRESHOLD 0.2
#define SA_WHITE_THRESHOLD 0.7

// contains code from https://forum.pjrc.com/threads/45418-FFT-analysis-using-teensy-3-2

AudioInputAnalog         adc1(MIC_PIN);             //default for adc1 is A2
AudioAnalyzeFFT1024      fft1024; 
AudioConnection          patchCord1(adc1, fft1024);

void spectrum_analyzer_setup() {
	// Audio connections require memory to work.
	AudioMemory(12);

	fft1024.windowFunction(AudioWindowHanning1024); // Configure the window algorithm to use
}

void spectrum_analyzer() {

	// An array to hold the 16 frequency bands
	float levels[16];
	for (int i=0; i<16; i++) levels[i] = 0; // initialize levels to 0

	while (checkButtons()) {
		if (fft1024.available()) {
			// each time new FFT data is available, bin it into 16 channels
			// Note- response doesn't seem flat so each is normalized <experimenting!
			// Note- these calculations go very fast!!

			levels[0] =  fft1024.read(0) * 1.21 * SA_MIC_GAIN + levels[0] / 2; // simple 2-term moving average
			levels[1] =  fft1024.read(1) * 1.18 * SA_MIC_GAIN + levels[1] / 2;
			levels[2] =  fft1024.read(2, 3) * 1.15 * SA_MIC_GAIN + levels[2] / 2;
			levels[3] =  fft1024.read(4, 6) * 1.12 * SA_MIC_GAIN + levels[3] / 2;
			levels[4] =  fft1024.read(7, 10) * 1.09 * SA_MIC_GAIN + levels[4] / 2;
			levels[5] =  fft1024.read(11, 15) * 1.06 * SA_MIC_GAIN + levels[5] / 2;
			levels[6] =  fft1024.read(16, 22) * 1.03 * SA_MIC_GAIN + levels[6] / 2;
			levels[7] =  fft1024.read(23, 32) * 1.00 * SA_MIC_GAIN + levels[7] / 2;
			levels[8] =  fft1024.read(33, 46) * 0.96 * SA_MIC_GAIN + levels[8] / 2;
			levels[9] =  fft1024.read(47, 66) * 0.93 * SA_MIC_GAIN + levels[9] / 2;
			levels[10] = fft1024.read(67, 93) * 0.90 * SA_MIC_GAIN + levels[10] / 2;
			levels[11] = fft1024.read(94, 131) * 0.87 * SA_MIC_GAIN + levels[11] / 2;
			levels[12] = fft1024.read(132, 184) * 0.84 * SA_MIC_GAIN + levels[12] / 2;
			levels[13] = fft1024.read(185, 257) * 0.81 * SA_MIC_GAIN + levels[13] / 2;
			levels[14] = fft1024.read(258, 359) * 0.78 * SA_MIC_GAIN + levels[14] / 2;
			levels[15] = fft1024.read(360, 511) * 0.75 * SA_MIC_GAIN + levels[15] / 2;
		}

		/*Serial.printf("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", \
		levels[0], levels[1], levels[2], levels[3], levels[4], levels[5], \
		levels[6], levels[7], levels[8], levels[9], levels[10], levels[11], \
		levels[12], levels[13], levels[14], levels[15]);*/

		int left_strip_num_bins = min(LEFT_STRIP_NUM_LEDS - SA_LEFT_STRIP_START_LED, 16);
		for (int i=0; i < left_strip_num_bins; i++) {
			left_strip.setPixelColor(i+SA_LEFT_STRIP_START_LED, \
				levels[i] > SA_COLOR_THRESHOLD? (255 - SA_COLOR_STEP*i) : 0, \
				levels[i] > SA_COLOR_THRESHOLD? i*SA_COLOR_STEP * levels[i] : 0, \
				0, levels[i] > SA_WHITE_THRESHOLD? 255 : 0);
		}

		int right_strip_num_bins = min(RIGHT_STRIP_NUM_LEDS - SA_RIGHT_STRIP_START_LED, 16);
		for (int i=0; i < right_strip_num_bins; i++) {
			int line = levels[i] * 100;
			right_strip.setPixelColor(i+SA_RIGHT_STRIP_START_LED, \
				levels[i] > SA_COLOR_THRESHOLD? (255 - SA_COLOR_STEP*i) : 0, \
				levels[i] > SA_COLOR_THRESHOLD? i*SA_COLOR_STEP * levels[i] : 0, \
				0, levels[i] > SA_WHITE_THRESHOLD? 255 : 0);
		}

		left_strip.show();
		right_strip.show();
	}
}