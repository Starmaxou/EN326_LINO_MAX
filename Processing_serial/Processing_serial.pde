import processing.serial.*;

Serial myPort;  // The serial port

String temp_text = "Curent temperature :";
String hygro_text = "Curent humidity :";
String press_text = "Curent Pression :";

int temp = 0;
int hygro = 0;
float pressure = 0.0;

void setup() {
  // List all the available serial ports
  printArray(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[0], 9600);
}

void draw() {
  while (myPort.available() > 0) {
    char inByte = (char)myPort.read();
    //print(inByte);
    print("temp = "); println(temp);
    print("hygro = "); println(hygro);
    print("pressure = "); println(pressure);
  }
}
