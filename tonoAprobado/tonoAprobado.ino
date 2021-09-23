#include "pitches.h" 
void tonoAprobar(){
  int melody[] = { NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_B4, NOTE_B4, NOTE_E5 }; 
  int durations[] = { 8, 8, 8, 4, 4, 4};
  int songLength = sizeof(melody)/sizeof(melody[0]);
  for (int thisNote = 0; thisNote < songLength; thisNote++){
    int duration = 1000/ durations[thisNote];
    tone(5, melody[thisNote], duration);
    int pause = duration * 1.3;
    delay(pause);
    noTone(5);
  }
}
void setup() {
}
bool tieneContinuar = true;
void loop() {
  if(tieneContinuar){
    tonoAprobar();
    tieneContinuar = false;
  }
}
