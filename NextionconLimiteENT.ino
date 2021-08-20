#include <OneWire.h>
#include <DallasTemperature.h>
#define  oneWirePin 5
OneWire oneWireBus(oneWirePin);
DallasTemperature sensor(&oneWireBus);

void accionVentilador();
void impirmirLimite();
void impirmirTemp();
void cambioLimite();


int ventilador = 4; 
int temperaturaLimite;//a set point



void setup() {
  
   Serial.begin(9600);
   sensor.begin(); 
   pinMode(ventilador, OUTPUT); //pin ventilador como salida
}
void loop() {
  
   cambioLimite();
   impirmirLimite();
   accionventilador();
   impirmirTemp();
}
void accionventilador()
{
  
   float temp = sensor.getTempCByIndex(0);
   if (temp > temperaturaLimite){
    digitalWrite(ventilador, HIGH); 
}
   else {
    digitalWrite(ventilador, LOW); 
  }
}
void impirmirLimite(){
   Serial.print("limite.val=");
   Serial.print(temperaturaLimite);
   Serial.write(0xff);
   Serial.write(0xff);
   Serial.write(0xff);
}
void impirmirTemp(){
   sensor.requestTemperatures();
   float temp = sensor.getTempCByIndex(0);
   int Value = temp * 10;
   Serial.print("volt.val=");
   Serial.print(Value);
   Serial.write(0xff);
   Serial.write(0xff);
   Serial.write(0xff);
}
void cambioLimite()
{
  // hacer con case
 
    if (Serial.available()>0) {
    String Received = Serial.readString();
    if(int(Received[0]==1))
    {
      temperaturaLimite++;
    }
    else if ((Received[0]==0))
    {
      temperaturaLimite--;
      
    } 
    else {
     
     temperaturaLimite = int(Received[0]);
      
    }
  } 
}
