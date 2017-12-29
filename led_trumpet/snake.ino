#define SK_DELAY 200 // game time step (ms)

void snake_setup() {}

void snake() {
	int snakeLength = 1;
	int maxSnakeLength = min(LEFT_STRIP_NUM_LEDS, RIGHT_STRIP_NUM_LEDS);

	int velocity = 1;
	int positions[maxSnakeLength];
	for (int i=0; i<maxSnakeLength; i++) positions[i] = 0;

	int foodPosition = maxSnakeLength / 2;

	while(true) {
		// update snake position
		if (button2.update() && button2.fallingEdge()) velocity = -1;
		if (button3.update() && button3.fallingEdge()) velocity = 1;
		for (int i=snakeLength-1; i>0; i--) {
			positions[i] = positions[i-1];
		}
		positions[0] += velocity;

		// grow the snake
		if (foodPosition == positions[0]) {
			// snake head found food
			snakeLength += 1;
			positions[snakeLength - 1] = positions[snakeLength - 2] - velocity;

			if (snakeLength >= maxSnakeLength) {
				changePattern();
				return;
			}

			// update food position
			foodPosition = millis() % maxSnakeLength; // use millis() as a random number
		}

		// draw the snake
		clearStrips();
		for (int i=1; i<snakeLength; i++) {
			// snake body is white
			left_strip.setPixelColor(positions[i], 0, 0, 0, int(float(max_brightness/4) * (snakeLength - i + 1) / snakeLength));
			right_strip.setPixelColor(positions[i], 0, 0, 0, int(float(max_brightness/4)* (snakeLength - i + 1) / snakeLength));
		}
		left_strip.setPixelColor(positions[0], max_brightness, 0, 0, 0); // snake head is red
		right_strip.setPixelColor(positions[0], max_brightness, 0, 0, 0);

		// draw the food
		left_strip.setPixelColor(foodPosition, 0, max_brightness, 0, 0); // food is green
		right_strip.setPixelColor(foodPosition, 0, max_brightness, 0, 0);

		left_strip.show();
		right_strip.show();

		// check to see if game is done
		if (button1.update() && button1.fallingEdge()) {
			changePattern();
			return;
		}
		// Serial.printf("snakeLength= %d velocity= %d\n", snakeLength, velocity);
		// Serial.printf("positions= ");
		for (int i=0; i<snakeLength; i++) {
			Serial.printf("%d ", positions[i]);
		}
		Serial.println();
		delay(SK_DELAY);
	}
}