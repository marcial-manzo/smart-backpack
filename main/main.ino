#include <SPI.h>
#include <MFRC522.h>
#include "pitches.h" 

#define LED_GREEN 2
#define LED_RED 4
#define BUZZER 5
#define RST_PIN 9
#define SS_PIN  10
#define MOTOR_PIN  3
#define REST 0

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  ledSetup();
  rfidSetup();
}
void loop() {
  if ( !isNewCardAvailable() ) {
    return;
  }

  String uid = readUID();
  if (isValidUid(uid)) {
    onLedAprobar();
    aprobarReact();
    offLedAprobar();
  } else {
    onLedRechazar();
    rechazarReact();
    offLedRechazar(); 
  }

}

void react(int melody[], int durations[], size_t songLength, int vibrationLevel[]) {
  for (int thisNote = 0; thisNote < songLength; thisNote++){
    int duration = 1000/ durations[thisNote];
    tone(BUZZER, melody[thisNote], duration);
    analogWrite(MOTOR_PIN,vibrationLevel[thisNote]);
    int pause = duration * 1.3;
    delay(pause);
    noTone(BUZZER);
  }
}

void aprobarReact(){
  int melody[] = { NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5 }; 
  int durations[] = { 8, 8, 8, 1};
  int vibrationLevel[] = {10,100,150,0};
  react(melody, durations, sizeof(melody)/sizeof(int),vibrationLevel);
}

void rechazarReact(){ 
  int melody[] = { NOTE_F3, NOTE_E3, NOTE_F3, NOTE_C3 }; 
  int durations[] = { 8, 8, 8, 1};
  int vibrationLevel[] = {50,100,255,0};
  react(melody, durations, sizeof(melody)/sizeof(int),vibrationLevel);
}

int tiempoEspera = 1000;

void ledSetup(){
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
}

void rfidSetup() {
  while (!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
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

bool isNewCardAvailable() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return false;
  }

  return true;
}

String readUID() {
  ++String uid = "";

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
