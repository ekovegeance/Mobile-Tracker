#include <TinyGPSPlus.h>
#include <TinyGPS++.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
// The TinyGPS++ object
TinyGPSPlus gps;

int temp=0,i;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
// GSM
SoftwareSerial gsm(9, 10);
String stringVal = "";
void setup(){
  Serial.begin(9600);
  ss.begin(GPSBaud);
  gsm.begin(19200);
    lcd.init();
  lcd.backlight();
  lcd.begin(16,2);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);

  lcd.setCursor(1, 0);
  lcd.print("Vehicle Tracking");
  lcd.setCursor(0,1);
  lcd.print("    System      ");
  delay(2000);
  gsm_init();
  lcd.clear();  
  gsm.println("AT+CNMI=2,2,0,0,0");
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
        while (ss.available() > 0)
        {
          gps.encode(ss.read());
                if (gps.location.isUpdated())
                {
                 temp=0;
                 digitalWrite(13,HIGH);
                 tracking();
                } 
          if(!temp)
          break;  
        }
      }
      digitalWrite(13,LOW);
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
    gsm.println("AT");
    delay(1);
    while(gsm.available()>0)
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
    gsm.println("ATE0");
    while(gsm.available()>0)
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
    gsm.println("AT+CPIN?");
    while(gsm.available()>0)
    {
      if(gsm.find("+CPIN: READY"))
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
  gsm.println("AT+CMGF=1");
  delay(400);
  gsm.println("AT+CMGS=\"3151396616\"");   // use your 10 digit cell no. here
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
    send_data("Vehicle Tracking Alert:");
    Serial.println(" "); 
    send_data("Your Vehicle Current Location is:");
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
