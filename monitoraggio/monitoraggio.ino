/**
 * DOCS: 
 *  https://github.com/arduino-libraries/WiFi101/
 */


#include <SPI.h>
#include <WiFi101.h>


// Settings & vars for the AP
char ssid[] = "mkr1000";
char pass[] = "password"; // (not supported yet)
int wifi_status = WL_IDLE_STATUS;

// Settings and vars for the HTTP server
int http_port = 81;
WiFiServer http_server(http_port);
WiFiClient http_client;

// Settings and vars for the relays
byte PIN_relay_pump = 3;
byte relay_pump_status = 0;

byte debug_mode = 1;

void wifi_connect_as_ap() {
  // Is it IDLE ?
  if (wifi_status == WL_IDLE_STATUS) {
    // Try to connect
    wifi_status = WiFi.beginAP(ssid);
    // Wait 10 seconds for connection
    delay(4000);
    _debug_wifi_status();
    // Is it connected?
    wifi_connect_as_ap();
  }
}


void setup() {
  // Setup Serial
  while (!Serial);
  delay(1000);
  Serial.begin(9600);

  // Check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);       // don't continue
  }

  // Create the AP
  wifi_connect_as_ap();

  // Start the HTTP server
  http_server.begin();

  // Init the relay board
  pinMode(PIN_relay_pump, OUTPUT);
  digitalWrite(PIN_relay_pump, LOW);
}

byte _is_a_get_request(String txt) {
  String tmp = _get_request_path(txt);
  if (tmp == "N/A") {
    return 0;
  }
  return 1;
}

/**
 * @returns "N/A" if txt is not the right header 
 */
String _get_request_path(String txt) {
  if (txt.endsWith("HTTP/1.1")) {
    txt = txt.substring(0, txt.indexOf("HTTP/1.1"));
    if (txt.startsWith("GET")) {
      txt = txt.substring(3);
      txt.trim();
      Serial.println("Requesting " + txt); 
      return txt;
    }
  }
  return "N/A";
}

String _html_show_on_off_status(int status) {
  if (status == 0) {
    return "<span style='color:red;'><strong>OFF</strong></span>";
  }
  return "<span style='color:green;'><strong>ON</strong></span>";
}

void process_http_request(String path) {
  if (path == "/") {
    http_client.println("Status Relay: "+_html_show_on_off_status(relay_pump_status));
    http_client.println("<hr>Actions: <ul>");
    http_client.println("<li><a href='/pump_relay_on'>START Pump Relay</a></li>");
    http_client.println("<li><a href='/pump_relay_off'>STOP Pump Relay</a></li>");
    http_client.println("</ul>");
  } else if (path == "/pump_relay_off") {
    http_client.println("Pump Relay OFF");
    http_client.println("<hr>[<a href='/'>Back to Index</a>]");
    digitalWrite(PIN_relay_pump, LOW);
    relay_pump_status = 0;
  } else if (path == "/pump_relay_on") {
    http_client.println("Pump Relay ON");
    http_client.println("<hr>[<a href='/'>Back to Index</a>]");
    digitalWrite(PIN_relay_pump, HIGH);
    relay_pump_status = 1;
  } else {
    http_client.println("404");
  }
}

void process_http_requests() {
  // Listen for incoming http_clients
  http_client = http_server.available();
  byte tmp_request_is_a_get_request = 0;
  String tmp_get_request_line = "";

  if (http_client) {                             // if you get a http_client,
    Serial.println("New client connected.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the http_client
    while (http_client.connected()) {            // loop while the http_client's connected
      if (http_client.available()) {             // if there's bytes to read from the http_client,
        char c = http_client.read();             // read a byte, then
        // Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {
          // If the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            http_client.println("HTTP/1.1 200 OK");
            http_client.println("Content-type:text/html");
            http_client.println();
            if (tmp_request_is_a_get_request == 1) {
              process_http_request(_get_request_path(tmp_get_request_line));
            }
            break;
          }
          
          // We don't really care about other headers .. just the /GET ones
          if (_is_a_get_request(currentLine)) {
            tmp_request_is_a_get_request = 1;
            tmp_get_request_line = currentLine;
          }
          // Clear the currentLine buffer
          currentLine = "";
        }
        else if (c != '\r') {
          // Add to the currentLine buffer
          currentLine += c;
        }
      }
    }
    // close the connection:
    http_client.stop();
    Serial.println("Client disconnected.");
  }
}


void loop() {
  process_http_requests();
  
}











////////////////////////
// Non-core functions //
////////////////////////

void _debug_wifi_status() {
  if (debug_mode == 1) {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
  
    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
  
    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
    // print where to go in a browser:
    Serial.print("To see this page in action, open a browser to http://");
    Serial.print(ip);
    Serial.print(":");
    Serial.print(http_port);
    Serial.println();
  }    
}
