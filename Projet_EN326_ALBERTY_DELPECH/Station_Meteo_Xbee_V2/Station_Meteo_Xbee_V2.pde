import java.util.*;
import processing.serial.*;
import org.gicentre.utils.stat.*;        // For chart classes.
import org.gicentre.utils.colour.*;        // For colour classes.
import org.gicentre.utils.colour.ColourTable;

Serial myPort;  // The serial port
boolean Serial_flag = false;
int display_time = 60; // seconds
int refresh_rate = 1; // Hz
int x_offset = 200;
int y_offset = 80;
int width_graph = 1700;
int height_graph = 235;
String text_pattern[] = {"Humidity:", "Temperature:", "Luminosity:", "Pressions:"};
float[] input;
float[] max;
float[] min;
float[][] y_scale;
float[][] values;
int nb_points = 0;
int max_points = display_time*refresh_rate;

float[] x_scale = new float[max_points];

XYChart HygroGraph;
XYChart TempGraph;
XYChart LumGraph;
XYChart PresGraph;

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

  for (int i = 0; i < max_points; i++)
    x_scale[i] = i;

  HygroGraph = Graph_meteo();
  HygroGraph.setData(x_scale, values[0]);
  HygroGraph.setXAxisLabel("Temps (s)");
  HygroGraph.setLineColour(color(0, 0, 255));

  TempGraph = Graph_meteo();
  TempGraph.setData(x_scale, values[1]);
  TempGraph.setXAxisLabel("Temps (s)");
  TempGraph.setLineColour(color(255, 0, 0));

  LumGraph = Graph_meteo();
  LumGraph.setData(x_scale, values[2]);
  LumGraph.setXAxisLabel("Temps (s)");
  LumGraph.setLineColour(color(255, 255, 0));

  PresGraph = Graph_meteo();
  PresGraph.setData(x_scale, values[3]);
  PresGraph.setXAxisLabel("Temps (s)");

  for (int i = 0; i < 7; i++)
    input[i] = 0.0;
  while ((input[4]!= 1.0) && (input[5]!= 1.0) && (input[6]!= 1.0) && (input[7]!= 1.0))
    input = Read_data(input);
  input[4] = input[5] = input[6] = input[7] = 0.0;

  for (int i = 0; i < 4; i++)
  {
    min[i] = max[i] = input[i];
  }
}

void draw()
{
  if (myPort.available() > 0)
  {
    while (myPort.available() > 0)
    {
      while ((input[4]!= 1.0) && (input[5]!= 1.0) && (input[6]!= 1.0) && (input[7]!= 1.0))
        input = Read_data(input);
      input[4] = input[5] = input[6] = input[7] = 0.0;
    }
    HygroGraph.setData(x_scale, values[0]);
    TempGraph.setData(x_scale, values[1]);
    LumGraph.setData(x_scale, values[2]);
    PresGraph.setData(x_scale, values[3]);
    
    nb_points = (nb_points < max_points) ? nb_points + 1 : nb_points;
  }
  background(125);

  for (int i = 0; i < 4; i++)
  {
    fill(255, 255, 255);
    stroke(255, 255, 255);
    strokeWeight(2);
    rect(x_offset-10, 250*i+y_offset, 1710, 235); //graph background
    rect(x_offset-190, y_offset+170+250*i, 170, 65); //"min max" background

    fill(0, 0, 0);
    textAlign(LEFT);
    text("Max :", x_offset-180, y_offset+195+250*i);
    text("Min :", x_offset-180, y_offset+225+250*i);
  }

  fill(0, 0, 0);
  textAlign(CENTER);
  textSize(50);
  text("Station Météo XBee - Lino & Maxime", displayWidth / 2, 50);
  textSize(24);

  fill(0, 0, 0);
  textAlign(LEFT);
  text("Humidity (%)", x_offset-180, y_offset+30f);
  text("Temperature (°C)", x_offset-180, y_offset+30+250);
  text("Light (AU)", x_offset-180, y_offset+30+500);
  text("Pressure (kPa)", x_offset-180, y_offset+30+750);

  HygroGraph.draw(x_offset, y_offset, width_graph, height_graph);
  TempGraph.draw(x_offset, 250*1+y_offset, width_graph, height_graph);
  LumGraph.draw(x_offset, 250*2+y_offset, width_graph, height_graph);
  PresGraph.draw(x_offset, 250*3+y_offset, width_graph, height_graph);

  for (int i = 0; i < 4; i++)
  {

    min[i] = max[i] = values[i][0];
    for (int j = 0; j < nb_points; j++)
    {
      if (max[i] < values[i][j])
      {
        max[i] = values[i][j];
      } else if (min[i] > values[i][j])
      {
        min[i] = values[i][j];
      }
    }

    //Affichage des min/max
    fill(0, 0, 0);
    textAlign(LEFT);
    text(max[i], x_offset-100, y_offset+195+250*i);
    text(min[i], x_offset-100, y_offset+225+250*i);

    //Décalage des valeurs pour faire avancer le graphique
    for (int j = max_points-1; j > 0; j--)
      values[i][j] = values[i][j-1];

    values[i][0] = input[i];  //Sauvegarde des valeur (Input) dans le tableau des graphiques (values)
  }

  delay(1000/refresh_rate);
}

/*
 * Constructeur de graph avec les bons paramètres
 */
XYChart Graph_meteo()
{
  XYChart mychart = new XYChart(this);
  mychart.showXAxis(true);
  mychart.showYAxis(true);
  mychart.setMinY(0);
  mychart.setPointSize(0);
  mychart.setLineWidth(2);

  mychart.setLineColour(54);
  mychart.setAxisColour(54);
  mychart.setAxisLabelColour(54);
  mychart.setAxisValuesColour(54);

  return mychart;
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
          data[4] = 1.0;
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
