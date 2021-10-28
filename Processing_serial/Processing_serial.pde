import processing.serial.*;
import java.util.*;

Serial myPort;  // The serial port

String text_pattern[] = {"Humidity:", "Temperature:", "Luminosity:", "Pressions:"};

float hygro = 0;
float temp = 0;
float lum = 0;
float pressure = 0.0;

String val;

float[] data_read= {0,0,0,0,0};

void setup() {
  Init_serial();
}

void draw() {
  
  data_read = Read_data(data_read);
  if (data_read[4] == 1.0) {
    print("Value_hygro :");
    println(data_read[0]);

    print("Value_temp :");
    println(data_read[1]);

    print("Value_lum :");
    println(data_read[2]);

    print("Value_pres :");
    println(data_read[3]);
  }
}

void Init_serial() {
  // List all the available serial ports
  printArray(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[0], 9600);
}

float[] Read_data(float[] mydata) {
  float[] data = mydata;
  if (myPort.available() > 0) {

    val = myPort.readStringUntil('\n'); //Lecture du port série jusqu'à trouver un retour à la ligne

    if (val != null) {  //On test si une chaine a bien été récupérée
      String[] clean = val.split(" ");  //On divise la chaine en deux pour extraire le texte et la data
      if ( clean.length == 2) { //Test si le split a pu être fait
        String text = clean[0];
        String value = clean[1];

        if (text.equals(text_pattern[0])) { //On test si le text reçu correspond à un des patternes.
          data[0] = Float.parseFloat(value);  //Conversion du string en float
          data[4] = 1.0;
        }
        if (text.equals(text_pattern[1])) { //On test si le text reçu correspond à un des patternes.
          data[1] = Float.parseFloat(value);   //Conversion du string en float
          data[4] = 1.0;
        }
        if (text.equals(text_pattern[2])) { //On test si le text reçu correspond à un des patternes.
          data[2] = Float.parseFloat(value);    //Conversion du string en float
          data[4] = 1.0;
        }
        if (text.equals(text_pattern[3])) { //On test si le text reçu correspond à un des patternes.
          data[3] = Float.parseFloat(value); //Conversion du string en float
          data[4] = 1.0;
        }
      }
    }
  }
  return data;
}
