
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "Honda-ASUS";
const char* password = "mobydick";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "off";
String output4State = "off";

// Assign output variables to GPIO pins
const int output5 = 5;
const int output4 = 4;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}
void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              output5State = "off";
              digitalWrite(output5, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output4State = "on";
              digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "off";
              digitalWrite(output4, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("<p>GPIO 5 - State " + output5State + "</p>");
            // If the output5State is off, it displays the ON button       
            if (output5State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 4  
            client.println("<p>GPIO 4 - State " + output4State + "</p>");
            // If the output4State is off, it displays the ON button       
            if (output4State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}



String SendHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html>";
  ptr += "<html lang=\"en\">";
  ptr += "<head>";
  ptr += "  <meta charset=\"UTF-8\">";
  ptr += "  <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">";
  ptr += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  ptr += "  <title>Document</title>";
  ptr += "</head>";
  ptr += "<body>";
  ptr += "      <h3 id=\"displayPin\" style=\"text-align:center;height:30px\"></h1>";
  ptr += "      <div>";
  ptr += "        <button onclick=\"addNum(1)\" style=\"width:30%;padding:0.5em;\">1</button>";
  ptr += "        <button onclick=\"addNum(2)\" style=\"width:30%;padding:0.5em;\">2</button>";
  ptr += "        <button onclick=\"addNum(3)\" style=\"width:30%;padding:0.5em;\">3</button>";
  ptr += "      </div>";
  ptr += "      <div>";
  ptr += "        <button onclick=\"addNum(4)\" style=\"width:30%;padding:0.5em;\">4</button>";
  ptr += "        <button onclick=\"addNum(5)\" style=\"width:30%;padding:0.5em;\">5</button>";
  ptr += "        <button onclick=\"addNum(6)\" style=\"width:30%;padding:0.5em;\">6</button>";
  ptr += "      </div>";
  ptr += "      <div>";
  ptr += "        <button onclick=\"addNum(7)\" style=\"width:30%;padding:0.5em;\">7</button>";
  ptr += "        <button onclick=\"addNum(8)\" style=\"width:30%;padding:0.5em;\">8</button>";
  ptr += "        <button onclick=\"addNum(9)\" style=\"width:30%;padding:0.5em;\">9</button>";
  ptr += "      </div>";
  ptr += "      <div>";
  ptr += "        <button onclick=\"clearNums()\" style=\"width:30%;padding:0.5em;\">X</button>";
  ptr += "        <button onclick=\"addNum(0)\" style=\"width:30%;padding:0.5em;\">0</button>";
  ptr += "        <button id=\"submitPin\" type=\"button\" style=\"width:30%;padding:0.5em;\">&gt</button>";
  ptr += "      </div>";
  ptr += "      <h4 style=\"text-align:center;color:maroon;\">Locked</h4>";
  ptr += "</body>";
  ptr += "<script>";
  ptr += "  displayPin = document.getElementById(\"displayPin\");";
  ptr += "  function addNum(num) {";
  ptr += "    console.log(\"adding: \",num)";
  ptr += "    if (displayPin.innerHTML.length < 6) {";
  ptr += "      displayPin.innerHTML += num";
  ptr += "    }";
  ptr += "  }";
  ptr += "  function clearNums() {";
  ptr += "    console.log(\"clearing\")";
  ptr += "    displayPin.innerHTML = \"\"";
  ptr += "  }";
  ptr += "  let but1 = document.getElementById(\"but1\")";
  ptr += "  let submitPin = document.getElementById(\"submitPin\")";
  ptr += "  submitPin.addEventListener(\"click\", ()=> {";
  ptr += "    window.location.replace(\"http://\"+location.hostname+\"/\");";
  ptr += "  })";
  ptr += "</script>";
  ptr += "</html>";

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
