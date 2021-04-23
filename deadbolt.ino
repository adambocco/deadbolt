#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put your SSID & Password */
const char* ssid = "NodeMCU";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

uint8_t LED1pin = D7;
bool LED1status = LOW;

uint8_t LED2pin = D6;
bool LED2status = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  if(LED1status)
  {digitalWrite(LED1pin, HIGH);}
  else
  {digitalWrite(LED1pin, LOW);}
  
  if(LED2status)
  {digitalWrite(LED2pin, HIGH);}
  else
  {digitalWrite(LED2pin, LOW);}
}

void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
}

void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(true,LED2status)); 
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status)); 
}

void handle_led2on() {
  LED2status = HIGH;
  Serial.println("GPIO6 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,true)); 
}

void handle_led2off() {
  LED2status = LOW;
  Serial.println("GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html>"
  ptr += "<html lang=\"en\">"
  ptr += "<head>"
  ptr += "  <meta charset=\"UTF-8\">"
  ptr += "  <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">"
  ptr += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
  ptr += "  <title>Document</title>"
  ptr += "</head>"
  ptr += "<body>"
  ptr += "      <h3 id=\"displayPin\" style=\"text-align:center;height:30px\"></h1>"
  ptr += "      <div>"
  ptr += "        <button onclick=\"addNum(1)\" style=\"width:30%;padding:0.5em;\">1</button>"
  ptr += "        <button onclick=\"addNum(2)\" style=\"width:30%;padding:0.5em;\">2</button>"
  ptr += "        <button onclick=\"addNum(3)\" style=\"width:30%;padding:0.5em;\">3</button>"
  ptr += "      </div>"
  ptr += "      <div>"
  ptr += "        <button onclick=\"addNum(4)\" style=\"width:30%;padding:0.5em;\">4</button>"
  ptr += "        <button onclick=\"addNum(5)\" style=\"width:30%;padding:0.5em;\">5</button>"
  ptr += "        <button onclick=\"addNum(6)\" style=\"width:30%;padding:0.5em;\">6</button>"
  ptr += "      </div>"
  ptr += "      <div>"
  ptr += "        <button onclick=\"addNum(7)\" style=\"width:30%;padding:0.5em;\">7</button>"
  ptr += "        <button onclick=\"addNum(8)\" style=\"width:30%;padding:0.5em;\">8</button>"
  ptr += "        <button onclick=\"addNum(9)\" style=\"width:30%;padding:0.5em;\">9</button>"
  ptr += "      </div>"
  ptr += "      <div>"
  ptr += "        <button onclick=\"clearNums()\" style=\"width:30%;padding:0.5em;\">X</button>"
  ptr += "        <button onclick=\"addNum(0)\" style=\"width:30%;padding:0.5em;\">0</button>"
  ptr += "        <button id=\"submitPin\" type=\"button\" style=\"width:30%;padding:0.5em;\">&gt</button>"
  ptr += "      </div>"
  ptr += "      <h4 style=\"text-align:center;color:maroon;\">Locked</h4>"
  ptr += "</body>"
  ptr += "<script>"
  ptr += "  displayPin = document.getElementById(\"displayPin\");"
  ptr += "  function addNum(num) {"
  ptr += "    console.log(\"adding: \",num)"
  ptr += "    if (displayPin.innerHTML.length < 6) {"
  ptr += "      displayPin.innerHTML += num"
  ptr += "    }"
  ptr += "  }"
  ptr += "  function clearNums() {"
  ptr += "    console.log(\"clearing\")"
  ptr += "    displayPin.innerHTML = \"\""
  ptr += "  }"
  ptr += "  let but1 = document.getElementById(\"but1\")"
  ptr += "  let submitPin = document.getElementById(\"submitPin\")"
  ptr += "  submitPin.addEventListener(\"click\", ()=> {"
  ptr += "    window.location.replace(\"http://\"+location.hostname+\"/\");"
  ptr += "  })"
  ptr += "</script>"
  ptr += "</html>"

   if(led1stat)
  {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}

  if(led2stat)
  {ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";}
  else
  {ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
