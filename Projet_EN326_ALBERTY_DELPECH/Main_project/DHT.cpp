#include <Arduino.h>
#include <stdio.h>
#include <stdint.h>
#include "DHT.h"

uint8_t readTemp(uint8_t *temp, uint8_t *humid) 
{ 
  uint8_t data[5];
  data[0] = data[1] = data[2] = data[3] = data[4] = 0;
  unsigned long start_t = 18;
  unsigned long timeout = 1000;
  uint8_t bit = digitalPinToBitMask(pin_humTemp_in);
  uint8_t port = digitalPinToPort(pin_humTemp_in);
  volatile uint8_t *ddr = portModeRegister(port);   // Registre MODE (INPUT / OUTPUT)
  volatile uint8_t *out = portOutputRegister(port); // Registre OUT (écriture)
  volatile uint8_t *in = portInputRegister(port);   // Registre IN (lecture)
  // calcul du nombre de cycles en fonction du temps de timeout
  unsigned long max_cycles = microsecondsToClockCycles(timeout);

  *out |= bit;  // PULLUP
  *ddr &= ~bit; // INPUT
  delay(100);   //
  // Activation du capteur
  *ddr |= bit;  // OUTPUT
  *out &= ~bit; // LOW
  delay(start_t); 
  // Desactivation des intéruptions
  noInterrupts();
  // Attente de la réponse du capteur
  *out |= bit;  // PULLUP
  delayMicroseconds(40);
  *ddr &= ~bit; // INPUT
 
  timeout = 0;
  while(!(*in & bit)) { // Attente d'un etat bas
    if (++timeout == max_cycles) {
        interrupts();
        return DHT_TIMEOUT;
      }
  }
    
  timeout = 0;
  while(*in & bit) { // Attente d'un etat haut
    if (++timeout == max_cycles) {
        interrupts();
        return DHT_TIMEOUT;
      }
  }

  // Lecture de 40 bits de données
  for (byte i = 0; i < 40; ++i) {
 
    unsigned long cycles_low = 0;
    while(!(*in & bit)) {
      if (++cycles_low == max_cycles) {
        interrupts();
        return DHT_TIMEOUT;
      }
    }
    unsigned long cycles_high = 0;
    while(*in & bit) {
      if (++cycles_high == max_cycles) {
        interrupts();
        return DHT_TIMEOUT;
      }
    }
    data[i / 8] <<= 1;
    if (cycles_high > cycles_low) {
      data[i / 8] |= 1;
    }
  }
  
// réactivation des intéeuptions
  interrupts();
   
// Vérification du CRC
  byte checksum = (data[0] + data[1] + data[2] + data[3]) & 0xff;
  if (data[4] != checksum)
    return DHT_CHECKSUM;
  else
  {
    *temp = data[2];
    *humid = data[0];
    return DHT_SUCCESS;
  }
}
