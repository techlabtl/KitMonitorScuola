#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SimpleDHT.h>

#define BUTTON 0

const char *ssid = "Kit Monitoraggio";

ESP8266WebServer server(80);

/*
   Go to http://192.168.4.1 in a web browser
*/

int dato = 0;
void handleRoot() {
  String info = "<h1>You are connected!</h1>";
  for (int i=0;i<10;i++){
    info+="<div>port ";
    info+=i;
    info+=" : ";
    info+=digitalRead(i);
    info+="</div>";
  }

  info+="<div>analog port 0 : ";
  info+=analogRead(0);
  info+="</div>";
  
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
