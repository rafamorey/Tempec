#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>

Adafruit_SSD1306 display(-1);
// Declaracion de funciones
void ventiladorAccion();
void subeLimite();
void bajaLimite();

// se define pin para usar con one wire
#define ONE_WIRE_BUS 6
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensor (&oneWire);

// declaracion de variables
float temperature;
int btnUp = 3;
int btnDown = 2;
int ventilador = 5;
bool ventiladorActivado = false;
int limite = 29;
long contador1 = 0;

//variables almacenadoras de milisegundos
long tiempoAnteriorUp;
long tiempoAnteriorDown;
long diferenciaUp;
long diferenciaDown;

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando sensor temperatura");
  pinMode (ventilador, OUTPUT);

// interrupcion para subir limite
  pinMode(btnUp, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btnUp), subeLimite, RISING);
  pinMode(btnDown, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btnDown), bajaLimite, RISING);
}
void loop() {
  //forma correcta de ejecutar una funcion cada cierto tiempo
if(millis() - contador1 > 600){
  contador1 = millis();
  sensor.requestTemperatures();
  temperature = sensor.getTempCByIndex(0);
  
  ventiladorAccion();

  }

}

//funcion para encender apagar un ventilador
void ventiladorAccion(){
  if(temperature > limite){
  digitalWrite(ventilador, HIGH);
  ventiladorActivado = true;
  Serial.println("ventilador Activado" + String (temperature)); 
  } else {
  digitalWrite(ventilador, LOW);
  ventiladorActivado = false;
  Serial.println("Ventilador desactivado" +String(temperature));
  }
}

//funcion para subir setpoint
void subeLimite(){
  diferenciaUp = millis() - tiempoAnteriorUp;
  tiempoAnteriorUp = millis(); 
  if (diferenciaUp > 250){
  limite ++;
  Serial.println("Limite: " + String(limite));
  }
}

void bajaLimite(){
  diferenciaDown = millis() - tiempoAnteriorDown;
  tiempoAnteriorDown = millis(); 
  if (diferenciaDown > 250){
  limite --;
  Serial.println("Limite: " + String(limite));
  }
}