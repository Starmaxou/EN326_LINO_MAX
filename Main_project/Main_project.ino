#include <Wire.h>
extern "C" {
  
}
#include "pressure.h"
void setup()
{
  Wire.begin();
  Serial.begin(115200);
  Serial.println(">>>> Wire Start!");
  Serial.println(">>>> Serial Start!");

  Read_coeff_pression();
  Serial.println(">>>> Read_coeff_pression done!");

}

void loop()
{
  float Pression = 0;

  Pression = Read_pression();
  Serial.print(">> Pressions = "); Serial.print(Pression); Serial.println(" kPa");
  delay(1000);
}
