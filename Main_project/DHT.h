#ifndef __DHT_H
#define __DHT_H

#define pin_humTemp_in 5

#define DHT_SUCCESS 0
#define DHT_TIMEOUT 1
#define DHT_CHECKSUM 2

uint8_t readTemp(uint8_t *temp, uint8_t *humid);

#endif