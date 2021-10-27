
import processing.serial.*;

Serial myPort;  // The serial port

void setup() {
  // List all the available serial ports
  printArray(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[2], 38400);
}

void draw() {
  while (myPort.available() > 0) {
    char inByte = char(myPort.read());
    print(inByte);
  }
}
