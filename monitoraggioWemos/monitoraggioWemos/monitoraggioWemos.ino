#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_TSL2561_U.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

Adafruit_BME280 bme;

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

const char *ssid = "Kit Monitoraggio";

ESP8266WebServer server(80);

float temperature = 0;
float humidity = 0;
float pressure = 0;
int luminosity = 0;

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
  info += " lux</p>";
  //ricarica la pagina in automatica dopo 5 secondi, così da avere le informazioni aggiornate
  info += "<script> setTimeout(function(){location.reload();}, 5000);  </script>";
  server.send(200, "text/html", info);

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
}

void loop() {
  server.handleClient();
  readSensors();
  delay(1000);
}

void readSensors() {
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure()/100.0F;

  /* Get a new sensor light event */ 
  sensors_event_t event;
  tsl.getEvent(&event);
  luminosity = event.light;
}

