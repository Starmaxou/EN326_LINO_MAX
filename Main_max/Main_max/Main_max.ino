#include <Wire.h>
#define I2C_ADDR          0x60
#define I2C_WRITE_CMD     0xC0
#define I2C_START_CONVERT 0x12

#define I2C_READ_CMD      0xC1
#define I2C_READ_PRESSURE_MSB 0x00
#define I2C_READ_PRESSURE_LSB 0x01
#define I2C_READ_TEMP_MSB 0x02
#define I2C_READ_TEMP_LSB 0x03
#define I2C_READ_COEFF  0x04

float a0, b1, b2, c12;

void setup()
{
  Serial.begin(9600);
  Serial.print("Hello world!");
  Wire.begin();
  //Read_coeff_pression();
}

void loop()
{
  float Pression = 0;
  Start_measure_pressure();

  //Pression = Read_pression();
  Serial.print("Pressions = ");
  Serial.println(Pression);
}

void Start_measure_pressure()
{
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(I2C_WRITE_CMD);
  Wire.write(I2C_START_CONVERT);
  Wire.endTransmission();
  delay(50);
}

float Read_pression()
{
  uint16_t Temperature_ADC = 0;
  uint16_t Pressure_ADC = 0;
  float Pressure;

  /**
   * Lecture Pression
   */

  Wire.beginTransmission(I2C_ADDR);
  Wire.write(I2C_READ_PRESSURE_MSB);
  Wire.endTransmission();
  
  Wire.requestFrom(I2C_READ_CMD, 1);
  if (1 <= Wire.available()) {
    Pressure_ADC = Wire.read();
    Pressure_ADC = Pressure_ADC << 8;
  }

  Wire.beginTransmission(I2C_ADDR);
  Wire.write(I2C_READ_PRESSURE_LSB);
  Wire.endTransmission();
  
  Wire.requestFrom(I2C_READ_CMD, 1);
  if (1 <= Wire.available())
    Pressure_ADC |= Wire.read();

  /**
   * Lecture Temperature
   */

  Wire.beginTransmission(I2C_ADDR);
  Wire.write(I2C_READ_TEMP_MSB);
  Wire.endTransmission();
  
  Wire.requestFrom(I2C_ADDR, 1);
  if (1 <= Wire.available()) {
    Temperature_ADC = Wire.read();
    Temperature_ADC = Temperature_ADC << 8;
  }

  Wire.beginTransmission(I2C_READ_CMD);
  Wire.write(I2C_READ_TEMP_LSB);
  Wire.endTransmission();
  
  Wire.requestFrom(I2C_ADDR, 1);
  if (1 <= Wire.available())
    Temperature_ADC |= Wire.read();
    
  /**
   * Calcul de la pression compenser
   */
   Pressure = a0 + (b1 + c12 * Temperature_ADC) * Pressure_ADC + b2 * Temperature_ADC;
   return Pressure;
}

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
  if (8 <= Wire.available()) {
    _a0 = Wire.read();
    _a0 = _a0 << 8;
    _a0 |= Wire.read();
    
    _b1 = Wire.read();
    _b1 = _b1 << 8;
    _b1 |= Wire.read();
    
    _b2 = Wire.read();
    _b2 = _b2 << 8;
    _b2 |= Wire.read();

    _c12 = Wire.read();
    _c12 = _c12 << 8;
    _c12 |= Wire.read();
  }

  a0 = (float)_a0 / 8;
  b1 = (float)_b1 / 8192;
  b2 = (float)_b2 / 16384;
  c12 = (float)_c12;
  c12 /= 4194304.0;
}
