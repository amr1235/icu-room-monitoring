
#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);
void setup() {
  Serial.begin(38400); // Default communication rate of the Bluetooth module
    pinMode(6, OUTPUT); // Declare the LED as an output
  pinMode(7, OUTPUT); // Declare the LED as an output
  dht.begin();

}
void loop() {
  int h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  int t = dht.readTemperature();
  
 // if(Serial.available() > 0){ // Checks whether data is comming from the  port
 //    state = Serial.read(); // Reads the dserialata from the serial por
 //}
    int state=0;
    if(Serial.available() > 0)
    { // Checks whether data is comming from the serial port
        state = Serial.read(); // Reads the data from the serial port
        if(state==157){
           digitalWrite(6, HIGH); // Turn the LED on
           digitalWrite(7, LOW); // Turn the LED on    
        }
        else if(state==134){
      
           digitalWrite(6, LOW); // Turn the LED on
           digitalWrite(7, HIGH); // Turn the LED on    
        }
    }
   char str[200];
   strcpy(str, "!{");
   strcat(str, "\"");
   strcat(str, "hum");
   strcat(str, "\"");
   strcat(str, ":");
   strcat(str,String(h).c_str());
   strcat(str, ",");

   strcat(str, "\"");
   strcat(str, "temp");
   strcat(str, "\"");
   strcat(str, ":");
   strcat(str,String(t).c_str());
   strcat(str, "}");
   strcat(str, "$");
   Serial.write(str);
   Serial.flush();
  
}
