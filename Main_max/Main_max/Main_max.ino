#include <Wire.h>

#define I2C_ADDR          0x60

#define I2C_START_CONVERT 0x12
#define I2C_READ_PRESSURE_MSB 0x00
#define I2C_READ_PRESSURE_LSB 0x01
#define I2C_READ_TEMP_MSB 0x02
#define I2C_READ_TEMP_LSB 0x03
#define I2C_READ_COEFF  0x04

float a0, b1, b2, c12;

void setup()
{
  Serial.begin(9600);
  Serial.print("Session Start!");
  Wire.begin();
  Serial.print("Wire Init done!");
  Read_coeff_pression();
}

void loop()
{
  float Pression = 0;

  Pression = Read_pression();
  Serial.print("Pressions = ");
  Serial.println(Pression);
  delay(100);
}

/*******************************************************
 * Fonction de lecture et de calcul de la pression
 */
float Read_pression()
{
  uint16_t Temperature_ADC = 0;
  uint16_t Pressure_ADC = 0;
  float Pressure;

  Start_measure_pressure();

  /**
   * Lecture Pression
   */

  Wire.beginTransmission(I2C_ADDR);
  Wire.write(I2C_READ_PRESSURE_MSB);
  Wire.endTransmission();
  
  Wire.requestFrom(I2C_ADDR, 1);
  if (Wire.available()) {
    Pressure_ADC = Wire.read();
    Pressure_ADC = Pressure_ADC << 8;
  }

  Wire.beginTransmission(I2C_ADDR);
  Wire.write(I2C_READ_PRESSURE_LSB);
  Wire.endTransmission();
  
  Wire.requestFrom(I2C_ADDR, 1);
  if (Wire.available())
    Pressure_ADC |= Wire.read();

  /**
   * Lecture Temperature
   */

  Wire.beginTransmission(I2C_ADDR);
  Wire.write(I2C_READ_TEMP_MSB);
  Wire.endTransmission();
  
  Wire.requestFrom(I2C_ADDR, 1);
  if (Wire.available()) {
    Temperature_ADC = Wire.read();
    Temperature_ADC = Temperature_ADC << 8;
  }

  Wire.beginTransmission(I2C_ADDR);
  Wire.write(I2C_READ_TEMP_LSB);
  Wire.endTransmission();
  
  Wire.requestFrom(I2C_ADDR, 1);
  if (Wire.available())
    Temperature_ADC |= Wire.read();
    
  /**
   * Calcul de la pression compenser
   */
   Pressure = a0 + (b1 + c12 * Temperature_ADC) * Pressure_ADC + b2 * Temperature_ADC;
   return Pressure;
}

/**********************************************
 * Fonction de lecture des coefficents pour le capteur de pression
 */
void Read_coeff_pression()
{
  uint16_t _a0, _b1, _b2, _c12;
  /**
   * Lecture coefficient
   */
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(I2C_READ_COEFF);
  Wire.endTransmission();
  
  Wire.requestFrom(I2C_ADDR, 8);
  if (Wire.available()) {
    _a0 = (Wire.read() << 8) | Wire.read();
    _b1 = (Wire.read() << 8) | Wire.read();
    _b2 = (Wire.read() << 8) | Wire.read();
    _c12 = (Wire.read() << 8) | Wire.read();
  }

  Serial.print("///////////////////////////////");
  Serial.print("Coefficient brute : ");
  Serial.print("_a0 = "); Serial.println(_a0);
  Serial.print("_b1 = "); Serial.println(_b1);
  Serial.print("_b2 = "); Serial.println(_b2);
  Serial.print("_c12 = "); Serial.println(_c12);
  Serial.print("///////////////////////////////");

  a0 = (float)_a0 / 8;
  b1 = (float)_b1 / 8192;
  b2 = (float)_b2 / 16384;
  c12 = (float)_c12;
  c12 /= 4194304.0;

  Serial.print("Coefficient reel : ");
  Serial.print("a0 = "); Serial.println(a0);
  Serial.print("b1 = "); Serial.println(b1);
  Serial.print("b2 = "); Serial.println(b2);
  Serial.print("c12 = "); Serial.println(c12);
  Serial.print("///////////////////////////////");
}

/************************************************
 * Envoi de la commande I2C pour le démarage d'une lecture de pression et temperature
 */
void Start_measure_pressure()
{
  Serial.print("Start Measure pressure");
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(I2C_START_CONVERT);
  Wire.endTransmission();
  delay(50);
}
