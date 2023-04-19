#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

/********************************************
 －－－－深圳市晶彩智能有限公司－－－－
  File name：40.WIFI Web Servers LED.ino
  Version：V2.0
  Illustrate：WIFI Web Servers Two-color LED light experiment
 ********************************************/
#include <WiFi.h>

// Enter the WIFI name and password that you can access to the Internet. 
//It is recommended to use an Android phone for web control to ensure that the ESP32 is in the same network.
const char* ssid = "Jesus Love(2.4GHz)";
const char* password = "no-87757045";

// Set the web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliary variable for storing the current output state
String LedRedState = "off";
String LedGreenState = "off";
String LedBlueState = "off";

// Assign output variables to GPIO pins
#define bootMode    0
#define LedR        4
#define LedG        16
#define LedB        17

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define the timeout in milliseconds (eg: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize output variable to output
  pinMode(bootMode, INPUT);
  
  pinMode(LedR, OUTPUT);
  pinMode(LedG, OUTPUT);
  pinMode(LedB, OUTPUT);

  // Set output to HIGH
  digitalWrite(LedR, HIGH);
  digitalWrite(LedG, HIGH);
  digitalWrite(LedB, HIGH);

  // Connect to Wi-Fi network using SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print the local IP address and start the web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Monitor clients

  if (client) {                             // If a new client connects
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // Print a message in the serial port
    String currentLine = "";                // Create a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // Loop while client connects
      currentTime = millis();
      if (client.available()) {             // If you want to read bytes from the client
        char c = client.read();             // Then read a byte
        Serial.write(c);                    // Print out on serial monitor
        header += c;
        if (c == '\n') {                    // If the byte is a newline
          // If the current line is empty, there are two newlines on a line.
          // This is the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // Then the content-type, so the client knows what to expect, followed by the empty line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turn gpio on and off
            if (header.indexOf("GET /LedR/on") >= 0) {
              Serial.println("LED Red ON");
              LedRedState = "on";
              digitalWrite(LedR, LOW);
            } 
            if (header.indexOf("GET /LedR/off") >= 0) {
              Serial.println("LED Red OFF");
              LedRedState = "off";
              digitalWrite(LedR, HIGH);

            }
            if (header.indexOf("GET /LedG/on") >= 0) {
              Serial.println("LED Green ON");
              LedGreenState = "on";
              digitalWrite(LedG, LOW);
            }
            if (header.indexOf("GET /LedG/off") >= 0) {
              Serial.println("LED Green OFF");
              LedGreenState = "off";
              digitalWrite(LedG, HIGH);
            }
            if (header.indexOf("GET /LedB/on") >= 0) {
              Serial.println("LED Blue ON");
              LedBlueState = "on";
              digitalWrite(LedB, LOW);
            }
            if (header.indexOf("GET /LedB/off") >= 0) {
              Serial.println("LED Blue OFF");
              LedBlueState = "off";
              digitalWrite(LedB, HIGH);
            }
            
            // Display HTML pages
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            // CSS to style the on/off button
            // Feel free to change the background color and font size properties to suit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".buttonRoff { background-color: #555555; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".buttonRon {background-color: #AF4C50;}</style></head>");

            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".buttonGoff { background-color: #555555; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".buttonGon {background-color: #4CAF50;}</style></head>");
            
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".buttonBoff { background-color: #555555; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");        
            client.println(".buttonBon {background-color: #4C50AF;}</style></head>");
            
            // Page title
            client.println("<body><h1>ESP32 Web Server LED </h1>");

            // Display current status and ON/OFF button of GPIO 16
            client.println("<p>RED LED - State " + LedRedState + "</p>");

            // If output26State is off, show ON button
            if (LedRedState=="off") {
              client.println("<p><a href=\"/LedR/on\"><button class=\"buttonRoff\">OFF</button></a></p>");
            } else {
              client.println("<p><a href=\"/LedR/off\"><button class=\"buttonRoff buttonRon\">ON</button></a></p>");
            } 

            // Display current status and ON/OFF button of GPIO 16
            client.println("<p>GREEN LED - State " + LedGreenState + "</p>");

            // If output26State is off, show ON button
            if (LedGreenState=="off") {
              client.println("<p><a href=\"/LedG/on\"><button class=\"buttonGoff\">OFF</button></a></p>");
            } else {
              client.println("<p><a href=\"/LedG/off\"><button class=\"buttonGoff buttonGon\">ON</button></a></p>");
            } 
               
            // Display current status and ON/OFF button of GPIO 17
            client.println("<p> BLUE LED - State " + LedBlueState + "</p>");

            // If output26State is off, show ON button
            if (LedBlueState=="off") {
              client.println("<p><a href=\"/LedB/on\"><button class=\"buttonBoff\">OFF</button></a></p>");
            } else {
              client.println("<p><a href=\"/LedB/off\"><button class=\"buttonBoff buttonBon\">ON</button></a></p>");
            }
            client.println("</body></html>");
            
            // HTTP response ends with another blank line
            client.println();
            // Out of the while loop
            break;
          } else { // If you have a newline then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // If you get characters other than carriage return
          currentLine += c;      // Add it to the end of currentLine
        }
      }
    }
    // Clear header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}