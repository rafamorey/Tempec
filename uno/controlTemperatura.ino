
//Librerias sd1306
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

//libreria sensor temperatura
#include <OneWire.h>
#include <DallasTemperature.h>

//Muestra el display
Adafruit_SSD1306 display(-1);

//info tecnica de sensor de temperatura
#define ONE_WIRE_BUS 7 //pin en donde se conecta el sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensor(&oneWire);

//se crea array para almacenar los ultimos 128 datos recolectados por el sensor
float bufferTemps[128];
//Definicion de pines
//byte led = 5;
byte botonUp = 3;
byte botonDown = 2;
byte ventilador = 5;

//variables globales
float temperatura;
bool ventiladorActivado = false;
int temperaturaLimite = 20;
bool cambioLimite = false;//variable utilizada como flag

//variables utilizadas en las funciones de interrupcion
long contador1 = 0;
long tiempoAnteriorUp;
long diferenciaUp;
long tiempoAnteriorDown;
long diferenciaDown;


void setup() {
  //se crea bucle para inicializar en 110 todos los valores del array
  for(int i=0;i<128;i++){
    bufferTemps[i]=110;
  }
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);//se inicializa display
  display.clearDisplay();//limpia display
  Serial.begin(9600);//se inicializa serial
  Serial.println("Iniciando sistema control temperatura");
  pinMode(ventilador, OUTPUT); //pin ventilador como salida
  pinMode(botonUp, INPUT_PULLUP);//boton arriba con resistencia interna
  pinMode(botonDown, INPUT_PULLUP);//boton abajo entrada cn resistencia interna
  //funcion para interrupcion, manda llamar la funcion cada vez que se obrime boton
  attachInterrupt(digitalPinToInterrupt(botonDown), bajaLimite, FALLING);
  attachInterrupt(digitalPinToInterrupt(botonUp), subeLimite, RISING);

  dibujaLimite(temperaturaLimite);//funcion para mostrar el setpoint de temperatura
  
}

void loop() {

  if (millis() - contador1 > 1000) { //estas dos lineas me permiten ejecutar una
    //funcion cada segundo, haciendolo de esta manera no utilizamos delays, y
    // le permitimos al codigo no deterse y garantizar que la funcion se ejecutara
    // cada cierto tiempo
    contador1 = millis();
    sensor.requestTemperatures();//leemos valor en sensor de temperatura
    temperatura = sensor.getTempCByIndex(0);//indicamos la direccion del sensor
    accionVentilador();//funcion para encender ventilador
    llenaBuffer(temperatura);//se manda llamar funcion para almacenar datos de sensor
    dibujaTemperatura(temperatura);//funcion para mostrar el valor de temperatura
    dibujaCuadricula();//funcion para dibujar cuadricula de grafica
    dibujaCurva();
    setPoint();
  }
  
  //condicional para validar si se ha oprimido el boton para subir o bajar el limite
  if(cambioLimite){
    dibujaLimite(temperaturaLimite);//si el botn es oprimido, se muestra el nuevo valor de limite
    cambioLimite = false;//regresa el estado del boton a false, asi no se activa nuevamente
    //funcion hasta que es oprimido el boton nuevamente
    
  }

}

void accionVentilador() {
  if (temperatura > temperaturaLimite) {
    digitalWrite(ventilador, HIGH);
    ventiladorActivado = true;
    Serial.println("Ventilador Activado");
  } else {
    digitalWrite(ventilador, LOW);
    ventiladorActivado = false;
    Serial.println("Ventilador Desactivado");
  }
}

//funcion de interrupcion, se compara el tiempo transcurrido y se resta el anterior
//si este es menor que 300 no activa el boton para evitar rebotes. si es mayor
//aumenta en 1 el valor de limite
void subeLimite() {
  diferenciaUp = millis() - tiempoAnteriorUp;
  tiempoAnteriorUp = millis();
  if (diferenciaUp > 300) {
    temperaturaLimite++;
    //Serial.println("Limite -> " + String(temperaturaLimite));
    cambioLimite = true;
  }

}

void bajaLimite() {
  diferenciaDown = millis() - tiempoAnteriorDown;
  tiempoAnteriorDown = millis();
  if (diferenciaDown > 300) {
    temperaturaLimite--;
    cambioLimite = true;
    //Serial.println("Limite -> " + String(temperaturaLimite));//en una interrupcion
    //solo debo modificar variables, no usar delay, ni imprimir nada.
  }

}

//funcion para escribir el valor de temperatura
void dibujaTemperatura(float temp) {
  display.setTextColor(WHITE,BLACK);//se configura color de letra y fondo
  display.setTextSize(1);//se configura tamaño de letra
  display.setCursor(0, 0);//se configura ubicacion de cursor
  display.println("T = " + String(temp, 2));//se muetsra un texto y el valor de 
  //temperatura con 2 decimales
  display.display();//se escribe con toda la confuguracion anterior
}

//funcion para dibujar el nuevo valor limite de temperatura, una vez que es
//oprimido algun boton (arriba/abajo)
void dibujaLimite(float temp) {
  display.setTextColor(WHITE,BLACK);
  display.setTextSize(1);
  display.setCursor(64, 0);//se ubica cursor a la mitad del display
  display.println("T = " + String(temp,0));
  display.display();
}

//funcion para dibujar cuadricula debajo de los valores de temperatura
void dibujaCuadricula(){
  display.fillRect(0,8,128,23,BLACK);//se crea un rectangulo negro para borrar y
                                    //renovar los datos y que de esta manera no
                                    //haya datos nuevos encima de los anteriores
  for(int b=8;b<=32;b= b+3){//se crea un for para repetir el codigo de abajo, iniciando
    //el valor de b en 13 e incrementandolo en 4 en cada iteracion, siempre y cuando
    //b sea menor al alto del display(32 pixeles, hay display de 64 pixeles)
    for(int i=0; i<128; i = i+10){//este for es el que va a dibujar, dibuja un punto
      display.drawPixel(i,b,WHITE);//en el eje x, separados cada 12 espacios, hasta
  }                               //alcanzar el ancho del display (128 pixeles)
  }
}

//funcion para graficar el valor de temperatura
void dibujaCurva(){
  for(int i=0; i<128;i++){//para el ancho del display
    if(bufferTemps[i]<109){//se creo una variable de tipo array, y arriba se igualaron
                          //todos los valores del eje x a 110, es por eso que si el valor
                          //de la posicion es menor a 109 entra la condicion
    int y = map(bufferTemps[i],10,40,30,8);//se crea un entero y se iguala a la 
                                          //funcion map(valor lectura,valor min deseado
                                          //valor max deseado, valor mas bajo del display
                                          //valor mas alto del display
    display.drawPixel(i,y,WHITE);//dibuja el valor de temperatura almacenado en el
                                //buffer con los valores de temperatura
    }
  }
}
//funcionar para almacenar las ultimas 128 lecturas recolectadas por el sensor
//de temperatura
void llenaBuffer(float temp){//se llena con la variable temp
  for(int i=1; i<128;i++){//se recogen todas las posiciones, en este caso 128
    bufferTemps[i-1] = bufferTemps[i];//la posicion anterior es igual a la nueva
                                      //posicion. es decir se va a almacenar un dato
                                      //en la pos 128, despues al obtener un nuevo dato
                                      //lo que tengo en 128 se pasa a 127 y la nueva
                                      //posicion se guarda en 128. asi cada que guarde
                                      //un nuevo dato
  }
  bufferTemps[127]=temp;//la ultima posicion es igual al ultima dato recolectado
  //por el sensor
}

//se crea funcion para ubicar una linea en grafica como referencia del setpoint
void setPoint(){
  int p = map(temperaturaLimite,10,40,30,8);//se crea variable y se iguala a funcion
                                            //map
  for(int i=0; i<128; i = i+2){
        display.drawPixel(i,p,WHITE); 
}
}
