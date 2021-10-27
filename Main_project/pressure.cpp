#include <Arduino.h>
#include <Wire.h>
#include <stdio.h>
#include <stdint.h>
#include "pressure.h"

float a0, b1, b2, c12;

float CheckSigned(float value);
void Start_measure_pressure();

float CheckSigned(float value)
{
  if(value > 32767){ //On test si le bit de signe est à 1
    value = value - 65536; //Si oui, Complement à 2.
  }
  return value;
}

/************************************************
   Envoi de la commande I2C pour le démarage d'une lecture de pression et temperature
*/
void Start_measure_pressure()
{
#if TRACE_DEBUG
  Serial.println("// Start Measure pressure \\");
#endif
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(I2C_START_CONVERT);
  Wire.write(I2C_READ_PRESSURE_MSB);
  Wire.endTransmission();
  delay(300);
}

/*******************************************************
   Fonction de lecture et de calcul de la pression
*/
float Read_pression()
{
  uint16_t Temperature_ADC = 0;
  uint16_t Pressure_ADC = 0;
  float Pcomp, Pressure;

  Start_measure_pressure();

  /**
     Lecture Pression et temperature
  */
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(I2C_READ_PRESSURE_MSB);
  Wire.endTransmission();
  

  Wire.requestFrom(I2C_ADDR, 4);
  if (Wire.available()) {
    Pressure_ADC = (((uint16_t)Wire.read() << 2) | Wire.read());
    Temperature_ADC = (((uint16_t)Wire.read() << 2) | Wire.read());
  }
  
#if TRACE_DEBUG 
  Serial.println(" ADC Values : ");
  Serial.print("> Temp = "); Serial.println(Temperature_ADC);
  Serial.print("> Pressure = "); Serial.println(Pressure_ADC);
#endif

  /**
     Calcul de la pression compenser
  */
  Pcomp = a0 + (b1 + c12 * Temperature_ADC) * Pressure_ADC + b2 * Temperature_ADC;
  Pressure = (65.0F / 1023.0F) * Pcomp + 50.0F;
  return Pressure;
}

/**********************************************
   Fonction de lecture des coefficents pour le capteur de pression
*/
void Read_coeff_pression()
{
  uint16_t _a0, _b1, _b2, _c12;
  /**
     Lecture coefficient
  */
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(I2C_READ_COEFF);
  Wire.endTransmission();

  Wire.requestFrom(I2C_ADDR, 8);
  if (Wire.available()) {
    _a0 = (((uint16_t)Wire.read() << 8) | Wire.read());
    _b1 = (((uint16_t)Wire.read() << 8) | Wire.read());
    _b2 = (((uint16_t)Wire.read() << 8) | Wire.read());
    _c12 = (((uint16_t)Wire.read() << 8) | Wire.read()) >> 2;
  }

  Serial.println("///////////////////////////////");
  Serial.println("Coefficient brute : ");
  Serial.print("_a0 = "); Serial.println(_a0);
  Serial.print("_b1 = "); Serial.println(_b1);
  Serial.print("_b2 = "); Serial.println(_b2);
  Serial.print("_c12 = "); Serial.println(_c12);
  Serial.print("///////////////////////////////");
  
  a0 = CheckSigned((float)_a0);
  b1 = CheckSigned((float)_b1);
  b2 = CheckSigned((float)_b2);
  c12 = CheckSigned((float)_c12);
  
  a0 = a0 / 8;
  b1 = b1 / 8192;
  b2 = b2 / 16384;
  c12 = c12 / 4194304.0;

  Serial.println("Coefficient reel : ");
  Serial.print("a0 = "); Serial.println(a0,DEC);
  Serial.print("b1 = "); Serial.println(b1,DEC);
  Serial.print("b2 = "); Serial.println(b2,DEC);
  Serial.print("c12 = "); Serial.println(c12,DEC);
  Serial.print("///////////////////////////////");

}
