#include <Wire.h>
#include "pressure.h"
#include "DHT.h"
#include "lum.h"

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Serial.println(">>>> Wire Start!");
  Serial.println(">>>> Serial Start!");

  pinMode(pin_lum_in, INPUT);
  pinMode(pin_humTemp_in, INPUT_PULLUP);
  Serial.println(">>>> pinMode init done!");

  Read_coeff_pression();
  Serial.println(">>>> Read_coeff_pression done!");
}

void loop()
{
  float Pression = 0.0;
  uint8_t temp;
  uint8_t humid;

  Read_pression(&Pression);
  uint8_t error = readTemp(&temp, &humid);
  uint16_t lum_data = readLum();

  /**
   * Affichage des mesures
   */
  if(error != DHT_SUCCESS)
  {
    Serial.print("Error using DHT11");
    Serial.println(error);
  }
  else
  {
    Serial.print("Curent humidity :");
    Serial.print(humid); 
    Serial.println(" "); //necessaire pour exctration des données sur Processing
    
    Serial.print("Curent temperature :");
    Serial.print(temp);
    Serial.println(" "); //necessaire pour exctration des données sur Processing
  }
  Serial.print("Curent luminosity :");
  Serial.print(lum_data);
  Serial.println(" "); //necessaire pour exctration des données sur Processing

  Serial.print("Curent pressions :"); 
  Serial.print(Pression);
  Serial.println(" "); //necessaire pour exctration des données sur Processing
  delay(1000);
}
