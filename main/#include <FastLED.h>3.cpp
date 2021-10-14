#include <FastLED.h>
#include <MFRC522.h>
#include <SPI.h>
#include "pitches.h" 

// led matrix
#define LED_PIN  6
#define COLOR_ORDER GRB
#define CHIPSET WS2812
#define BRIGHTNESS 16
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define NUM_LEDS (MATRIX_WIDTH * MATRIX_HEIGHT)

// vibration
#define MOTOR_PIN  3

// buzzer
#define BUZZER 5

// rfid
#define RST_PIN 9
#define SS_PIN  10

CRGB leds[ NUM_LEDS ];
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  ledMatrixSetup();
  rfidSetup();
}

void loop() {
  if ( !isNewCardAvailable() ) {
    return;
  }

  String uid = readUID();
  if (isValidUid(uid)) {
    drawFive();
    //drawHappyFace();
    //aprobarReact();
    //resetMatrix();
  } else {
    drawHeart();
    //drawSadFace();
    //rechazarReact();
    //resetMatrix(); 
  }

}

void react(
  int melody[], 
  int durations[], 
  size_t songLength, 
  int vibrationLevel[]) {
  
  for (int thisNote = 0; thisNote < songLength; thisNote++){
    int duration = 1000/ durations[thisNote];
    tone(BUZZER, melody[thisNote], duration);
    analogWrite(MOTOR_PIN, vibrationLevel[thisNote]);
    int pause = duration * 1.3;
    delay(pause);
    noTone(BUZZER);
  }
  analogWrite(MOTOR_PIN, 0);
}

void aprobarReact(){
  int melody[] = { NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5 }; 
  int durations[] = { 8, 8, 8, 1};
  int vibrationLevel[] = {255, 0, 0, 255};
  react(melody, durations, sizeof(melody)/sizeof(int), vibrationLevel);
}

void rechazarReact(){ 
  int melody[] = { NOTE_F3, NOTE_E3, NOTE_F3, NOTE_C3 }; 
  int durations[] = { 8, 8, 8, 1};
  int vibrationLevel[] = {124, 255, 255, 124};
  react(melody, durations, sizeof(melody)/sizeof(int), vibrationLevel);
}

// rfid
void rfidSetup() {
  while (!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

bool isNewCardAvailable() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return false;
  }

  return true;
}

String readUID() {
  String uid = "";

  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    uid.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  uid.toUpperCase();

  mfrc522.PICC_HaltA(); 

  Serial.print(F("uid: "));
  Serial.println(uid);
  return uid;
}

bool isValidUid(String uid) {
  byte keys_size = 2; 
  const char* keys[] = {"E3C3B979", "F0EE4BA8"};
  
  for (byte i = 0; i < keys_size; i++) {
    const char* key = keys[i];
    if (uid.compareTo(key) == 0) {
      return true;
    }
  }
  return false;
}

// led matrix
int tiempoEspera = 1000;

void ledMatrixSetup(){
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  resetMatrix(); 
}

uint16_t XY( uint8_t x, uint8_t y) {
  return (y * MATRIX_WIDTH) + x;
}

void displayImage(uint64_t image, std::function<CHSV(int, int)>) {
  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      if (bitRead(row, j) == 1) {
        leds[ XY(i, j)]  = color(i, j);
      }
    }
  }
  FastLED.show();
}

CHSV hueBlack(int i, int j) {
  return CHSV(0, 0, 0);
}

CHSV hueGreen(int i, int j) {
  return CHSV(HSVHue::HUE_GREEN, 255, 255);
}

CHSV hueRed(int i, int j) {
  return CHSV(HSVHue::HUE_RED, 255, 255);
}

CHSV huePink(int i, int j) {
  return CHSV(HSVHue::HUE_PINK, 255, 255);
}

void resetMatrix() {
    displayImage(0xffffffffffffffff, hueBlack);
}

void drawHappyFace() {
    displayImage(0x003c420000666600, hueGreen);
}

void drawSadFace() {
    displayImage(0x00423c0000666600, hueRed);
}

void drawImages(
  const uint64_t images[], 
  int imagesSize, 
  int delayedBy,
  CHSV (*color)(int, int)) {
  
  for (int i = 0; i < imagesSize; i++){
    displayImage(images[i], color);
    delay(delayedBy);
    resetMatrix();
  }
}

void drawHeart() {
  const uint64_t images[] = {
    0x183c7effffff6600,
    0x0038040418043c00
  };
  drawImages(images, sizeof(images)/8, 500, huePink);
}

void drawFive() {
  const uint64_t images[] = {
    0xffffffffffffffff,
    0x7f7f7f7f7f7f7f7f,
    0x1f3f1f1f1f3f3f1f,
    0x1f3f1f1f1f3f3f1f,
    0x0f3f0f0f1f2f3f0f,
    0x073f07071f273f07,
    0x033b07071b233f03,
    0x0139050519213d01,
    0x0038040418203c00
  };

  const uint32_t ms = millis();
  int32_t xHueDelta32 = ((int32_t)cos16( ms * (39/1) ) * (310 / MATRIX_HEIGHT));
  int32_t yHueDelta32 = ((int32_t)cos16( ms * (27/1) ) * (350 / MATRIX_WIDTH));
  
  drawImages(images, sizeof(images)/8, 200, 
    [xHueDelta32, yHueDelta32](int i, int j) -> CHSV {
      uint8_t pixelHue = xHueDelta32 * i + yHueDelta32 * j;
      return CHSV(pixelHue, 255, 255);
    }
  );
}