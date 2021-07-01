#include <Arduino.h>
// declaracion de variables
int led = 5;
int btn = 3;
bool estadoPulsador;
//se debe de declarar todas las funciones de forma global
void consulta_estado();

void setup() {
  Serial.begin(9600);
// declaracion de variables como in-out
  pinMode(btn, INPUT_PULLUP);
  pinMode(led, OUTPUT);


  // attachInterrupt es una funcion para activar interrupciones, podria recibir solo el nombre del pin, pero arduino recomienda que sea pasado con otra funcion, digitalPinToInterrupt, el cual recibe como parametro el pin, asi mismo atachInterrupt recibe 3 parametros, el pin, la funcion que se activara con ese pin, esta se escribe sin parentesis, y por ultimo cuando debe activarse la interrupcion, existen diferentes estados, RISING, FALLING, LOW, etc 
  attachInterrupt(digitalPinToInterrupt(btn), consulta_estado, CHANGE);
}

void loop() {
  
  
  
}

void consulta_estado(){
 
    // si el led esta encendido
   if(digitalRead(led) == true){
    //  apago el led
     digitalWrite(led,LOW);
     Serial.println("Apagado");
  // le damos un delay para hacer antirebote
     delay(400);
   } else {
    //  si el led esta apagado enciendo el led
    digitalWrite(led, HIGH);
    Serial.println("Encendido");
    delay(400);
      }   
    }
  
