import java.util.*;
import processing.serial.*;

Serial myPort;  // The serial port
boolean Serial_flag = false;
int display_time = 60; // seconds
int refresh_rate = 2; // Hz
int x_offset = 200;
int y_offset = 80;
String text_pattern[] = {"Humidity:", "Temperature:", "Luminosity:", "Pressions:"};
float[] input;
float[] max;
float[] min;
float[][] y_scale;
float[][] values;
int nb_points = 0;
int max_points = display_time*refresh_rate;

void setup()
{
  Init_serial();

  size(displayWidth, displayHeight);
  textSize(24);

  input = new float[8];
  max = new float[4];
  min = new float[4];
  y_scale = new float[4][7];
  values = new float[4][max_points];
  for (int i = 0; i < 7; i++)
    input[i] = 1;
  while ((input[4]!= 1.0) && (input[5]!= 1.0) && (input[6]!= 1.0) && (input[7]!= 1.0))
    input = Read_data(input);
  input[4] = input[5] = input[6] = input[7] = 0.0;

  input[0] = input[1] = input[2] = input[3] = input[4] = 1;

  for (int i = 0; i < 4; i++)
  {
    min[i] = max[i] = input[i];
  }
}

void draw()
{
  if (Serial_flag)
  {
    Serial_flag = false;

    background(100);

    for (int i = 0; i < 4; i++)
    {
      fill(255, 255, 255);
      stroke(255, 255, 255);
      strokeWeight(2);
      rect(x_offset, 250*i+y_offset, 1700, 235);
      rect(x_offset-180, y_offset+250*i, 180, 40);
      rect(x_offset-180, y_offset+170+250*i, 170, 65);
      stroke(180, 180, 180);
      line(x_offset+100, y_offset+205+250*i, x_offset+1640, y_offset+205+250*i);
      line(x_offset+100, y_offset+15+250*i, x_offset+100, y_offset+205+250*i);
      triangle(x_offset+1640, y_offset+200+250*i, x_offset+1640, y_offset+210+250*i, x_offset+1650, y_offset+205+250*i);
      triangle(x_offset+95, y_offset+15+250*i, x_offset+105, y_offset+15+250*i, x_offset+100, y_offset+5+250*i);
      fill(180, 180, 180);
      textAlign(CENTER);
      text("time (t)", x_offset+1660, y_offset+230+250*i);
      textAlign(LEFT);
      text("Max :", x_offset-170, y_offset+195+250*i);
      text("Min :", x_offset-170, y_offset+225+250*i);
      textAlign(CENTER);
      for (int j = 0; j < 16; j++)
      {
        strokeWeight(1);
        line(x_offset+100+100*j, y_offset+205+250*i, x_offset+100+100*j, y_offset+15+250*i);
        strokeWeight(2);
        text((j)*(display_time/15), x_offset+100+100*j, y_offset+225+250*i);
      }
      strokeWeight(1);
      for (int j = 0; j < 7; j++)
      {
        line(x_offset+100, y_offset+19+250*i+(190/6)*j, x_offset+1640, y_offset+19+250*i+(190/6)*j);
      }
      strokeWeight(2);
    }
    fill(0, 0, 0);
    textAlign(LEFT);
    text("Humidity (%)", x_offset-175, y_offset+30f);
    text("Temperature (°C)", x_offset-175, y_offset+30+250);
    text("Light (AU)", x_offset-175, y_offset+30+500);
    text("Pressure (kPa)", x_offset-175, y_offset+30+750);
    for (int i = 0; i < 4; i++)
    {
      min[i] = max[i] = values[i][0];
      for (int j = 0; j < 7; j++)
      {
        if (max[i] < values[i][j])
        {
          max[i] = values[i][j];
          for (int k =0; k < max_points; k++)
          {
            y_scale[i][k] = min[i]-1+(max[i]+1-(min[i]-1))/7*k;
          }
        } else if (min[i] > values[i][j])
        {
          min[i] = values[i][j];
          for (int k =0; k < max_points; k++)
          {
            y_scale[i][k] = min[i]-1+(max[i]+1-(min[i]-1))/7*k;
          }
        }
      }

      fill(0, 0, 0);
      textAlign(LEFT);
      text(max[i], x_offset-100, y_offset+195+250*i);
      text(min[i], x_offset-100, y_offset+225+250*i);
      textAlign(RIGHT);
      fill(180, 180, 180);
      for (int j = 0; j < 7; j++)
      {
        text(y_scale[i][j], x_offset+90, y_offset+19+250*i+190-190/6*j);
        text(y_scale[i][j], x_offset+90, y_offset+19+250*i+190-190/6*j);
      }
      strokeWeight(1);
      stroke(255, 0, 0);
      for (int j = 1; j < max_points; j++)
      {
        if (values[i][j] >= min[i] && values[i][j] <= max[i])
        {
          float y = 190-map(values[i][j], min[i]-1, max[i]+1, 0, 186);
          float y1 = 190-map(values[i][j-1], min[i]-1, max[i]+1, 0, 186);
          println("value :" + values[i][j]);
          println("max :" + max[i]);
          println("min :" + min[1]);
          line(x_offset+100+1500/float(max_points)*j, y_offset+15+y+250*i, x_offset+100+1500/float(max_points)*(j-1), y_offset+15+y1+250*i);
        }
      }

      for (int j = max_points-1; j > 0; j--)
      {
        values[i][j] = values[i][j-1];
      }
      values[i][0] = input[i];
    }

    nb_points++;
    //delay(10/refresh_rate);
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
  data[4] = 0;
  if (myPort.available() > 0) {

    String val = myPort.readStringUntil('\n'); //Lecture du port série jusqu'à trouver un retour à la ligne

    if (val != null) {  //On test si une chaine a bien été récupérée
      String[] clean = val.split(" ");  //On divise la chaine en deux pour extraire le texte et la data
      if ( clean.length == 2) { //Test si le split a pu être fait
        String text = clean[0];
        String value = clean[1];

        if (text.equals(text_pattern[0])) { //On test si le text reçu correspond à un des patternes.
          data[0] = Float.parseFloat(value);  //Conversion du string en float
          data[4] = data[4];
        }
        if (text.equals(text_pattern[1])) { //On test si le text reçu correspond à un des patternes.
          data[1] = Float.parseFloat(value);   //Conversion du string en float
          data[5] = 1.0;
        }
        if (text.equals(text_pattern[2])) { //On test si le text reçu correspond à un des patternes.
          data[2] = Float.parseFloat(value);    //Conversion du string en float
          data[6] = 1.0;
        }
        if (text.equals(text_pattern[3])) { //On test si le text reçu correspond à un des patternes.
          data[3] = Float.parseFloat(value); //Conversion du string en float
          data[7] = 1.0;
        }
      }
    }
  }
  return data;
}

void serialEvent(Serial p)
{
  input = Read_data(input);
  Serial_flag = true;
  println(Serial_flag);
}
