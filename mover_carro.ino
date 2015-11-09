#include <Wire.h>  //permite la comuicacio co el protocolo i2c
 
//Direccion I2C de la IMU
#define MPU 0x68
 
//Ratios de conversion
#define A_R 16384.0
#define G_R 131.0
 
//Conversion de radianes a grados 180/PI
#define RAD_A_DEG = 57.295779
 
//MPU-6050 da los valores en enteros de 16 bits
//Valores sin refinar
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
 
//Angulos
float Acc[2];
float Gy[2];
float Angle[2],ANG;  //ANG-variable auxiliar para coparar el angulo sin filtro

void setup()
{
Wire.begin();
Wire.beginTransmission(MPU);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);
Serial.begin(9600);
pinMode(4,OUTPUT);
pinMode(7,OUTPUT);
}

void loop()
{
   //Leer los valores del Acelerometro de la IMU
   Wire.beginTransmission(MPU);
   Wire.write(0x3B); //Pedir el registro 0x3B - corresponde al AcX
   Wire.endTransmission(false);
   Wire.requestFrom(MPU,6,true); //A partir del 0x3B, se piden 6 registros
   AcX=Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 registros
   AcY=Wire.read()<<8|Wire.read();
   AcZ=Wire.read()<<8|Wire.read();
 
    //Se calculan los angulos Y, X respectivamente.
   Acc[1] = atan(-1*(AcX/A_R)/sqrt(pow((AcY/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
   Acc[0] = atan((AcY/A_R)/sqrt(pow((AcX/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
 
   //Leer los valores del Giroscopio
   Wire.beginTransmission(MPU);
   Wire.write(0x43);
   Wire.endTransmission(false);
   Wire.requestFrom(MPU,4,true); //A diferencia del Acelerometro, solo se piden 4 registros
   GyX=Wire.read()<<8|Wire.read();
   GyY=Wire.read()<<8|Wire.read();
 
   //Calculo del angulo del Giroscopio
   Gy[0] = GyX/G_R;
   Gy[1] = GyY/G_R;
   ANG=Angle[0]; //angulo antes del filtro.
   //Aplicar el Filtro Complementario
   Angle[0] = 0.98 *(Angle[0]+Gy[0]*0.010) + 0.02*Acc[0];
   Angle[1] = 0.98 *(Angle[1]+Gy[1]*0.010) + 0.02*Acc[1];
 
   //Mostrar los valores por consola
   //Serial.print("Angle X: "); Serial.print(Angle[0]); Serial.print("-");
   //Serial.print("GyX: ");
   Serial.print(Angle[0]);Serial.print("\n");
   //Serial.print("Angle Y: "); 
   //Serial.print(Angle[1]);// Serial.print("\n------------\n");
   //Serial.print("aceleracion x: "); Serial.print(AcX); Serial.print("-");
   //Serial.print("aceleracion y: "); Serial.print(AcY); Serial.print("-");
   //Serial.print("GyY: ");
   //Serial.print(GyY);// Serial.print("\n");
   int condicion=1;
   int tiempo=10;
   if(Angle[0]>=condicion)
   {
     digitalWrite(4,HIGH);//
     digitalWrite(7,LOW);
     tiempo=tiempo*((Angle[0]));
     delay(tiempo);
   }
   if(Angle[0]<=-condicion)
   {
     digitalWrite(4,LOW);//
     digitalWrite(7,HIGH);
     tiempo=tiempo*((-Angle[0]));
     delay(tiempo);
   }
   if(Angle[0]>(-condicion)||(Angle[0]<condicion))
   {
     digitalWrite(4,HIGH);
     digitalWrite(7,HIGH);
     delay(10); 
   }
   //delay(10); //Nuestra dt sera, pues, 0.010, que es el intervalo de tiempo en cada medida
}
