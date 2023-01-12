//Library
#include <SoftwareSerial.h>
#include <TinyGPS++.h>


// Alias & Serial Port
TinyGPSPlus gps;
SoftwareSerial gsm(6,7); //RX dan TX


String response;
int lastStringLength = response.length();
String link;
double latitude, longitude;

 
void setup() {
  Serial.begin(9600);
  Serial.println("GPS Mulai");
 
    gsm.begin(9600);  
    gsm.println("AT+CMGF=1");
    Serial.println("GSM started at 9600");
    delay(1000);
    Serial.println("Setup Complete! GSM is Ready!");
    Serial.print(link);
    gsm.println("AT+CNMI=2,2,0,0,0");
 
}
 
void loop() {
 
  if (gsm.available()>0){
      response = gsm.readStringUntil('\n');
    }
     
 
  if(lastStringLength != response.length()){
      GPS();
      //Perintah ON
      if(response.indexOf("mt")!=-1){ 
         
          gsm.println("AT+CMGF=1");    
          delay(1000); 
          gsm.println("AT+CMGS=\"+6285809125465\"\r"); 
          delay(1000);          
          gsm.println(link);
          delay(1000);
          gsm.println((char)26);
          delay(1000);
      }
  }
  Serial.print(link);  
}

// Function GPS
void GPS(){
  if(Serial.available()) {
    gps.encode(Serial.read());
  }
  if(gps.location.isUpdated()) {
    latitude = gps.location.lat();
    longitude = gps.location.lng();
    link = "maps.google.com/maps?q=" + String(latitude, 6) + "," + String(longitude, 6);
    Serial.println(link);
  }
  
}
