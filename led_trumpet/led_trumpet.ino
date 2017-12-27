#include <Adafruit_NeoPixel.h>

#define BUTTON_1_PIN 2
#define BUTTON_2_PIN 3
#define BUTTON_3_PIN 4

#define LEFT_STRIP_PIN 10
#define RIGHT_STRIP_PIN 8
#define LEFT_STRIP_NUM_LEDS 26
#define RIGHT_STRIP_NUM_LEDS 23

#define NUM_PATTERNS 2
#define BRIGHTNESS_STEP 50 // step size for increasing or decreasing brightness

#define MIC_PIN A0
#define MIC_GAIN_PIN 6
#define MIC_OUTPUT_MAX 1024
#define MIC_OUTPUT_MIN 200
#define MIC_READ_TIME 50

#define WHITE_NOISE_THRESHOLD 600

int max_brightness = 255; // maximum LED brightness

Adafruit_NeoPixel left_strip = Adafruit_NeoPixel(LEFT_STRIP_NUM_LEDS, LEFT_STRIP_PIN, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel right_strip = Adafruit_NeoPixel(RIGHT_STRIP_NUM_LEDS, RIGHT_STRIP_PIN, NEO_GRBW + NEO_KHZ800);

int pattern = 0; // current pattern

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  pinMode(BUTTON_3_PIN, INPUT_PULLUP);

  pinMode(MIC_GAIN_PIN, OUTPUT);
  digitalWrite(MIC_GAIN_PIN, 0);
  
  left_strip.setBrightness(max_brightness);
  left_strip.begin();
  left_strip.show(); // initialize all pixels to 'off'
  right_strip.setBrightness(max_brightness);
  right_strip.begin();
  right_strip.show(); // initialize all pixels to 'off'

  rainbow_setup();
  red_blue_setup();

  attachInterrupt(digitalPinToInterrupt(BUTTON_1_PIN), changePattern, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_2_PIN), decreaseMaxBrightness, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_3_PIN), increaseMaxBrightness, FALLING);
}

unsigned int last_button_1_state = true;
void loop() {
  if (pattern == 0) {
    rainbow();
  } else if (pattern == 1) {
    red_blue();
  }

}

void changePattern() {
  pattern ++;
  if (pattern >= NUM_PATTERNS) pattern = 0;
}

void decreaseMaxBrightness() {
  max_brightness -= BRIGHTNESS_STEP;
  if (max_brightness < 0) max_brightness = 0;
}

void increaseMaxBrightness() {
  max_brightness += BRIGHTNESS_STEP;
  if (max_brightness > 255) max_brightness = 255;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return left_strip.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return left_strip.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return left_strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

uint8_t red(uint32_t c) {
  return (c >> 8);
}
uint8_t green(uint32_t c) {
  return (c >> 16);
}
uint8_t blue(uint32_t c) {
  return (c);
}

// Returns mic read value between 0 and max_val
int readMic(uint32_t max_val) {
  unsigned long startMillis = millis();
  int peakToPeak;

  unsigned int signalMax = 0;
  unsigned int signalMin = MIC_OUTPUT_MAX;

  unsigned int sample;

  // collect data for MIC_READ_WINDOW mS
  while(millis() - startMillis < MIC_READ_TIME) {
    sample = analogRead(MIC_PIN);
    if (sample < signalMin) signalMin = sample;
    else if (sample > signalMax) signalMax = sample;
  }

  peakToPeak = signalMax - signalMin;
  Serial.println(peakToPeak);
  peakToPeak -= WHITE_NOISE_THRESHOLD;
  return (float)(peakToPeak > 0 ? peakToPeak : 0) / (MIC_OUTPUT_MAX - WHITE_NOISE_THRESHOLD) * max_val;
}

