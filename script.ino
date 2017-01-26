#include <SoftwareSerial.h>

/*
	Este código usa secuencias de escape para ciertas cosas. La consola serial por defecto del arduino no las acepta, con
	lo que sólo se verán caracteres extraños. Es necesario utilizar otro programa, como por ejemplo putty, para que funcionen.
*/

#define PRINT(a) Serial.print(a);BTserial.print(a)		// macros para escribir en la pantalla y por bluetooth a la vez
#define PRINTLN(a) Serial.println(a);BTserial.println(a)

SoftwareSerial BTserial(10,11);// RX | TX
int carEsc=0;
char c=' ';
int command = 0;	// si command=1, hace una medición cada 500 ms
void setup()
{
  pinMode(2,INPUT_PULLUP);
  pinMode(4,OUTPUT);
  pinMode(5,INPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(A0,INPUT);
  digitalWrite(9,LOW);
  digitalWrite(8 ,HIGH);
  Serial.begin(9600);
  Serial.println("Arduino is ready");
  BTserial.begin(9600);
  command = 0;
}

void loop()
{

  while(0,1)
  {
    if(digitalRead(2)==LOW)
      break;
    if(BTserial.available())
    {
      c = BTserial.read();
      if(c=='1')
        break;
      if(c=='2') 
      {
        command=1;
        break;
      }
      if(c=='3')
      {
        command=0;
        break;
      }
      if(c=='5')	// secuencias de escape que borran la pantalla
      {
        Serial.write(0x1B);
        Serial.write('[');
        Serial.print("1;1H");
        Serial.write(0x1B);
        Serial.write('[');
        Serial.print("2J");
      }
      if(c=='8')
        carEsc=1;	// activa las secuencias de escape
      if(c=='9')
        carEsc=0;
    }
    if(Serial.available())
    {
      c = Serial.read();
      if(c=='1')
        break;
      if(c=='2') 
      {
        command=1;
        break;
      }
      if(c=='3')
      {
        command=0;
        break;
      }
      if(c=='5')
      {
        Serial.write(0x1B);
        Serial.write('[');
        Serial.print("1;1H");
        Serial.write(0x1B);
        Serial.write('[');
        Serial.print("2J");
      }
      if(c=='8')
        carEsc=1;
      if(c=='9')
        carEsc=0;
    }
    if(command)
      break;
  }
  while(digitalRead(2)==LOW);
  //tone(4,40000,1);
  aaa:;
  tone(4,40000);
  delayMicroseconds(20);
  noTone(4);
  long tiempo = micros();
  long t2,t3=tiempo;
  delayMicroseconds(50);
  while(!digitalRead(5) && micros()-tiempo<50000);  // esperamos hasta que llegue el eco,
  t3 = micros();									// o hasta 50 ms, para que no se quede atascado.
  long result = t3-tiempo;
  if(carEsc)    // magia
  {
    Serial.write(0x1B);	// secuencias de escape que borran la última línea escrita
    Serial.write('[');
    Serial.write('1');
    Serial.write('F');
    Serial.write(0x1B);
    Serial.write('[');
    Serial.write('2');
    Serial.write('K');
  }
  if(result>=50000) 
  {
  	PRINTLN("No se ha podido medir la distancia");
  }
  else
  {
    long cm = -1+result/58;		// el -1 es calibración
    PRINT("Distancia: ");
    PRINT(cm);
    if((result/29)&1)	// resolución de 0,5 cm
    {
    	PRINTLN(",5 cm");
    }
    else
    {
    	PRINTLN(",0 cm");
    }
  }
  if(command)
    delay(500);
}
