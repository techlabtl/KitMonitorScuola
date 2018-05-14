#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_TSL2561_U.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define chipSelect D8


///CUSTOMIZATION VARIABLES
//set the threshold of light on/off in lux
#define LIGHT_THRESHOLD 500
//checks sensors every x milliseconds
#define SENSOR_TIME 1*1000
//saves to SD every x milliseconds
#define SAVE_TIME 10*60*1000
//ssid name
const char *ssid = "Kit Monitoraggio";


Adafruit_BME280 bme;

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

ESP8266WebServer server(80);

float temperature = 0;
float humidity = 0;
float pressure = 0;
float luminosity = 0;
bool is_light_on = false;

float total_temperature = 0;
float total_humidity = 0;
float total_pressure = 0;
float total_luminosity = 0;
float total_light_on = 0;

long int last_saved;
boolean is_sd_card_present;
/*
   Go to http://192.168.4.1 in a web browser
*/

void handleRoot() {
  String info="<meta charset=\"utf-8\"/>";
  info += "<h1>Kit Monitoraggio</h1>";
  info += "<p>Temperatura: ";
  info +=temperature;
  info += " °C</p><p>Umidita: ";
  info += humidity;
  info += " %</p><p>Pressione: ";
  info += pressure;
  info += " hPa</p><p>Luminosita: ";
  info += luminosity;
  info += " lux</p><p>Luce ";
  if (is_light_on){
    info += "Accesa";  
  }
  else{
    info += "Spenta";  
  }
  //ricarica la pagina in automatica dopo 5 secondi, così da avere le informazioni aggiornate
  //info += "<script> setTimeout(function(){location.reload();}, 5000);  </script>";
  server.send(200, "text/html", info);

}

void reset_total_variables(){
  total_temperature=0;
  total_humidity=0;
  total_pressure=0;
  total_luminosity=0;
  total_light_on=0;
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");


  //BME SETUP
  bool status;
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin();  
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
  }


  //TSL SETUP
  if(!tsl.begin())
  {
    /* There was a problem detecting the TSL2561 ... check your connections */
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  tsl.enableAutoRange(true);
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);

  //SD SETUP
  if (!SD.begin(chipSelect)) {
    Serial.println("ERROR! Card failed, or not present");
    is_sd_card_present=false;
  }
  is_sd_card_present=true;

  last_saved=millis();

  reset_total_variables();
  
  //updating log file
  String dataString="!!! sensor reset;from now on: reading every ";
  dataString+=SENSOR_TIME;
  dataString+=" milliseconds, saving every ";
  dataString+=SAVE_TIME;
  dataString+=" milliseconds\n";
  dataString+="temp °C;Hum %;Pres hPa;Lum lux;light on in seconds;";
  if (is_sd_card_present){
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
    }
    else {
      Serial.println("error opening datalog.txt");
    }
  }
}

void loop() {
  server.handleClient();
  readSensors();
  if (millis()-last_saved>SAVE_TIME){
    if (is_sd_card_present){
      saveToSd();
    }
    last_saved=millis();
    reset_total_variables();
  }
  delay(SENSOR_TIME);
}

void saveToSd(){
  String dataString = "";
  float kk = float(SAVE_TIME)/float(SENSOR_TIME);
  dataString+=String(total_temperature/kk)+";";
  dataString+=String(total_humidity/kk)+";";
  dataString+=String(total_pressure/kk)+";";
  dataString+=String(total_luminosity/kk)+";";
  dataString+=String(total_light_on)+";";

  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }
  else {
    Serial.println("error opening datalog.txt");
  }
}

void readSensors() {
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure()/100.0F;

  /* Get a new sensor light event */ 
  sensors_event_t event;
  tsl.getEvent(&event);
  luminosity = event.light;

  //check if light is on
  is_light_on=analogRead(0)>LIGHT_THRESHOLD;

  //update total variables
  total_temperature+=temperature;
  total_humidity+=humidity;
  total_pressure+=pressure;
  total_luminosity+=luminosity;
  if(is_light_on){
    total_light_on+=SENSOR_TIME/1000.0F;
  }
}

