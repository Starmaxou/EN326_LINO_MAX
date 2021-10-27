#ifndef __PRESSURE_H
#define __PRESSURE_H

#define I2C_ADDR          0x60

#define I2C_START_CONVERT 0x12
#define I2C_READ_PRESSURE_MSB 0x00
#define I2C_READ_PRESSURE_LSB 0x01
#define I2C_READ_TEMP_MSB 0x02
#define I2C_READ_TEMP_LSB 0x03
#define I2C_READ_COEFF  0x04

void Read_coeff_pression();
float Read_pression();


#endif
