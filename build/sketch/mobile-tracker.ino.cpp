#include <Arduino.h>
#line 1 "/home/me/git/Mobile-Tracker/mobile-tracker/mobile-tracker.ino"
#line 1 "/home/me/git/Mobile-Tracker/mobile-tracker/mobile-tracker.ino"
// Mobile Tracker
#include <TinyGPSPlus.h>
#include <TinyGPS++.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);


static const int RXPinGPS = 4, TXPinGPS = 3; //pin GPS
static const int RXPinGSM = 5, TXPinGSM = 6; //pin GSM
static const uint32_t GPSBaud = 9600;
static const uint32_t GSMBaud = 9600;
// The TinyGPS++ object
TinyGPSPlus gps;

int temp=0,i;
// The serial connection to the GPS device
SoftwareSerial ssGPS(RXPinGPS, TXPinGPS);
// GSM
SoftwareSerial ssGSM(RXPinGSM, TXPinGSM);
String stringVal = "";
#line 23 "/home/me/git/Mobile-Tracker/mobile-tracker/mobile-tracker.ino"
void setup();
#line 53 "/home/me/git/Mobile-Tracker/mobile-tracker/mobile-tracker.ino"
void loop();
#line 74 "/home/me/git/Mobile-Tracker/mobile-tracker/mobile-tracker.ino"
void serialEvent();
#line 89 "/home/me/git/Mobile-Tracker/mobile-tracker/mobile-tracker.ino"
void gsm_init();
#line 144 "/home/me/git/Mobile-Tracker/mobile-tracker/mobile-tracker.ino"
void init_sms();
#line 151 "/home/me/git/Mobile-Tracker/mobile-tracker/mobile-tracker.ino"
void send_data(String message);
#line 156 "/home/me/git/Mobile-Tracker/mobile-tracker/mobile-tracker.ino"
void send_sms();
#line 160 "/home/me/git/Mobile-Tracker/mobile-tracker/mobile-tracker.ino"
void lcd_status();
#line 169 "/home/me/git/Mobile-Tracker/mobile-tracker/mobile-tracker.ino"
void tracking();
#line 23 "/home/me/git/Mobile-Tracker/mobile-tracker/mobile-tracker.ino"
void setup(){
  Serial.begin(9600);
  ssGPS.begin(GPSBaud);
  ssGSM.begin(GSMBaud);
  lcd.init();
  lcd.backlight();
  lcd.begin(16,2);

  lcd.setCursor(1, 0);
  lcd.print("Mobile Tracker");
  lcd.setCursor(0,1);
  lcd.print("    System      ");
  delay(2000);
  gsm_init();
  lcd.clear();  
  ssGSM.println("AT+CNMI=2,2,0,0,0");
  lcd.print("GPS Initializing");
  lcd.setCursor(0,1);
  lcd.print("  No GPS Range  "); 
  delay(2000);
  lcd.clear();
  lcd.print("GPS Range Found");
  lcd.setCursor(0,1);
  lcd.print("GPS is Ready");
  delay(2000);
  lcd.clear();
  lcd.print("System Ready");
  temp=0;
}

void loop()
{ 
serialEvent();

      while(temp)
      {
        while (ssGPS.available() > 0)
        {
          gps.encode(ssGPS.read());
                if (gps.location.isUpdated())
                {
                 temp=0;
                 
                 tracking();
                } 
          if(!temp)
          break;  
        }
      }
      
}
void serialEvent()
{
  while(Serial.available()>0)
  { 
    if(Serial.find("mt"))
    {
      temp=1;
      break;
    }
    else
    {
    temp=0;
    }
  }
}
void gsm_init()
{
  lcd.clear();
  lcd.print("Finding Module..");
  boolean at_flag=1;
  while(at_flag)
  {
//    ssGSM.println("AT+CMGF=1");
//    delay(1);
    while(ssGSM.available()>0)
    {
      if(Serial.find("OK"))
      at_flag=0;
    }
    
    delay(1000);
  }
  lcd.clear();
  lcd.print("Module Connected..");
  delay(1000);
  lcd.clear();
  lcd.print("Disabling ECHO");
  boolean echo_flag=1;
  while(echo_flag)
  {
    ssGSM.println("ATE0");
    while(ssGSM.available()>0)
    {
      if(Serial.find("OK"))
      echo_flag=0;
    }
    delay(1000);
  }
  lcd.clear();
  lcd.print("Echo OFF");
  delay(1000);
  lcd.clear();
  lcd.print("Finding Network..");
  boolean net_flag=1;
  while(net_flag)
  {
    ssGSM.println("AT+CPIN?");
    while(ssGSM.available()>0)
    {
      if(ssGSM.find("+CPIN: READY"))
      net_flag=0;
    }
    delay(1000);
  }
  lcd.clear();
  lcd.print("Network Found..");
  
  delay(1000);
  lcd.clear();
}
void init_sms()
{
  ssGSM.println("AT+CMGF=1");
  delay(400);
  ssGSM.println("AT+CMGS=\"+6285809125465\"");   // use your 10 digit cell no. here
  delay(400);
}
void send_data(String message)
{
  Serial.print(message);
 delay(200);
}
void send_sms()
{
  Serial.write(26);
}
void lcd_status()
{
  lcd.clear();
  lcd.print("Message Sent");
  delay(2000);
  lcd.clear();
  lcd.print("System Ready");
  return;
}
void tracking()
{
    init_sms();
    send_data("Mobile Tracker Alert:");
    Serial.println(" "); 
    send_data("Your Device Current Location is:");
    Serial.println(" "); 
    Serial.print("Latitude: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print("\n Longitude: ");
    Serial.println(gps.location.lng(), 6);

   // https://www.google.com/maps/@8.2630696,77.3022699,14z
   Serial.print("https://www.google.com/maps/@");
   Serial.print(gps.location.lat(), 6);
   Serial.print(',');
   Serial.print(gps.location.lng(), 6);
   Serial.print(",14z");
    send_sms();
    delay(2000);
    lcd_status();
}

