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
int jj = 0;

void setup()
{
  Bridge.begin();
  FileSystem.begin();
  delay(2000);

  emon1.current(CT_PIN, 98.77);
  dht.begin();

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

  echoToFile(dataString, "/mnt/sda1/SD_A/datalog.txt");
  echoToCollector(dataString);

  // jj++;
  // if (jj > 300) {
  //   echoToPlotly();
  //   digitalWrite(13, HIGH);
  //   delay(1000);
  //   digitalWrite(13, LOW);
  //   delay(1000);
  //   jj = 0;
  // }

  digitalWrite(13, HIGH);
  delay(300);
  digitalWrite(13, LOW);
  delay(300);

  delay(30000);

}

void echoToFile(String dataString, String filename) {
  Process p;
  p.runShellCommand("echo '" + dataString + "' >> " + filename);
  while (p.running());
}

void echoToCollector(String dataString) {
  Process p;
  p.runShellCommand("/mnt/sda1/SD_A/collector.py " + dataString);
  while (p.running());
}

void echoToPlotly() {
  Process p;
  p.runShellCommand("/mnt/sda1/SD_A/plotterA.py");
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
