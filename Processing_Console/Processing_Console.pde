
import processing.serial.*;
int temp,humid,lumi,press;
Serial myPort;  // The serial port
int display_time = 60; // seconds
int refresh_rate = 1; // Hz
int x_offset = 200;
int y_offset = 80;
boolean data_valid = true; // if false programm will exit froam draw()

void setup() 
{
 //init_serial();

  size(displayWidth, displayHeight);
  textSize(24);
 
}

void draw() 
{
  float[] input;
  float[] max;
  float[] min;
  float[][] y_scale;
  input = new float[4];
  max = new float[4];
  min = new float[4]; 
  y_scale = new float[4][7];
  
  input[0] = input[1] = input[2] = input[3] = 0;
  min[0] = min[1] = min[2] = min[3] = 0;
  max[0] = max[1] = max[2] = max[3] = 0.7;
  
  background(0);
  
  for(int i = 0; i < 4; i++)
  {
    fill(255,255,255);
    stroke(0,0,255);
    strokeWeight(2);
    rect(x_offset,250*i+y_offset,1700,235);
    rect(x_offset-180,y_offset+250*i,180,40);
    rect(x_offset-180,y_offset+170+250*i,170,65);
    stroke(180,180,180);
    line(x_offset+100,y_offset+205+250*i,x_offset+1640,y_offset+205+250*i);
    line(x_offset+100,y_offset+15+250*i,x_offset+100,y_offset+205+250*i);
    triangle(x_offset+1640, y_offset+200+250*i, x_offset+1640, y_offset+210+250*i, x_offset+1650, y_offset+205+250*i);
    triangle(x_offset+95,y_offset+15+250*i,x_offset+105,y_offset+15+250*i,x_offset+100,y_offset+5+250*i);
    fill(180,180,180);
    textAlign(CENTER);
    text("time (t)",x_offset+1660,y_offset+230+250*i);
    textAlign(LEFT);
    text("Max :",x_offset-170,y_offset+195+250*i);
    text("Min :",x_offset-170,y_offset+225+250*i);
    textAlign(CENTER);
    for(int j = 0; j < 16; j++)
    {
      line(x_offset+100+100*j,y_offset+205+250*i,x_offset+100+100*j,y_offset+200+250*i);
      text((j)*(display_time/15),x_offset+100+100*j,y_offset+225+250*i);
    }
    for(int j = 0; j < 7; j++)
    {
      line(x_offset+100, y_offset+19+250*i+(190/6)*j, x_offset+105, y_offset+19+250*i+(190/6)*j);
    }
  }
  textAlign(LEFT);
  text("Light (AU)",x_offset-175,y_offset+30f);
  text("Temperature (°C)",x_offset-175,y_offset+30+250);
  text("Pressure (kPa)",x_offset-175,y_offset+30+500);
  text("Humidity (%)",x_offset-175,y_offset+30+750);
  
  int end = 0;
  while(data_valid)
  {
    end++;
    if(end > 60)
    {
      data_valid = false;
    }
    for(int i = 0; i < 4; i++)
    {
      if(max[i] < input[i])
      {
        max[i] = input[i];
        for(int j = 0; j < 7; j++)
        {
          y_scale[i][j] = min[i]+(min[i]-max[i])/7*j;
        }
      }
      else if(min[i] > input[i])
      {
        min[i] = input[i];
        for(int j = 0; j < 7; j++)
        {
          y_scale[i][j] = min[i]+(min[i]-max[i])/7*j;
        }
      }
      
    }
    //delay(1000/refresh_rate);
  } 

}


void init_serial()
{
  printArray(Serial.list()); // Listing serial ports
  myPort = new Serial(this, Serial.list()[2], 38400); //Open serial
}
