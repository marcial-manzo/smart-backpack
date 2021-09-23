#include <SPI.h>
#include <MFRC522.h>
#include "pitches.h" 

#define LED_GREEN 2
#define LED_RED 4
#define BUZZER 5
#define RST_PIN 9
#define SS_PIN  10
#define REST 0

MFRC522 mfrc522(SS_PIN, RST_PIN);
 
void tonoAprobar(){
  int melody[] = { NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_B4, NOTE_B4, NOTE_E5 }; 
  int durations[] = { 8, 8, 8, 4, 4, 4};
  int songLength = sizeof(melody)/sizeof(melody[0]);
  for (int thisNote = 0; thisNote < songLength; thisNote++){
    int duration = 1000/ durations[thisNote];
    tone(BUZZER, melody[thisNote], duration);
    int pause = duration * 1.3;
    delay(pause);
    noTone(BUZZER);
  }
}

int divider = 0, noteDuration = 0;
void tonoRechazar(){ 
  int tempo = 120; 

  int melody[] = {
   NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16, NOTE_A4,2
  }; 
  int notes = sizeof(melody) / sizeof(melody[0]) / 2; 
  int wholenote = (60000 * 4) / tempo;
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) { 
    divider = melody[thisNote + 1];
    if (divider > 0) { 
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) { 
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    } 
    tone(BUZZER, melody[thisNote], noteDuration*0.9); 
    delay(noteDuration); 
    noTone(BUZZER);
  }
}

int tiempoEspera = 1000;

void ledSetup(){
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
}

void onLedAprobar(){
  digitalWrite(LED_GREEN, HIGH);  
  delay(tiempoEspera);  
}

void offLedAprobar(){  
  digitalWrite(LED_GREEN, LOW);  
  delay(tiempoEspera);
}

void onLedRechazar(){
  digitalWrite(LED_RED, HIGH);  
  delay(tiempoEspera);
}

void offLedRechazar(){
  digitalWrite(LED_RED, LOW);  
  delay(tiempoEspera); 
}

void setup() {
  ledSetup();
  Serial.begin(9600);
  while (!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
  if ( !isNewCardAvailable() ) {
    onLedAprobar();
    tonoAprobar();
    offLedAprobar();
    onLedRechazar();
    tonoRechazar();
    offLedRechazar();
    return;
  }

  String uid = readUID();
}

bool isNewCardAvailable() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return false;
  }

  return false;
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
