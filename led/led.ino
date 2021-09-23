int ledAprobado = 2;
int letRechazado = 4;
int tiempoEspera = 1000;
void ledSetup(){
  pinMode(ledAprobado, OUTPUT);
  pinMode(letRechazado, OUTPUT);
}
void ledAprobar(){
  digitalWrite(ledAprobado, HIGH);  
  delay(tiempoEspera);  
  digitalWrite(ledAprobado, LOW);  
  delay(tiempoEspera);  
}
void ledRechazar(){
  digitalWrite(letRechazado, HIGH);  
  delay(tiempoEspera);  
  digitalWrite(letRechazado, LOW);  
  delay(tiempoEspera); 
}
void setup() {
  ledSetup();
}

void loop() {
  ledAprobar();
  ledRechazar();
}
