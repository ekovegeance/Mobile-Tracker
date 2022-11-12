#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPinGPS = 4, TXPinGPS = 3; //pin GPS
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ssGPS(RXPinGPS, TXPinGPS);

void setup(){
  Serial.begin(9600);
  ssGPS.begin(GPSBaud);
}

void loop(){
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ssGPS.available() > 0){
    gps.encode(ssGPS.read());
    if (gps.location.isUpdated()){
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6);
    }
  }
}
