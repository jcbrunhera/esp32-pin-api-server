#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
  
const char *SSID = "your_ssid";
const char *PWD = "your_pswd";
 
WebServer server(80);  

void connectToWiFi() {
  
  // -- fixed IP 
  IPAddress local_IP(192, 168, 15, 36);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 0, 0);
  IPAddress primaryDNS(8, 8, 8, 8);   //optional
  IPAddress secondaryDNS(8, 8, 4, 4); //optional
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  // -------  fixed IP
  
  Serial.print("Connecting to ");
  Serial.println(SSID);  
  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  } 
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}

void setup_routing() {     
  server.on("/GetPinValue", GetPinValue);     
  server.on("/SetPinValue", HTTP_POST, SetPinValue);    
  server.begin();    
}

void GetPinValue() {
  Serial.println("getPinValue");
  int pinNumber = server.arg("pinNumber").toInt();
  char str[250];
  float pinValue = analogRead(pinNumber);
  sprintf(str, "{ 'pinNumber': %i, 'Value': %f }", pinNumber, pinValue);
  server.send(200,"application/json",str);
  Serial.println(str);
}
  

void SetPinValue() {
  Serial.println("SetPinValue");
  int pinNumber = server.arg("pinNumber").toInt();
  int pinValue = server.arg("pinValue").toInt();
  char str[250];
  sprintf(str, "{ 'RespCode':0, 'pinNumber': %i, 'Value': %i }", pinNumber, pinValue);
  server.send(200,"application/json",str);
  Serial.println(str);
  pinMode(pinNumber,OUTPUT);
  digitalWrite( pinNumber, pinValue);
}

void setup() {     
  Serial.begin(115200);         
  connectToWiFi();     
  setup_routing();     
}    
       
void loop() {    
  server.handleClient();     
}

