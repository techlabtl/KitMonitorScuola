#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include "Adafruit_BME680.h"
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
#define SAVE_TIME_SMALL 10*60*1000
//saves to SD every x milliseconds
#define SAVE_TIME_MID 60*60*1000
//saves to SD every x milliseconds
#define SAVE_TIME_LONG 5*60*60*1000

//ssid name
const char *ssid = "Kit Monitoraggio";


Adafruit_BME680 bme;

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

ESP8266WebServer server(80);

float temperature = 0;
float humidity = 0;
float pressure = 0;
float luminosity = 0;
float gas = 0;
float sound = 0;

float total_small_time[]={0,0,0,0,0,0};
float total_mid_time[]={0,0,0,0,0,0};
float total_long_time[]={0,0,0,0,0,0};

float last_data_saved_small[]={NULL,NULL,NULL,NULL,NULL,NULL};
float last_data_saved_mid[]={NULL,NULL,NULL,NULL,NULL,NULL};
float last_data_saved_long[]={NULL,NULL,NULL,NULL,NULL,NULL};

long int last_saved_small;
long int last_saved_mid;
long int last_saved_long;

boolean is_sd_card_present;

//used to switch an internal resistor to fool the powerbank about current demand and keep it turned on..
bool fool_powerbank = false;

/*
   Go to http://192.168.4.1 in a web browser
*/

String encodeGasMeasure(float kOhms){
  if (kOhms>360){
    return "Ottima";
  }
  if (kOhms>180){
    return "Molto Buona";
  }
  if (kOhms>90){
    return "Buona";
  }
  if (kOhms>45){
    return "Non Buona";
  }
  if (kOhms>22){
    return "Cattiva";
  }
  if (kOhms>11){
    return "Pessima";
  }
  return "Mortale";
}


void handleRoot() {
  String info = "<meta charset=\"utf-8\"/>";
  if (!is_sd_card_present){
    info += "ATTENZIONE! SD CARD NON RILEVATA";
  }
  info += "<h1>Kit Monitoraggio</h1>";
  info += "<h2>Instantaneo:</h2>";
  info += "<p>Temperatura: ";
  info += temperature;
  info += " °C</p><p>Umidita: ";
  info += humidity;
  info += " %</p><p>Pressione: ";
  info += pressure;
  info += " hPa</p><p>Luminosita: ";
  info += luminosity;
  info += " lux</p><p>Qualita' dell'Aria: ";
  info += encodeGasMeasure(gas)+"("+gas;
  info += " KOhms)</p><p>Rumore: ";
  info += sound;
  info += " db</p>";
  
  info += "<h2>Ultimi 10 minuti:</h2>";
  if (last_data_saved_small[0]==NULL){
    info += "<p>In fase di misurazione...</p>";
  }
  else{
    info += "<p>Temperatura: ";
    info += last_data_saved_small[0];
    info += " °C</p><p>Umidita: ";
    info += last_data_saved_small[1];
    info += " %</p><p>Pressione: ";
    info += last_data_saved_small[2];
    info += " hPa</p><p>Luminosita: ";
    info += last_data_saved_small[3];
    info += " lux</p><p>Qualita' dell'Aria: ";
    info += encodeGasMeasure(last_data_saved_small[4])+"("+last_data_saved_small[4];
    info += " KOhms)</p><p>Rumore: ";
    info += last_data_saved_small[5];
    info += " db</p>";
  }

  info += "<h2>Ultimi 60 minuti:</h2>";
  if (last_data_saved_mid[0]==NULL){
    info += "<p>In fase di misurazione...</p>";
  }
  else{
    info += "<p>Temperatura: ";
    info += last_data_saved_mid[0];
    info += " °C</p><p>Umidita: ";
    info += last_data_saved_mid[1];
    info += " %</p><p>Pressione: ";
    info += last_data_saved_mid[2];
    info += " hPa</p><p>Luminosita: ";
    info += last_data_saved_mid[3];
    info += " lux</p><p>Qualita' dell'Aria: ";
    info += encodeGasMeasure(last_data_saved_mid[4])+"("+last_data_saved_mid[4];
    info += " KOhms)</p><p>Rumore: ";
    info += last_data_saved_mid[5];
    info += " db</p>";
  }

  info += "<h2>Ultime 5 ore:</h2>";
  if (last_data_saved_long[0]==NULL){
    info += "<p>In fase di misurazione...</p>";
  }
  else{
    info += "<p>Temperatura: ";
    info += last_data_saved_long[0];
    info += " °C</p><p>Umidita: ";
    info += last_data_saved_long[1];
    info += " %</p><p>Pressione: ";
    info += last_data_saved_long[2];
    info += " hPa</p><p>Luminosita: ";
    info += last_data_saved_long[3];
    info += " lux</p><p>Gas: ";
    info += encodeGasMeasure(last_data_saved_long[4])+"("+last_data_saved_long[4];
    info += " KOhms)</p><p>Rumore: ";
    info += last_data_saved_long[5];
    info += " db</p>";
  }
  //ricarica la pagina in automatica dopo 5 secondi, così da avere le informazioni aggiornate
  info += "<script> setTimeout(function(){location.reload();}, 5000);  </script>";
  server.send(200, "text/html", info);

}

void setup() {
  //internal resistor to fool powerbank
  pinMode(D3, OUTPUT);
  pinMode(D4, INPUT_PULLUP);

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
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms


  //TSL SETUP
  if (!tsl.begin())
  {
    /* There was a problem detecting the TSL2561 ... check your connections */
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  tsl.enableAutoRange(true);
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);

  //SD SETUP
  if (!SD.begin(chipSelect)) {
    Serial.println("ERROR! Card failed, or not present");
    is_sd_card_present = false;
  }
  is_sd_card_present = true;

  last_saved_small = millis();

  for (int i=0;i<(sizeof(total_small_time)/sizeof(float));i++){
    total_small_time[i]=0;
    total_mid_time[i]=0;
    total_long_time[i]=0;
  }

  //updating log file
  String dataString = "!!! sensor reset;from now on: reading every ";
  dataString += SENSOR_TIME;
  dataString += " milliseconds, saving every ";
  dataString += SAVE_TIME_SMALL;
  dataString += " milliseconds\n";
  dataString += "temp °C;Hum %;Pres hPa;Lum lux;gas in KOhms;Rumo db";
  if (is_sd_card_present) {
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

  //update small medium
  if (millis() - last_saved_small > SAVE_TIME_SMALL) {
    if (is_sd_card_present) {
      saveToSd();
    }
    last_saved_small = millis();
    float kk = float(SAVE_TIME_SMALL) / float(SENSOR_TIME);
    kk=ceil(kk/2.0f);
    for (int i=0;i<(sizeof(total_small_time)/sizeof(float));i++){
      last_data_saved_small[i]=total_small_time[i]/kk;
      total_small_time[i]=0;
    }
  }

  //update mid medium
  if (millis() - last_saved_mid > SAVE_TIME_MID) {
    last_saved_mid = millis();
    float kk = float(SAVE_TIME_MID) / float(SENSOR_TIME);
    kk=ceil(kk/2.0f);
    for (int i=0;i<(sizeof(total_mid_time)/sizeof(float));i++){
      last_data_saved_mid[i]=total_mid_time[i]/kk;
      total_mid_time[i]=0;
    }
  }
  
  //update long medium
  if (millis() - last_saved_long > SAVE_TIME_LONG) {
    last_saved_long = millis();
    float kk = float(SAVE_TIME_LONG) / float(SENSOR_TIME);
    kk=ceil(kk/2.0f);
    for (int i=0;i<(sizeof(total_long_time)/sizeof(float));i++){
      last_data_saved_long[i]=total_long_time[i]/kk;
      total_long_time[i]=0;
    }
  }

  delay(SENSOR_TIME);
  fool_powerbank = !fool_powerbank;
  digitalWrite(D3, fool_powerbank);

}

void saveToSd() {
  String dataString = "";
  float kk = float(SAVE_TIME_SMALL) / float(SENSOR_TIME);
  kk=ceil(kk/2.0f);
  for (int i=0;i<(sizeof(total_small_time)/sizeof(float));i++){
    dataString += String(total_small_time[i] / kk) + ";"; 
  }

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
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    temperature = 0;
    humidity = 0;
    pressure = 0;
    gas = 0;
  }
  else {
    temperature = bme.temperature;
    humidity = bme.humidity;
    pressure = bme.pressure / 100.0F;
    gas = bme.gas_resistance / 1000.0;
  }

  /* Get a new sensor light event */
  sensors_event_t event;
  tsl.getEvent(&event);
  luminosity = event.light;

  sound = 0;
  for (int i = 0; i < 100; i++) {
    float k = analogRead(0);
    if (k > sound) {
      sound = k;
    }
    delay(1);
  }

  //update total variables
  total_small_time[0] += temperature;
  total_small_time[1] += humidity;
  total_small_time[2] += pressure;
  total_small_time[3] += luminosity;
  total_small_time[4] += gas;
  total_small_time[5] += sound;

  total_mid_time[0] += temperature;
  total_mid_time[1] += humidity;
  total_mid_time[2] += pressure;
  total_mid_time[3] += luminosity;
  total_mid_time[4] += gas;
  total_mid_time[5] += sound;

  total_long_time[0] += temperature;
  total_long_time[1] += humidity;
  total_long_time[2] += pressure;
  total_long_time[3] += luminosity;
  total_long_time[4] += gas;
  total_long_time[5] += sound;
}

