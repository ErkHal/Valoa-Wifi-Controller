#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <ValoaWifi.h>

/*
###############################################
                                                
    ValoaWifi Controller written by ErkHal      
    for WeMos D1 Mini Pro hardware              
    https://github.com/ErkHal/Valoa-wifi       

###############################################
*/

// Change this to your liking
String hostname = "valoa";
 
/* Set these to your desired credentials. */
const char *ssid = "YOUR_SSID_HERE";
const char *password = "YOUR_WIFI_PASSWORDHERE";

// Set web server port number to 80
WiFiServer server(80);

String header;

// RGB controller init
ValoaWifi valoaController(0, 4, 5);

/*#####################################################
      Helper functions for responding to requests
  #####################################################
*/
void sendOkResponse(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Access-Control-Allow-Origin: *");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  client.println("OK");
  client.println();
}

void sendErrorResponse(WiFiClient client) {
  client.println("HTTP/1.1 500");
  client.println("Access-Control-Allow-Origin: *");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  client.println();
}

/*========================================
                    Power on setup
  ======================================== 
*/
void setup() {

  Serial.begin(115200);


  //Initialize WiFi connection to given credentials
  WiFi.hostname(hostname.c_str());
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); //IP address assigned to your ESP

  /*
    Set up mDNS so that in a LAN network we can connect using hostname
    instead of a IP address
  */
  if(MDNS.begin(hostname.c_str())) {

    Serial.println(String("mDNS responder started for " + hostname + ".local"));
    MDNS.addService("http", "tcp", 80);
    
    server.begin();
    Serial.println("HTTP server started");
    
  } else {
    Serial.println("Error setting up mDNS responder !");
    while(1) {
      delay(1000);
    }
  }
}


void loop() {
  WiFiClient client = server.available();
  StaticJsonBuffer<JSON_BUFFER_CAPACITY>jsonBuffer;

  // A request is received
  if (client) {                             
    Serial.println("New request.");         
    String currentLine = "";       

    while (client.connected()) {            
      if (client.available()) {             
        
        char c = client.read();             
        Serial.write(c);
        
        if (c == '\n') {
          // When this statement is true, it means that the request
          if(currentLine.length() == 0) {
            JsonObject& jsonBody = jsonBuffer.parseObject(client);
            if(jsonBody.success()) {
              Serial.println("Request parsed");
                
              // Read values from JSON body
              int red = atoi(jsonBody["rgb"][0]);
              int green = atoi(jsonBody["rgb"][1]);
              int blue = atoi(jsonBody["rgb"][2]);
              sendOkResponse(client);

              // Write PWM values to RGB pins
              valoaController.setColor(red, green, blue);
              break;

            } else { 
              Serial.println("Request parsing failed");
              sendErrorResponse(client);
              break;
            }
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
  }
}
