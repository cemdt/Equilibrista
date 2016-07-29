//Este codigo es una variable del original solo para buscar un valor apto de referencia.

#include <Wire.h>

//Direccion I2C de la IMU
#define MPU 0x68

//MPU-6050 da los valores en enteros de 16 bits
//Valores sin refinar
int16_t AcX;

int TiempoMuestreo=1;        // tiempo de muestreo Se encuentra en milisegundos
unsigned long pasado=0;      // tiempo pasado (Se hace para asegurar tiempo de muestreo)
unsigned long ahora;
float Ref=0.995;              // referencia 
double Y;                    // Salida
double error;                // error
double errorPass=0;          // Suma del error para la parte integral
double errorAnt=0;           // error anterior para la derivada
double U;                    // Señal control

// constantes del controlador 
int Kp=1300;
int Ki=8;
int Kd=300;

int llantaI=10;              // llanta Izquierda
int llantaD=11;              // llanta derecha
int a=5;                     // variable de prueba
 
void setup()
{
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);

}

void loop() {
  while(a<=5){
    //Leer los valores del Acelerometro de la IMU
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
  }
   Serial.println(U);


