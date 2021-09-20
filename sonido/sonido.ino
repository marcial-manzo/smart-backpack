//A sketch to demonstrate the tone() function

//Specify digital pin on the Arduino that the positive lead of piezo buzzer is attached.
int piezoPin = 8;

void setup() {
pinMode(piezoPin, OUTPUT);
analogWrite(piezoPin,0);

}//close setup

void loop() {

  /*Tone needs 2 arguments, but can take three
    1) Pin#
    2) Frequency - this is in hertz (cycles per second) which determines the pitch of the noise made
    3) Duration - how long teh tone plays
  */
  //tone(piezoPin, 261, 200);
  //delay(5000);
tone(piezoPin, 261, 200); //envío un tono al pin del altavoz con frecuencia 261 (do) y duración 200 milisegundos. 
   delay(300); //espero 300 milisegundos desde que empezó el comnado anterior (por eso la espera es de 100 hasta el siguiente tono)
  tone(piezoPin, 261, 200);
   delay(300);
  tone(piezoPin, 293, 400);
  delay(500);
  tone(piezoPin, 261, 300);
   delay(400);
  tone(piezoPin, 349, 300);
   delay(400);
  tone(piezoPin, 329, 400);
  delay(3000);


  //tone(piezoPin, 1000, 500);
  //delay(1000);

}
