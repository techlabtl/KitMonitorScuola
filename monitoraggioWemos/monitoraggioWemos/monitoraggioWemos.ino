#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SimpleDHT.h>

const char *ssid = "Kit Monitoraggio";

ESP8266WebServer server(80);

SimpleDHT22 dht;
#define DHT22PIN D0

float temperature = 0;
float humidity = 0;
int luminosity;

/*
   Go to http://192.168.4.1 in a web browser
*/

void handleRoot() {
  String info="<meta charset=\"utf-8\"/>";
  info += "<h1>Benvenuto!</h1>";
  info += "<p>Temperatura: ";
  info +=temperature;
  info += " °C</p><p>Umidita: ";
  info += humidity;
  info += " %</p><p>Luminosita: ";
  info += luminosity;
  info += " lux</p>";
  info += "<script> setTimeout(function(){location.reload();}, 5000);  </script>";
  server.send(200, "text/html", info);

}

void setup() {
  pinMode(DHT22PIN, INPUT);


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

  //to prevent DHT reading errors
  delay(3000);
}

void loop() {
  server.handleClient();
  readSensors();
  delay(1000);
}

void readSensors() {
  temperature = 0;
  humidity = 0;
  luminosity;

  int err = SimpleDHTErrSuccess;
  if ((err = dht.read2(DHT22PIN, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT22 failed, err="); Serial.println(err); delay(2000);
    return;
  }

  luminosity = analogRead(A0);

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Luminosity: ");
  Serial.print(luminosity);
  Serial.println(" Lux");


}

