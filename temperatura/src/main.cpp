#include <Arduino.h>
// declaracion de variables
int led = 5;
int btn = 3;
void setup() {
  Serial.begin(9600);
// declaracion de variables como in-out
  pinMode(btn, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  //se declara el estado del boton leyendo el pin 3
  bool estadoPulsador = digitalRead(3);
// si el estado esta apagado
  if(estadoPulsador == false){
    // si el led esta encendido
   if(digitalRead(led) == true){
    //  apago el led
     digitalWrite(led,LOW);
  // le damos un delay para hacer antirebote
  delay(400);
   } else {
    //  si el led esta apagado enciendo el led
    digitalWrite(led, HIGH);
    delay(400);
  }

}
}