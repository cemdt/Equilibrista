// programa PID desde cero 

#include <Wire.h>

//Direccion I2C de la IMU
#define MPU 0x68

//MPU-6050 da los valores en enteros de 16 bits
//Valores sin refinar
int16_t AcX;

int TiempoMuestreo=1;        // tiempo de muestreo Se encuentra en milisegundos
unsigned long pasado=0;      // tiempo pasado (Se hace para asegurar tiempo de muestreo)
unsigned long ahora;
float Ref=-0.2;              // referencia 
double Y;                    // Salida
double error;                // error
double errorPass=0;          // Suma del error para la parte integral
double errorAnt=0;           // error anterior para la derivada
double U;                    // Señal control

// constantes del controlador 
int Kp=1300;
int Ki=8;
int Kd=300;

int llantaS=3;              // ruedas


void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);//Establece una velocidad de conexion de 9600 baudios, velocidad standart del bluetooth
  pinMode(5,OUTPUT);//Las salidas son de los 4 datos recividos de la apk, los cuales van a un L293
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);

}

void loop() {
     Wire.beginTransmission(MPU);
   Wire.write(0x3B);                          //Pedir el registro 0x3B - corresponde al AcX
   Wire.endTransmission(false);
   Wire.requestFrom(MPU,6,true);             //A partir del 0x3B, se piden 6 registros
   AcX=Wire.read()<<8|Wire.read();           //Cada valor ocupa 2 registros
  
   Y=float(AcX)/17000;                       // medición de la planta
 
  ahora=millis();                            // tiempo actual 
   
  int CambioTiempo= ahora-pasado;                       // diferencia de tiempo actual- pasado
  if(CambioTiempo >= TiempoMuestreo)                    // si se supera el tiempo de muestreo
  {
      error = Ref - Y;                                 // error (Lazo cerrado)-- Feedback
      errorPass=error*TiempoMuestreo+errorPass;        // cálculo de aproximación del area
      double errorD =(error-errorAnt)/TiempoMuestreo;  // cálculo derivativo 
      float P=Kp*error;                                // control proporcional
      float I=Ki*errorPass;                            // control Integral
      float D=Kd*errorD;
      
      U=P+I+D;                       // Señal de control
      
      pasado=ahora;                // actualizar tiempo
      errorAnt=error;              // actualizar el error
     
  }  
  
   Serial.println(U);

  if (U < -255)     // límites de saturación de la señal de control
  {
      U=-255;
  }
  if (U > 255)    // límites de saturación de la señal de control
  {
      U=255;
  }
  
   int atras=0;
   int adelante=0;
  
   if (U>0){
     adelante=U;       // PWM de los motores atras y adelante (las dos ruedas funcionan al tiempo)
   }
   if (U<0){
     atras =abs(U);
   }
  if (adelante != 0){   // no se permite que se envie el comando hacia adelante y hacia atras al tiempo 
     if (atras == 0){  
     
       analogWrite(llantaS,adelante);
       digitalWrite(6,LOW);
       digitalWrite(5,HIGH);
     }
  }
 
  if (atras != 0){         // no se permite que se envie el comando hacia adelante y hacia atras al tiempo 
     if (adelante == 0){
       analogWrite(llantaS,atras);
       digitalWrite(5,LOW);
       digitalWrite(6,HIGH);
       
    }
  }
  int pwm=0,parada=255;//la variable pwm y parada son para que los motores comiencen su movimiento progresivamente, o se detenga de la misma forma
  char dato=Serial.read();//almaceno en "dato" la caracter que recivo desde la apk
  pwm=0;//simplemente asegurar que el dato vuelva siempre a 0
  while(dato=='w'){
    if(pwm<=255){//arranque progresivo
      pwm++;
      delay(6);
    }
    analogWrite(3,pwm);
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);
    digitalWrite(8,LOW);
    digitalWrite(5,HIGH);
    if(Serial.read()>0){//solamente modificamos la variable dato, cuando halla algo en el puerto serial (explicaion en el README)
    dato=Serial.read();
    }
       Serial.println(U);

  if (U < -255)     // límites de saturación de la señal de control
  {
      U=-255;
  }
  if (U > 255)    // límites de saturación de la señal de control
  {
      U=255;
  }
  
   int atras=0;
   int adelante=0;
  
   if (U>0){
     adelante=U;       // PWM de los motores atras y adelante (las dos ruedas funcionan al tiempo)
   }
   if (U<0){
     atras =abs(U);
   }
  if (adelante != 0){   // no se permite que se envie el comando hacia adelante y hacia atras al tiempo 
     if (atras == 0){  
     
       analogWrite(llantaS,adelante);
       digitalWrite(6,LOW);
       digitalWrite(5,HIGH);
     }
  }
 
  if (atras != 0){         // no se permite que se envie el comando hacia adelante y hacia atras al tiempo 
     if (adelante == 0){
       analogWrite(llantaS,atras);
       digitalWrite(5,LOW);
       digitalWrite(6,HIGH);
       
    }
  }
  }
  pwm=0;
  while(dato=='s'){
       if(pwm<=255){
      pwm++;
      delay(6);
    }
    analogWrite(3,pwm);
    digitalWrite(5,LOW);
    digitalWrite(7,LOW);
    digitalWrite(8,LOW);
    digitalWrite(6,HIGH);
    if(Serial.available()>0){
    dato=Serial.read();
  }
     Serial.println(U);

  if (U < -255)     // límites de saturación de la señal de control
  {
      U=-255;
  }
  if (U > 255)    // límites de saturación de la señal de control
  {
      U=255;
  }
  
   int atras=0;
   int adelante=0;
  
   if (U>0){
     adelante=U;       // PWM de los motores atras y adelante (las dos ruedas funcionan al tiempo)
   }
   if (U<0){
     atras =abs(U);
   }
  if (adelante != 0){   // no se permite que se envie el comando hacia adelante y hacia atras al tiempo 
     if (atras == 0){  
     
       analogWrite(llantaS,adelante);
       digitalWrite(6,LOW);
       digitalWrite(5,HIGH);
     }
  }
 
  if (atras != 0){         // no se permite que se envie el comando hacia adelante y hacia atras al tiempo 
     if (adelante == 0){
       analogWrite(llantaS,atras);
       digitalWrite(5,LOW);
       digitalWrite(6,HIGH);
       
    }
  }
  }
  pwm=0;
  while(dato=='a'){
    digitalWrite(6,LOW);
    digitalWrite(5,LOW);
    digitalWrite(8,LOW);
    digitalWrite(7,HIGH);
    if(Serial.available()>0){
    dato=Serial.read();
    }
       Serial.println(U);

  if (U < -255)     // límites de saturación de la señal de control
  {
      U=-255;
  }
  if (U > 255)    // límites de saturación de la señal de control
  {
      U=255;
  }
  
   int atras=0;
   int adelante=0;
  
   if (U>0){
     adelante=U;       // PWM de los motores atras y adelante (las dos ruedas funcionan al tiempo)
   }
   if (U<0){
     atras =abs(U);
   }
  if (adelante != 0){   // no se permite que se envie el comando hacia adelante y hacia atras al tiempo 
     if (atras == 0){  
     
       analogWrite(llantaS,adelante);
       digitalWrite(6,LOW);
       digitalWrite(5,HIGH);
     }
  }
 
  if (atras != 0){         // no se permite que se envie el comando hacia adelante y hacia atras al tiempo 
     if (adelante == 0){
       analogWrite(llantaS,atras);
       digitalWrite(5,LOW);
       digitalWrite(6,HIGH);
       
    }
  }
  }
  pwm=0;
  while(dato=='d'){
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);
    digitalWrite(5,LOW);
    digitalWrite(8,HIGH);
    if(Serial.available()>0){
    dato=Serial.read();
    }
       Serial.println(U);

  if (U < -255)     // límites de saturación de la señal de control
  {
      U=-255;
  }
  if (U > 255)    // límites de saturación de la señal de control
  {
      U=255;
  }
  
   int atras=0;
   int adelante=0;
  
   if (U>0){
     adelante=U;       // PWM de los motores atras y adelante (las dos ruedas funcionan al tiempo)
   }
   if (U<0){
     atras =abs(U);
   }
  if (adelante != 0){   // no se permite que se envie el comando hacia adelante y hacia atras al tiempo 
     if (atras == 0){  
     
       analogWrite(llantaS,adelante);
       digitalWrite(6,LOW);
       digitalWrite(5,HIGH);
     }
  }
 
  if (atras != 0){         // no se permite que se envie el comando hacia adelante y hacia atras al tiempo 
     if (adelante == 0){
       analogWrite(llantaS,atras);
       digitalWrite(5,LOW);
       digitalWrite(6,HIGH);
       
    }
  }
  }
  pwm=0;
  while(dato=='c'){//El caracter 'c' en mi caso es el caracter que ordena la parada de los motores, y como podran ver en este caso tiene un while y no un if (explicacion en el README)
    while(parada>0){
      parada--;
      delay(9);
      analogWrite(3,parada);
    }
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);
    digitalWrite(5,LOW);
    digitalWrite(8,LOW);
    if(Serial.available()>0){
    dato=Serial.read();
    }
       Serial.println(U);

  if (U < -255)     // límites de saturación de la señal de control
  {
      U=-255;
  }
  if (U > 255)    // límites de saturación de la señal de control
  {
      U=255;
  }
  
   int atras=0;
   int adelante=0;
  
   if (U>0){
     adelante=U;       // PWM de los motores atras y adelante (las dos ruedas funcionan al tiempo)
   }
   if (U<0){
     atras =abs(U);
   }
  if (adelante != 0){   // no se permite que se envie el comando hacia adelante y hacia atras al tiempo 
     if (atras == 0){  
     
       analogWrite(llantaS,adelante);
       digitalWrite(6,LOW);
       digitalWrite(5,HIGH);
     }
  }
 
  if (atras != 0){         // no se permite que se envie el comando hacia adelante y hacia atras al tiempo 
     if (adelante == 0){
       analogWrite(llantaS,atras);
       digitalWrite(5,LOW);
       digitalWrite(6,HIGH);
       
    }
  }
  }
  parada=255;
  pwm=0;
} 
