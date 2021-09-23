#include "pitches.h"
#define REST      0 


int divider = 0, noteDuration = 0;
void tonoRechazar(){ 
  int tempo = 120; 
  int buzzer = 5; 
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
    tone(buzzer, melody[thisNote], noteDuration*0.9); 
    delay(noteDuration); 
    noTone(buzzer);
  }
}

void setup() {
  
}
bool debeSalir = false;

void loop() {
  if(!debeSalir){
    tonoRechazar();
    debeSalir = true;
  }
}
