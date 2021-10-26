#define pin_lum_in A0
#define pin_humTemp_in 5

#define DHT_SUCCESS 0
#define DHT_TIMEOUT 1
#define DHT_CHECKSUM 2



void setup() 
{
  Serial.begin(38400);
  Serial.println("Runing early Init...");
  pinMode(pin_lum_in, INPUT);
  pinMode(pin_humTemp_in, INPUT_PULLUP);
  
  Serial.println("INIT done, Moving to main loop");
}

void loop() 
{
  uint8_t temp;
  uint8_t humid;
  uint16_t lum_data = readLum();
  uint8_t error = readTemp(&temp, &humid);
  if(error != DHT_SUCCESS)
  {
    Serial.print("Error using DHT11");
    Serial.println(error);
  }
  else
  {
    Serial.print("Curent humidity :");
    Serial.println(humid);
    Serial.print("Curent temperature :");
    Serial.println(temp);
  }
  Serial.print("Curent luminosity :");
  Serial.println(lum_data);
  delay(1000);
}

uint16_t readLum( void )
{
  uint16_t raw_lum_data = analogRead(A0);
  return (0.2*map(raw_lum_data,0 ,1024 ,0 ,5000 ));
}

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
