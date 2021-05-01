#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <MFRC522.h>

#define BUZZER D8
#define DEADBOLT D1
#define RST_PIN D3 
#define SS_PIN D4
#define STASSID "Honda-ASUS"
#define STAPSK  "mobydick"
#define PINCODE "123456"
#define UNLOCKED_DELAY 6000

// const char* ssid = STASSID;
// const char* password = STAPSK;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

ESP8266WebServer server(80);

const char* www_username = "scoped22";
const char* www_password = "mobydick";

char ptr[] = "<!DOCTYPE html>"
"<html lang=\"en\">\n"
"\n"
"<head>\n"
"  <meta charset=\"UTF-8\">\n"
"  <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n"
"  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
"  <title>Document</title>\n"
"</head>\n"
"\n"
"<body>\n"
"  <h1 id=\"displayPin\" style=\"text-align:center;height:30px\"></h1>\n"
"    <div>\n"
"      <button onclick=\"addNum(1)\" style=\"font-size:1.5em; width:30%;padding:0.5em;\">1</button>\n"
"      <button onclick=\"addNum(2)\" style=\"font-size:1.5em; width:30%;padding:0.5em;\">2</button>\n"
"      <button onclick=\"addNum(3)\" style=\"font-size:1.5em; width:30%;padding:0.5em;\">3</button>\n"
"    </div>\n"
"    <div>\n"
"      <button onclick=\"addNum(4)\" style=\"font-size:1.5em; width:30%;padding:0.5em;\">4</button>\n"
"      <button onclick=\"addNum(5)\" style=\"font-size:1.5em; width:30%;padding:0.5em;\">5</button>\n"
"      <button onclick=\"addNum(6)\" style=\"font-size:1.5em; width:30%;padding:0.5em;\">6</button>\n"
"    </div>\n"
"    <div>\n"
"      <button onclick=\"addNum(7)\" style=\"font-size:1.5em; width:30%;padding:0.5em;\">7</button>\n"
"      <button onclick=\"addNum(8)\" style=\"font-size:1.5em; width:30%;padding:0.5em;\">8</button>\n"
"      <button onclick=\"addNum(9)\" style=\"font-size:1.5em; width:30%;padding:0.5em;\">9</button>\n"
"    </div>\n"
"    <div>\n"
"      <button onclick=\"clearNums()\" style=\"background-color:red; font-size:1.5em; width:30%;padding:0.5em;\">X</button>\n"
"      <button onclick=\"addNum(0)\" style=\"font-size:1.5em; width:30%;padding:0.5em;\">0</button>\n"
"      <button id=\"submitPin\" type=\"button\" style=\"background-color:green;font-size:1.5em; width:30%;padding:0.5em;\">&gt</button>\n"
"    </div>\n"
"    <h1 id=\"statusSection\" style=\"text-align:center;color:maroon;\">Locked</h4>\n"
"\n"
"<script>\n"
"  let displayPin = document.getElementById(\"displayPin\");\n"
"  let submitPin = document.getElementById(\"submitPin\");\n"
"  let statusSection = document.getElementById(\"statusSection\");\n"
"\n"
"  function addNum(num) {\n"
"    console.log(\"adding: \", num)\n"
"    if (displayPin.innerHTML.length < 6) {\n"
"      displayPin.innerHTML += num\n"
"    }\n"
"  }\n"
"\n"
"  function clearNums() {\n"
"    console.log(\"clearing\")\n"
"    displayPin.innerHTML = \"\"\n"
"  }\n"
"\n"
"  function sendPinCode() {\n"
"    let XHR = new XMLHttpRequest();\n"
"    XHR.onload = () => {\n"
"      console.log(this)\n"
"      let body = JSON.parse(this.responseText);\n"
"\n"
"      if (this.status == 200) {\n"
"        if (body[\'response\'] == \"Unlocked\") {\n"
"          statusSection.innerHTML = body[\'response\'];\n"
"        }\n"
"      } else if (this.status == 400) {\n"
"        statusSection.innerHTML = body[\'response\'];\n"
"      } else {\n"
"        statusSection.innerHTML = \"Error\";\n"
"      }\n"
"    }\n"
"    XHR.onerror = () => { alert(\"Server Error\") };\n"
"    XHR.open(\"POST\", \"http://\" + window.location.hostname + \"/unlock\");\n"
"    XHR.send(JSON.stringify({ \"pin\": displayPin.innerText }));\n"
"  }\n"
"\n"
"  submitPin.onclick = sendPinCode;\n"
"</script>\n"
"</body>\n"
"</html>\n";




void unlock() {
    digitalWrite(DEADBOLT, HIGH);
    delay(UNLOCKED_DELAY);
    digitalWrite(DEADBOLT, LOW);
}

void handleRoot() {
    server.send(200, "text/html", ptr);
}

void handleUnlock() {
    if (handlePinAuth()) {
      unlock();
    }
}

bool handlePinAuth() { 
  if (!server.hasArg("pin")){     //Parameter not found
    server.send(400, "application/json", "{'response':'Enter a pin'}"); 
  }
  else if (server.arg("pin") == PINCODE) {
    server.send(200, "application/json", "{'response':'Unlocked'}");  
    return true;
  } 
  else {
    server.send(400, "application/json", "{'reponse': 'Incorrect pin'}");  
  }
  return false; 
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void handleRFID() {
    if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    Serial.println("No card");
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    Serial.println("Can't read serial data");
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if (content.substring(1) == "B6 33 ED F8"||
      content.substring(1)=="09 8F 68 99"||
      content.substring(1)=="A9 81 AE 98"||
      content.substring(1)=="49 3D A3 98"||
      content.substring(1)== "16 17 EA F8" ||
      content.substring(1)=="36 D4 2A F9") {
        
    unlock();
  } 
}

void setup() {

  Serial.begin(115200);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  // Initialize buzzer and deadbolt signal to ouputs, low
  pinMode(DEADBOLT, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(DEADBOLT,LOW);
  digitalWrite(BUZZER,LOW);


  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);

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
  
  // <-------- API -------->
  server.on("/", HTTP_GET, handleRoot);

  server.on("/unlock", HTTP_POST, handleUnlock);
  
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.print("http://");
  Serial.println(WiFi.localIP());

}


void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  handleRFID();
}

