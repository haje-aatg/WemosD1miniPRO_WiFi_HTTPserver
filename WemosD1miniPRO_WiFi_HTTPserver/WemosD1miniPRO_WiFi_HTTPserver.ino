#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
/////////////////////
// Pin Definitions //
/////////////////////
//////////////////////
// WiFi Definitions //
//////////////////////
#define ssid "testSSID"    // WiFi SSID
#define password "testPW"  // WiFi password
//////////////////////////
// Variable Definitions //
//////////////////////////
ESP8266WebServer server(80);
unsigned long Time = 0;
String ledTilstand = "OFF";

void setup() {
  initHardware();
  setupWiFi();
  server.on("/", handleRoot);  // link to the function that manage launch page
  server.begin();
  Serial.println("HTTP server started");
  digitalWrite(LED_BUILTIN, HIGH);  //OFF is HIGH on Wemos D1 mini PRO
}

void loop() {
  Time = millis();
  Serial.println(Time);
  server.handleClient();
  delay(1000);
}
///////////////
// Functions //
///////////////
void initHardware() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}

void setupWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {  // Vent uendeligt for en forbindelse
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void handleRoot() {
  if (server.hasArg("LED")) {
    handleSubmit();
  } else {
    server.send(200, "text/html", getPage());
  }
}

String getPage() {
  String page = "<html lang=fr-FR><head><meta http-equiv='refresh' content='1'/>";  //Refresh time er her 1 sekund (content='1')
  page += "<title>Wemos D1 mini PRO (ESP8266) Demo</title>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  page += "</head><body><h1>Wemos D1 mini PRO (ESP8266) Demo</h1>";
  page += "<h3>Timestamp</h3>";
  page += "<ul><li>Time since start : ";
  page += Time;
  page += "ms</li></ul>";
  page += "<h3>LED</h3>";
  page += "<form action='/' method='POST'>";
  page += "<ul><li>D0 (Tilstand: ";
  page += ledTilstand;
  page += ")";
  page += "<INPUT type='radio' name='LED' value='1'>ON";
  page += "<INPUT type='radio' name='LED' value='0'>OFF</li></ul>";
  page += "<INPUT type='submit' value='Udf&#248r'>";
  page += "<br><br><p><a href='https://www.eucnord.dk'>www.eucnord.dk</p>";
  page += "</body></html>";
  return page;
}

void handleSubmit() {
  String LEDValue;
  LEDValue = server.arg("LED");
  Serial.print("Set GPIO: ");
  Serial.println(LEDValue);
  if (LEDValue == "1") {
    digitalWrite(LED_BUILTIN, LOW);
    ledTilstand = "On";
    server.send(200, "text/html", getPage());
  } else if (LEDValue == "0") {
    digitalWrite(LED_BUILTIN, HIGH);
    ledTilstand = "Off";
    server.send(200, "text/html", getPage());
  } else {
    Serial.println("Err Led Value");
  }
}
