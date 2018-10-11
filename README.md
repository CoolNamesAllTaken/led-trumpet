John McNelly

(johnmcnelly.com)[http://johnmcnelly.com]

## Overview
This project provides Teensyduino code for a trumpet-mounted LED system.  The LEDs used in this project are a SK6812 package, and the code is intended to run on a Teensy 3.2 or similar device.

## Hardware
More project info will be posted to (johnmcnelly.com)[http://johnmcnelly.com].  This README will be updated with images soon.

### Component List
- Teensy 3.2
- SK6812 RGBW individually-addressable LED strip
- Castle 10A BEC (5V output)
- Adafruit ADA1713 Electret Microphone Amplifier (MAX9814 with gain control)

## Software

### Program Layout
The primary .ino file is led_trumpet.ino, and the additional .ino files are automatically concatenated with it when the led_trumpet sketch folder is compiled with the Arduino IDE (with Teensyduino installed).

### Dependencies
This program requires multiple libraries that are included with the Teensyduino installer.
- Adafruit_Neopixel.h: Used for addressing the RGBW LEDs.
- Audio.h: Used for audio processing for use in the equalizer pattern (runs an FFT on the audio input to create frequency bins)
- Bounce.h: Used for debouncing button inputs
