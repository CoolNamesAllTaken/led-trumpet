#define RNBW_WAIT 20

#define RNBW_LED_COLOR_INCREMENT 20 // affects how quickly color changes from one LED to the next
#define RNBW_LED_COLOR_MOVE_SPEED 24 // affects how fast the rainbow moves (must be factor of 256)

#define RNBW_LED_ACCENT_MOVE_SPEED 50 // affects the move speed of the accent animation
#define RNBW_LED_ACCENT_MOVE_DIST 1 // distance that the accents move each time

#define RNBW_LED_ACTIVE_ACCENT_LENGTH 1 // length of accents (in pixels)
#define RNBW_LED_ACTIVE_ACCENT_SPACING 5 // distance between starts of accents

#define RNBW_LED_IDLE_ACCENT_LENGTH 5 // length of accents (in pixels)
#define RNBW_LED_IDLE_ACCENT_SPACING 60 // distance between starts of accents

void rainbow_setup() {}

void rainbow() {
	bool active_accents[max(LEFT_STRIP_NUM_LEDS, RIGHT_STRIP_NUM_LEDS)]; // array indicating which pixels are activated for the accent effect
	bool idle_accents[max(LEFT_STRIP_NUM_LEDS, RIGHT_STRIP_NUM_LEDS)];

	rnbw_setAccents(active_accents, RNBW_LED_ACTIVE_ACCENT_LENGTH, RNBW_LED_ACTIVE_ACCENT_SPACING);
	rnbw_setAccents(idle_accents, RNBW_LED_IDLE_ACCENT_LENGTH, RNBW_LED_IDLE_ACCENT_SPACING);
	
	while(checkButtons()) {
		uint8_t volume = 0/*readMic(255)*/; // set white max_brightness based on sound
		
		for(uint16_t j=0; j<256; j+=RNBW_LED_COLOR_MOVE_SPEED) {
			// iterate over color offset
			
			if ((millis() / RNBW_LED_ACCENT_MOVE_SPEED) % 2) {
				// shift accent
				if (volume > 0) rnbw_moveAccents(active_accents, RNBW_LED_ACCENT_MOVE_DIST);
				else rnbw_moveAccents(idle_accents, RNBW_LED_ACCENT_MOVE_DIST);
			}

			for(uint16_t i=0; i<left_strip.numPixels(); i++) {
				// iterate over pixels
				uint32_t color = Wheel((RNBW_LED_COLOR_INCREMENT*i + j) & 255);
				
				if (volume > 0) {
					// play "active" animation (rainbow)
					if (active_accents[i]) {
						// led is part of active accent
						left_strip.setPixelColor(i, max_brightness, max_brightness, max_brightness, max_brightness);
					} else {
						// led is active
						left_strip.setPixelColor(i, red(color) * max_brightness/255, green(color) * max_brightness/255, blue(color) * max_brightness/255, 0);
					}
				} else {
					// play "idle" animation
					if (idle_accents[i]) {
							// led is part of idle accent
							left_strip.setPixelColor(i, 0, 0, 0, 255 * max_brightness/255);
						} else {
							// led is idle
							left_strip.setPixelColor(i, red(color) / 4 * max_brightness/255, green(color) / 4 * max_brightness/255, blue(color) / 4 * max_brightness/255, 0);
						}
				}
			}
			for(uint16_t i=0; i<right_strip.numPixels(); i++) {
				// iterate over pixels
				uint32_t color = Wheel((RNBW_LED_COLOR_INCREMENT*i + j) & 255);
				
				if (volume > 0) {
					// play "active" animation (rainbow)
					if (active_accents[i]) {
						// led is part of active accent
						right_strip.setPixelColor(i, max_brightness, max_brightness, max_brightness, max_brightness);
					} else {
						// led is active
						right_strip.setPixelColor(i, red(color) * max_brightness/255, green(color) * max_brightness/255, blue(color) * max_brightness/255, 0);
					}
				} else {
					// play "idle" animation
					if (idle_accents[i]) {
							// led is part of idle accent
							right_strip.setPixelColor(i, 0, 0, 0, max_brightness);
						} else {
							// led is idle
							right_strip.setPixelColor(i, red(color) / 4 * max_brightness/255, green(color) / 4 * max_brightness/255, blue(color) / 4 * max_brightness/255, 0);
						}
				}
			}
			left_strip.show();
			right_strip.show();
			delay(RNBW_WAIT);
		}
	}
}

// Initializes an accent array.  Assumes accent_leds has length LEFT_STRIP_NUM_LEDS
void rnbw_setAccents(bool accent_leds[], int led_accent_length, int led_accent_spacing) {
	// initialize accent pixels to be off
	for (int i = 0; i < LEFT_STRIP_NUM_LEDS; i++) {
		accent_leds[i] = false;
	}

	// turn on select accent pixels
	for (int i = 0; i < LEFT_STRIP_NUM_LEDS; i+=led_accent_spacing) {
		for (int j = 0; j < led_accent_length; j++) {
			accent_leds[i + j] = true;
		}
	}
}

// Moves an accent array.  Assumes accent_leds has length LEFT_STRIP_NUM_LEDS
void rnbw_moveAccents(bool accent_leds[], int dist) {
	bool end_values[abs(dist)];
	for (int i = 0; i < dist; i++) {
		end_values[i] = accent_leds[LEFT_STRIP_NUM_LEDS - dist + i];
	}
	
	for (int i = 0; i < LEFT_STRIP_NUM_LEDS - dist; i++) {
		// iterate backwards from end of accent array, set to value dist pixels ahead
		accent_leds[(LEFT_STRIP_NUM_LEDS - 1) - i] = accent_leds[(LEFT_STRIP_NUM_LEDS - 1) - i - dist];
	}
	for (int i = 0; i < dist; i++) {
		// push old end values onto front of accent array
		accent_leds[i] = end_values[i];
	}
}
