#include <Arduino.h>

int led = 5;
int btn = 3;
void setup() {
  Serial.begin(9600);

  pinMode(btn, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  bool estadoPulsador = digitalRead(3);
  String estado;

  if(estadoPulsador == true){
   estado = "Apagado";
   digitalWrite(5,LOW); 
  } else {
    estado = "Encendido";
    digitalWrite(5,HIGH);
  }
  Serial.println(estado);
  delay(250);
}