#define pin_lum_in A0
#define pin_humTemp_in 5

#define DHT_SUCCESS 0
#define DHT_TIMEOUT_ERROR 1
#define DHT_CHECKSUM_ERROR 2

uint16_t lum_data = 0;
uint16_t raw_lum_data = 0;
float temp;
float humid;

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
  raw_lum_data = analogRead(A0);
  lum_data = 0.2*map(raw_lum_data,0 ,1024 ,0 ,5000 );
  int error = readDHT11(&temp, &humid);
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


uint8_t readDHT11(float* temp, float* humid) 
{ 
  uint8_t data[5];
  data[0] = data[1] = data[2] = data[3] = data[4] = 0;
  unsigned long start_time = 18;
  unsigned long timeout = 1000;
   /* Conversion du numéro de broche Arduino en ports / masque binaire "bas niveau" */
  uint8_t bit = digitalPinToBitMask(pin_humTemp_in);
  uint8_t port = digitalPinToPort(pin_humTemp_in);
  volatile uint8_t *ddr = portModeRegister(port);   // Registre MODE (INPUT / OUTPUT)
  volatile uint8_t *out = portOutputRegister(port); // Registre OUT (écriture)
  volatile uint8_t *in = portInputRegister(port);   // Registre IN (lecture)
  
  /* Conversion du temps de timeout en nombre de cycles processeur */
  unsigned long max_cycles = microsecondsToClockCycles(timeout);
   /* Evite les problèmes de pull-up */
  *out |= bit;  // PULLUP
  *ddr &= ~bit; // INPUT
  delay(100);   // Laisse le temps à la résistance de pullup de mettre la ligne de données à HIGH
 
  /* Réveil du capteur */
  *ddr |= bit;  // OUTPUT
  *out &= ~bit; // LOW
  delay(start_time); // Temps d'attente à LOW causant le réveil du capteur
  // N.B. Il est impossible d'utilise delayMicroseconds() ici car un délai
  // de plus de 16 millisecondes ne donne pas un timing assez précis.
  
  /* Portion de code critique - pas d'interruptions possibles */
  noInterrupts();
  
  /* Passage en écoute */
  *out |= bit;  // PULLUP
  delayMicroseconds(40);
  *ddr &= ~bit; // INPUT
 
  /* Attente de la réponse du capteur */
  timeout = 0;
  while(!(*in & bit)) { /* Attente d'un état LOW */
    if (++timeout == max_cycles) {
        interrupts();
        return DHT_TIMEOUT_ERROR;
      }
  }
    
  timeout = 0;
  while(*in & bit) { /* Attente d'un état HIGH */
    if (++timeout == max_cycles) {
        interrupts();
        return DHT_TIMEOUT_ERROR;
      }
  }

  /* Lecture des données du capteur (40 bits) */
  for (byte i = 0; i < 40; ++i) {
 
    /* Attente d'un état LOW */
    unsigned long cycles_low = 0;
    while(!(*in & bit)) {
      if (++cycles_low == max_cycles) {
        interrupts();
        return DHT_TIMEOUT_ERROR;
      }
    }

    /* Attente d'un état HIGH */
    unsigned long cycles_high = 0;
    while(*in & bit) {
      if (++cycles_high == max_cycles) {
        interrupts();
        return DHT_TIMEOUT_ERROR;
      }
    }
    
    /* Si le temps haut est supérieur au temps bas c'est un "1", sinon c'est un "0" */
    data[i / 8] <<= 1;
    if (cycles_high > cycles_low) {
      data[i / 8] |= 1;
    }
  }
  
  /* Fin de la portion de code critique */
  interrupts();
 
  /*
   * Format des données :
   * [1, 0] = humidité en %
   * [3, 2] = température en degrés Celsius
   * [4] = checksum (humidité + température)
   */
   
  /* Vérifie la checksum */
  byte checksum = (data[0] + data[1] + data[2] + data[3]) & 0xff;
  if (data[4] != checksum)
    return DHT_CHECKSUM_ERROR; /* Erreur de checksum */
  else
  {
    temp = data[0];
    humid = data[2];
    Serial.println("Coucou Connard");
    return DHT_SUCCESS; /* Pas d'erreur */
  }
}
