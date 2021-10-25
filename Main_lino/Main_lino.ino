#define pin_lum_in A0

#define lum_res 10000

uint16_t lum_data = 0;
uint16_t raw_lum_data = 0;
void setup() {
  Serial.begin(38400);
  Serial.println("Runing early Init...");
  pinMode(pin_lum_in, INPUT);
  
  Serial.println("INIT done, Moving to main loop");
}

void loop() {
  raw_lum_data = analogRead(A0);
  lum_data = 0.2*map(raw_lum_data,0 ,1024 ,0 ,5000 );
  
  Serial.print("Curent luminosity :");
  Serial.println(lum_data);
}
