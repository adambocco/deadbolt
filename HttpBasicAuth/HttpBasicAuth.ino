#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#ifndef STASSID
#define STASSID "Honda-ASUS"
#define STAPSK  "mobydick"
#define MP1 D1
#define MP2 D2
#define MP3 D6
#define MP4 D7
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
int pos;
int addr = 0;
int interval = 10;

ESP8266WebServer server(80);

const char* www_username = "scoped22";
const char* www_password = "narusrocks";

String webPage1 = 
  "<!DOCTYPE HTML>"
  "<html style='background-color:black;' lang='en'>"
  "<head>"
  "<meta charset='utf-8' />"
  "<meta http-equiv='X-UA-Compatible' content='IE=edge,chrome=1' />"
  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
  "<title>LED CONTROLLER</title>"
  "<link href='https://fonts.googleapis.com/css?family=Lato:400,900&subset=latin-ext' rel='stylesheet'>"
  "<link href='https://fonts.googleapis.com/icon?family=Material+Icons' rel='stylesheet'>"
  "<style>"
  "</style>"
  "</head>"
  
  "<body>";

String webPage2 =   
  "<h1><a style='text-decoration:none;border:3px solid black;border-radius:10px;background-color:#5AF;color:black;float:left;position:relative;margin:1em;' href='http://192.168.2.149/off'>LEFT</a></h1>"
  "<h1><a style='text-decoration:none;border:3px solid black;border-radius:10px;background-color:#5AF;color:black;float:left;position:relative;margin:1em;' href='http://192.168.2.149/on'>RIGHT</a></h1>"
  "</body>";
  

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MP1, OUTPUT);
  pinMode(MP2, OUTPUT);
  pinMode(MP3, OUTPUT);
  pinMode(MP4, OUTPUT);
  digitalWrite(MP1, LOW);
  digitalWrite(MP2, LOW);
  digitalWrite(MP3, LOW);
  digitalWrite(MP4, LOW);
  EEPROM.begin(512);
  EEPROM.get(addr, pos);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connect Failed! Rebooting...");
    delay(200);
    ESP.restart();
  }
    ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  ArduinoOTA.setHostname("hondaesp");
  ArduinoOTA.setPassword("mobydick");
  
  
  server.on("/", []() {
    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }
    server.send(200, "text/html", (webPage1+ "<p style='font-size: 46px;color:white;font-weight:bold;border:2px white solid;display:inline-block;margin:1em 0 0 2.6em;'>" + String(pos) + "</p>" +webPage2));
  });
  server.on("/on", []() {
    digitalWrite(LED_BUILTIN, HIGH);
    right(40);
    server.send(200, "text/html", (webPage1 + "<p style='font-size: 46px;color:white;font-weight:bold;border:2px white solid;display:inline-block;margin:1em 0 0 2.6em;'>" + String(pos) + "</p>" + webPage2));
  });
  server.on("/off", []() {
    digitalWrite(LED_BUILTIN, LOW);
    left(40);
    server.send(200, "text/html", (webPage1 + "<p style='font-size: 46px;color:white;font-weight:bold;border:2px white solid;display:inline-block;margin:1em 0 0 2.6em;'>" + String(pos) + "</p>" + webPage2));
  });
  server.begin();

  Serial.print("Open http://");
  Serial.print(WiFi.localIP());
  Serial.println("/ in your browser to see it working");
}

void halt() {
  digitalWrite(MP1, LOW);
  digitalWrite(MP2, LOW);
  digitalWrite(MP3, LOW);
  digitalWrite(MP4, LOW);
}

void right(int steps) {
  pos+= steps;
  EEPROM.put(addr, pos);
  EEPROM.commit();
  for (int i=0; i<steps; i++) {
    digitalWrite(MP1, HIGH);
    delay(interval);
    digitalWrite(MP4, LOW);
    delay(interval);
    digitalWrite(MP2, HIGH);
    delay(interval);
    digitalWrite(MP1, LOW);
    delay(interval);
    digitalWrite(MP3, HIGH);
    delay(interval);
    digitalWrite(MP2, LOW);
    delay(interval);
    digitalWrite(MP4, HIGH);
    delay(interval);
    digitalWrite(MP3, LOW);
    delay(interval);
  }
  halt();
}

void left(int steps) {
  pos-= steps;
  EEPROM.put(addr, pos);
  EEPROM.commit();
  for (int i=0; i<steps; i++) {
    digitalWrite(MP4, HIGH);
    delay(interval);
    digitalWrite(MP1, LOW);
    delay(interval);
    digitalWrite(MP3, HIGH);
    delay(interval);
    digitalWrite(MP4, LOW);
    delay(interval);
    digitalWrite(MP2, HIGH);
    delay(interval);
    digitalWrite(MP3, LOW);
    delay(interval);
    digitalWrite(MP1, HIGH);
    delay(interval);
    digitalWrite(MP2, LOW);
    delay(interval);
  }
  halt();
}


void loop() {
  ArduinoOTA.handle();
  server.handleClient();
}
