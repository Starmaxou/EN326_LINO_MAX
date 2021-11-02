#include <Arduino.h>
#include <stdio.h>
#include <stdint.h>
#include "lum.h"

uint16_t readLum( void )
{
  uint16_t raw_lum_data = analogRead(A0);
  return (0.2*map(raw_lum_data,0 ,1024 ,0 ,5000 ));
}