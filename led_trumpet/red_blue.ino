#define RB_WAIT 50 // millisecond wait time between flash steps

bool rb_flash_sequence[] = {1, 0, 1, 0, 1, 0};
unsigned int rb_flash_sequence_length = 0;
unsigned int rb_flash_sequence_index = 0;

bool flash_red = true;

void red_blue_setup() {
  rb_flash_sequence_length = sizeof(rb_flash_sequence) / sizeof(bool);
}

void red_blue() {
	if (rb_flash_sequence[rb_flash_sequence_index]) {
		// strobe on
		if (flash_red) rb_set_red(BRIGHTNESS);
		else rb_set_blue(BRIGHTNESS);
	} else {
		// strobe off
		if(flash_red) rb_set_red(0);
		else rb_set_blue(0);
	}
  rb_flash_sequence_index ++;
  if (rb_flash_sequence_index >= rb_flash_sequence_length) {
  	rb_flash_sequence_index = 0;
  	flash_red = !flash_red;
  }
  left_strip.show();
  right_strip.show();
  delay(RB_WAIT);
}

void rb_set_red(uint8_t brightness) {
	for (int i=0; i < LEFT_STRIP_NUM_LEDS/2; i++) {
		left_strip.setPixelColor(i, brightness, 0, 0, 0);
	}
	for (int i=0; i < RIGHT_STRIP_NUM_LEDS/2; i++) {
		right_strip.setPixelColor(i, brightness, 0, 0, 0);
	}
}

void rb_set_blue(uint8_t brightness) {
	for (int i=LEFT_STRIP_NUM_LEDS/2; i < LEFT_STRIP_NUM_LEDS; i++) {
		left_strip.setPixelColor(i, 0, 0, brightness, 0);
	}
	for (int i=RIGHT_STRIP_NUM_LEDS/2; i < RIGHT_STRIP_NUM_LEDS; i++) {
		right_strip.setPixelColor(i, 0, 0, brightness, 0);
	}
}