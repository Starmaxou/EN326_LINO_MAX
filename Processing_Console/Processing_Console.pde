
import processing.serial.*;

Serial myPort;  // The serial port
int display_time = 60; // seconds
int x_offset = 200;
int y_offset = 80;
void setup() 
{
 //init_serial();

  size(displayWidth, displayHeight);
  textSize(24);
 
}

void draw() 
{
  background(0);
  for(int i = 0; i < 5; i++)
  {
    fill(255,255,255);
    stroke(0,0,255);
    strokeWeight(2);
    rect(x_offset,200*i+y_offset,1700,185);
    rect(x_offset-180,y_offset+20+200*i,170,50);
    rect(x_offset-180,y_offset+120+200*i,170,65);
    stroke(180,180,180);
    line(x_offset+100,y_offset+155+200*i,x_offset+1640,y_offset+155+200*i);
    line(x_offset+100,y_offset+15+200*i,x_offset+100,y_offset+155+200*i);
    triangle(x_offset+1640, y_offset+150+200*i, x_offset+1640, y_offset+160+200*i, x_offset+1650, y_offset+155+200*i);
    triangle(x_offset+95,y_offset+15+200*i,x_offset+105,y_offset+15+200*i,x_offset+100,y_offset+5+200*i);
    fill(180,180,180);
    textAlign(CENTER);
    text("time (t)",x_offset+1660,y_offset+180+200*i);
    textAlign(LEFT);
    text("Max :",x_offset-170,y_offset+145+200*i);
    text("Min :",x_offset-170,y_offset+175+200*i);
    textAlign(CENTER);
    for(int j = 0; j < 16; j++)
    {
      line(x_offset+100+100*j,y_offset+155+200*i,x_offset+100+100*j,y_offset+150+200*i);
      text((j)*(display_time/15),x_offset+100+100*j,y_offset+175+200*i);
    }
  }
  text("Light (AU)",x_offset,y_offset);
  text("Temperature (°C)",x_offset,y_offset);
  text("Pressure (kPa)",x_offset,y_offset);
  text("Humidity",x_offset,y_offset);
  text("",x_offset,y_offset);
}

void init_serial()
{
  printArray(Serial.list()); // Listing serial ports
  myPort = new Serial(this, Serial.list()[2], 38400); //Open serial
}
