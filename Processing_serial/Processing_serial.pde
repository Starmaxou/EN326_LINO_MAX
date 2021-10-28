import processing.serial.*;
import java.util.*;

Serial myPort;  // The serial port

String temp_text = "Curent temperature :";
String hygro_text = "Curent humidity :";
String press_text = "Curent Pression :";


float hygro = 0;
float temp = 0;
float lum = 0;
float pressure = 0.0;

String val;
int cpt = 0;

void setup() {
  // List all the available serial ports
  printArray(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[0], 9600);
  cpt = 0;
}

void draw() {
  String clean;
  delay(10);
  if (myPort.available() > 0) {
    
    val = myPort.readStringUntil('\n');
    
    if(val != null){
      clean = val.replaceAll("\\D+",""); //remove non-digits
      if (!clean.equals("")){
        switch(cpt){
          case 0:
            hygro = Float.parseFloat(clean);
            print("hygro = "); println(hygro);
            break;
          case 1:
            temp = Float.parseFloat(clean);
            print("temp = "); println(temp);
            break;
          case 2:
            lum = Float.parseFloat(clean);
            print("lum = "); println(lum);
            break;
          case 3:
            pressure = Float.parseFloat(clean);
            print("pressure = "); println(pressure);
            break;
        }
        cpt = (cpt < 3) ? cpt+1 : 0;
      }
    }
   
  }
}
