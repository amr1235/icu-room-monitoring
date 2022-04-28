#include <ESP8266WiFi.h>
//#include <SoftwareSerial.h>
#include <Arduino.h>
#include <string.h>
#include "DHT.h"
#include <WebSocketsClient.h>
//#include <ESP8266WiFi.h>
//SoftwareSerial NodeMCU(D2,D3);
WebSocketsClient webSocket;
int mode = -1; 

unsigned long messageInterval = 1000;
bool connected = false;
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_TEXT:

            if(!strcmp((char *)payload,"room1")) 
            mode = 157;

            if(!strcmp((char *)payload,"room2")) 
            mode = 134;

            break; 
        case WStype_BIN:
            hexdump(payload, length);
            break;
    }
 
}
char char_array_user[255];
char char_array_pass[255];
char *data[]={char_array_user,char_array_pass};
char* serial_tochar(int choose_data) {
    while(Serial.available()==0) { }
    String str =Serial.readString();
    str.toCharArray(data[choose_data], str.length());
    return data[choose_data];
}

void connect_wifi() {
//    char * username;
//    Serial.println("Please enter the username: ");
//    username = strtok(serial_tochar(0), " ");
//    char * password;
////    Serial.println("Please enter the password: ");
//    password = strtok(serial_tochar(1), " ");
    WiFi.begin("artys", "as12f463y8hs");

    uint8_t i = 0;
    while(WiFi.status() != WL_CONNECTED && i < 20) {
      Serial.print(".");
      delay(500);
      i++;
    }
}

void print_networks() {
    int networks_number = WiFi.scanNetworks();
    for(int i=0;i<networks_number;i++) 
    {
      Serial.print("ssid: ");
      Serial.print(WiFi.SSID(i));
      Serial.print(", strength: ");
      Serial.print(WiFi.RSSI(i));
      Serial.print("\n");
    }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400); // Default communication rate of the Bluetooth module

for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("Pease wait for %d seconds\n", t);
        Serial.flush();
        delay(1000);
    }

    while(WiFi.status() !=  WL_CONNECTED )
    {
        print_networks();
        connect_wifi();
    }
    Serial.print("Local IP: "); Serial.println(WiFi.localIP());
    // server address, port and URL
    //webSocket.begin("powerful-lake-18491.herokuapp.com", 80, "/sensor","text");
    webSocket.begin("192.168.1.15", 3000, "/sensor","text");
    // event handler
    webSocket.onEvent(webSocketEvent);

}

unsigned long lastUpdate = millis();
String str;
void loop() {
    webSocket.loop();
  if (lastUpdate+messageInterval<millis()){
  Serial.println("before if");
  if(Serial.available() > 0){ // Checks whether data is comming from the serial port
    Serial.println("inside if");
    while(Serial.read() != '!') {};
    str = Serial.readStringUntil('$');
    Serial.println(str);
    Serial.println("after if");
    webSocket.sendTXT(str);
  }

  Serial.write(mode);

  lastUpdate = millis();
  }

}
