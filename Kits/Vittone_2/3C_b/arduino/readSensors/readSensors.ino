#include <Bridge.h>
#include <FileIO.h>

#include <DHT.h>
#include <EmonLib.h>                   // Include Emon Library

EnergyMonitor emon1;                   // Create an instance

#define DHT_PIN A3 // what pin we're connected to
#define CT_PIN A4
#define LIGHT_PIN A2
#define SOUND_PIN A1
#define AIR_PIN A0

#define DHTTYPE DHT22 // DHT 22 (AM2302)
DHT dht(DHT_PIN, DHTTYPE);


long int j =   0;

String datalog_file;
String buffer_file;
int frequency_delay;

const String arduino_param = "cat /mnt/sd/arduino/arduino.param | grep ";

void setup()
{
  Bridge.begin();
  FileSystem.begin();
  delay(60000); //wait for system to star correctly

  emon1.current(CT_PIN, 98.77);
  dht.begin();

  datalog_file = getParam("datalog");
  buffer_file = getParam("buffer");
  frequency_delay = getParam("delay").toInt();

}

void loop()
{

  String dataString;
  dataString += getTimeStamp();
  dataString += ',';

  //QUALITA' ARIA
  //Serial.print(analogRead(AIR_PIN));
  //Serial.print(',');
  dataString += analogRead(AIR_PIN);
  dataString += ',';

  //plotter1.plot(analogRead(AIR_PIN));


  //LUMINOSITA'
  //Serial.print(analogRead(LIGHT_PIN));
  //Serial.print(',');
  dataString += analogRead(LIGHT_PIN);
  dataString += ',';

  //plotter2.plot(analogRead(LIGHT_PIN));


  //RUMORE
  for (int i = 0; i < 3000; i++) {
    j += analogRead(SOUND_PIN);
  }
  //Serial.print(j);
  //Serial.print(',');
  dataString += j / 3000;
  dataString += ',';

  //plotter3.plot(j/3000);
  j = 0;

  //UMIDITA'
  float h = dht.readHumidity();
  //Serial.print(h);
  //Serial.print(',');

  dataString += h;
  dataString += ',';

  //plotter4.plot(h);


  //TEMPERATURA
  float t = dht.readTemperature() - 4; //correzione temperatura
  //Serial.print(t);
  //Serial.print(',');
  dataString += t;
  //dataString += ',';


  //CORRENTE
  //double Irms = emon1.calcIrms(1480);  // Calculate Irms only
  //Irms -= 0.5;
  //Serial.println(Irms);		       // Irms
  //dataString += Irms;
  //dataString += ',';

  //plotter6.plot(Irms);

  echoToFile(dataString, datalog_file);
  echoToCollector(dataString);

  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
  delay(frequency_delay);

}

void echoToFile(String dataString, String filename) {
  Process p;
  p.runShellCommand("echo '" + dataString + "' >> " + filename);
  while (p.running());
}

void echoToCollector(String dataString) {
  Process p;
  p.runShellCommand(buffer_file + " " + dataString);
  while (p.running());
}

String getTimeStamp() {
  String result;
  Process time;
  time.begin("date");
  time.addParameter("+%D,%T");
  time.run();

  while (time.available() > 0) {
    char c = time.read();
    if (c != '\n')
      result += c;
  }

  return result;
}

String getParam(String a) {
  Process p;
  String result;
  bool onRead=false;

  p.runShellCommand(arduino_param + a);

  while (p.running()); // do nothing until the process finishes, so you get the whole output
  while (p.available() > 0) {
    char c = p.read();
    if (c != '\n'){
      if (onRead){
        result += c;
      }
      if ( c==',') {
        onRead = true;
      }
    }
  }

  return result;
}
