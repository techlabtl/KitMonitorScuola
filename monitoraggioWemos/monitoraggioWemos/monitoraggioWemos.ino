#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssid = "Kit Monitoraggio";

ESP8266WebServer server(80);

/* 
   Go to http://192.168.4.1 in a web browser
*/

int dato=0;
void handleRoot() {
  String info="<h1>You are connected! Dato:";
  info+=dato;
  info+="</h1>";
  server.send(200, "text/html", info);

  dato++;
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
}

void loop() {
  server.handleClient();
}

