/*Este es un pequeño projecto, el cual esta en curso por los alumnos Cesar Cardinali y Nahuells Kliphan del Instituto Privado de Educacion Tecnica "JUAN XXIII"
  Consiste en un "robot equilibrista" un robot de solo 2 ruedas capaz de mantener el equilibrio. Ademas de poder ser controlado via Bluetooth, atraves de un HC
  06.
  Este codigo es solo de la parte de comunicacion Bluetooth de la APK del celular con la plaqueta arduino
 */



void setup() {
  Serial.begin(9600);//Establece una velocidad de conexion de 9600 baudios, velocidad standart del bluetooth
  pinMode(5,OUTPUT);//Las salidas son de los 4 datos recividos de la apk, los cuales van a un L293
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);

}

void loop() {
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
    delay(100);
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
  delay(100);
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
    delay(100);
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
    delay(100);
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
    delay(100);
  }
  parada=255;
  pwm=0;
} 
