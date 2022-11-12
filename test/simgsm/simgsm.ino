#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <TimeLib.h>
#define time_offset 10800 // Local time is UTC+3, so 10800 is 3hrs in sec, change based on your location

SoftwareSerial SIM900(7, 8);
char incoming_char = 0;
String gMap = "http://www.google.com/maps/place/";
double Latitude, Longitude, Altitude, speed;
boolean GPSstatus = false;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps; // The TinyGPS++ object

char Time[]  = "00:00:00";
char Date[]  = "00-00-2000";
String nameofday;
int Year;
byte last_second , Month, Day, Hour, Minute, Second;
String DateString = "";

void SIM900power()
{
  pinMode(9, OUTPUT); 
  digitalWrite(9,LOW);
  delay(1000);
  digitalWrite(9,HIGH);
  delay(2000);
  digitalWrite(9,LOW);
  delay(3000);
}

void setup() {
  SIM900power();
  SIM900.begin(9600); // for GSM shield
  Serial.begin(9600); // For serial monitor<br>  delay(20000);  // Give time to log on to network.<br>  SIM900.print("AT+CLIP=1\r"); // turn on caller ID notification
  delay(100);
}

void loop() {

  while (Serial.available() > 0)
    if (gps.encode(Serial.read()))
      getGPSdata();

  // Display any text that the GSM shield sends out on the serial monitor
  if (SIM900.available() > 0) {
    // Get the character from the cellular serial port
    // With an incomming call, a "RING" message is sent out
    incoming_char = SIM900.read();
    //  Check if the shield is sending a "RING" message
    if (incoming_char == 'R') {
      delay(10);
      Serial.print(incoming_char);
      incoming_char = SIM900.read();
      if (incoming_char == 'I') {
        delay(10);
        Serial.print(incoming_char);
        incoming_char = SIM900.read();
        if (incoming_char == 'N') {
          delay(10);
          Serial.print(incoming_char);
          incoming_char = SIM900.read();
          if (incoming_char == 'G') {
            delay(10);
            Serial.print(incoming_char);
            // If the message received from the shield is RING
            // Wait till call is confirmed received
            delay(1000);
            SIM900.println("ATH"); // hang up
            if (Latitude + Longitude == 0) {
              No_GPS_Message();
            }
            else {
              Valid_GPS_Message();
            }
          }
        }
      }
    }
  }
}
void Valid_GPS_Message()
{
  dateString(); //include time stamp
  SIM900.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  
  SIM900.println("AT+CMGS=\"+6285809125465\"\r"); // Replace x with mobile number+Country Code
  delay(1000);
  SIM900.println(dateString()); 
  delay(100);
  SIM900.print(gMap); 
  delay(100);
  SIM900.print(Latitude, 6);// 6 digit has to be defined, otherwaise only 2 digit is displayed
  delay(100);
  SIM900.print(",");
  delay(100);
  SIM900.println(Longitude, 6);// 6 digit has to be defined, otherwaise only 2 digit is displayed
  delay(100);
  SIM900.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void No_GPS_Message()
{
  SIM900.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000); 
  SIM900.println("AT+CMGS=\"+6285809125465\"\r"); // Replace x with mobile number
  delay(1000);
  SIM900.println("GPS DATA NOT AVAILABLE");
  delay(100);
  SIM900.println("PLEASE TRY LATER");
  delay(100);
  SIM900.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
void getGPSdata() {
  if (gps.location.isValid()) {
    Latitude = gps.location.lat(), 6; // Latitude in degrees (double)
    Longitude = gps.location.lng(), 6; // Longitude in degrees (double)
    GPSstatus =  true;
  }
  else
    GPSstatus =  false;
  if (gps.date.isValid()) {
    Year = gps.date.year(); // Year (2000+) (u16)
    Month = gps.date.month(); // Month (1-12) (u8)
    Day = gps.date.day(); // Day (1-31) (u8)
  }
  else
    GPSstatus =  false;

  if (gps.time.isValid()) {
    Hour = gps.time.hour(); // Hour (0-23) (u8)
    Minute = gps.time.minute(); // Minute (0-59) (u8)
    Second = gps.time.second(); // Second (0-59) (u8)
  }
  else
    GPSstatus =  false;

  if (last_second != gps.time.second()) {
    last_second = gps.time.second();
    // set current UTC time
    setTime(Hour, Minute, Second, Day, Month, Year);
    // add the offset to get local time
    adjustTime(time_offset);
    // update time array
    Time[6] = second() / 10 + '0';
    Time[7] = second() % 10 + '0';
    Time[3]  = minute() / 10 + '0';
    Time[4] = minute() % 10 + '0';
    Time[0]  = hour()   / 10 + '0';
    Time[1]  = hour()   % 10 + '0';
    // update date array
    Date[8] = (year()  / 10) % 10 + '0';
    Date[9] =  year()  % 10 + '0';
    Date[3]  =  month() / 10 + '0';
    Date[4] =  month() % 10 + '0';
    Date[0]  =  day()   / 10 + '0';
    Date[1]  =  day()   % 10 + '0';
  }
  switch (weekday())  {
    case 1:  nameofday = "SUNDAY, ";   break;
    case 2:  nameofday = "MONDAY, ";   break;
    case 3:  nameofday = "TUESDAY, ";   break;
    case 4:  nameofday = "WEDNESDAY, ";   break;
    case 5:  nameofday = "THURSDAY, ";   break;
    case 6:  nameofday = "FRIDAY, ";   break;
    default: nameofday = "SATURDAY, ";
  }
  if (gps.speed.isValid()) {
    speed = gps.speed.mps(); // Speed in meters per second (double)
  }
  else
    GPSstatus =  false;

  if (gps.altitude.isValid()) {
    Altitude = gps.altitude.meters(); // Altitude in meters (double)
  }
  else
    GPSstatus =  false;

  delay(1000);
}

String dateString() {
  DateString = "";
  DateString += nameofday;
  DateString += " ";
  DateString += Date;
  DateString += " ";
  DateString += Time;
  return DateString;
}
