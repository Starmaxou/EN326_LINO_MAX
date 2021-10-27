import processing.serial.*;
import java.util.*;

Serial myPort;  // The serial port

String temp_text = "Curent temperature :";
String hygro_text = "Curent humidity :";
String press_text = "Curent Pression :";



float temp = 0;
float hygro = 0;
float pressure = 0.0;

String val;
int cpt;

void setup() {
  // List all the available serial ports
  printArray(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[0], 9600);
  cpt = 0;
}

void draw() {
  String result;
  if (myPort.available() > 0) {
    
    val = myPort.readStringUntil('\n');
    
    if(val != null){
      Scanner scanner = new Scanner(val);
      if(scanner.hasNextInt()){
        temp = scanner.nextInt();
        print("temp = "); println(temp);
      }
      scanner.close();
    }
   
  }
}
