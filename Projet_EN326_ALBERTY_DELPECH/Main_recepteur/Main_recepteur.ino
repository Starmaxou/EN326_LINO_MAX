void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Session begin");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
    Serial.print((char)Serial.read());
}
