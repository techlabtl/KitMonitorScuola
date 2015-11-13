/* Codice messo insieme da Paolo Cavagnolo in primavera 2015 */
/* per il progetto del MIUR: Impariamo a consumare di meno   */
/* promosso da Nino Fanelli e ispirato da Riccardo Beltramo  */

#include <DHT.h>
#include <EmonLib.h>                   // Include Emon Library

EnergyMonitor emon1;                   // Create an instance

#define DHT_PIN A3 
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
  emon1.current(CT_PIN, 98.77);
  dht.begin();
  Serial.begin(9600);
}

void loop()
{
  
  String dataString;
  //QUALITA' ARIA
  dataString += analogRead(AIR_PIN);
  dataString += ',';

  //LUMINOSITA'
  dataString += analogRead(LIGHT_PIN);
  dataString += ',';

  //RUMORE
  for (int i = 0; i < 3000; i++) {
    j += analogRead(SOUND_PIN);
  }
  dataString += j / 3000;
  dataString += ',';

  j = 0;

  //UMIDITA'
  float h = dht.readHumidity();
  dataString += h;
  dataString += ',';

  //TEMPERATURA
  float t = dht.readTemperature() - 4; //correzione temperatura
  dataString += t;
  dataString += ',';

  //CORRENTE
  double Irms = emon1.calcIrms(1480);  // Calculate Irms only
  Irms -= 0.5;

  Serial.println(dataString);


  digitalWrite(13, HIGH);
  delay(300);
  digitalWrite(13, LOW);
  delay(300);

}

