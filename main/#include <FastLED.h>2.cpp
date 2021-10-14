#include <FastLED.h>

#define LED_PIN  6

#define COLOR_ORDER GRB
#define CHIPSET WS2812

#define BRIGHTNESS 16
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define NUM_LEDS (MATRIX_WIDTH * MATRIX_HEIGHT)

CRGB leds[ NUM_LEDS ];

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
}

void loop()
{
    drawHappyFace();
    FastLED.show();
    delay(5000);
    resetMatrix();
    FastLED.show();
    drawHeart();
    FastLED.show();
    delay(5000);
    resetMatrix();
    FastLED.show();
}

uint16_t XY( uint8_t x, uint8_t y) {
  return (y * MATRIX_WIDTH) + x;
}

void displayImage(uint64_t image, uint8_t h, uint8_t s, uint8_t v) {
  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      if (bitRead(row, j) == 1) {
        leds[ XY(i, j)]  = CHSV(h, s, v);
      }
    }
  }
}

void resetMatrix() {
    displayImage(0xffffffffffffffff, 0, 0, 0);
}

void drawHappyFace() {
    displayImage(0x003c420000666600, HSVHue::HUE_GREEN, 240, 240);
}

void drawSadFace() {
    displayImage(0x00423c0000666600, HSVHue::HUE_RED, 240, 240);
}

void drawHeart() {
    displayImage(0x183c7effffff6600, HSVHue::HUE_PINK, 240, 240);
}