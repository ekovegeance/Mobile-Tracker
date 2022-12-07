#include <SoftwareSerial.h>
static const int RXPinGSM = 5, TXPinGSM = 6; //pin GSM
SoftwareSerial ssGSM(RXPinGSM, TXPinGSM);
char msg;
char call;

void setup()
{
  ssGSM.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  Serial.println("GSM SIM900A BEGIN");
  Serial.println("Enter character for control option:");
  Serial.println("h : to disconnect a call");
  Serial.println("i : to receive a call");
  Serial.println("s : to send message");
  Serial.println("c : to make a call");  
  Serial.println("e : to redial");
  Serial.println();
  delay(100);
}

void loop()
{  
  if (Serial.available()>0)
   switch(Serial.read())
  {
    case 's':
      SendMessage();
      break;
    case 'c':
      MakeCall();
      break;
    case 'h':
      HangupCall();
      break;
    case 'e':
      RedialCall();
      break;
    case 'i':
      ReceiveCall();
      break;
  }
 if (ssGSM.available()>0)
 Serial.write(ssGSM.read());
}

void SendMessage()
{
  ssGSM.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  ssGSM.println("AT+CMGS=\"+6285809125465\"\r"); // Replace x with mobile number
  delay(1000);
  ssGSM.println("Mobile Tracker");// The SMS text you want to send
  delay(100);
   ssGSM.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void ReceiveMessage()
{
  ssGSM.println("AT+CNMI=2,2,0,0,0"); // AT Command to recieve a live SMS
  delay(1000);
  if (ssGSM.available()>0)
  {
    msg=ssGSM.read();
    Serial.print(msg);
  }
}

void MakeCall()
{
  ssGSM.println("ATD+6285809125465;"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
  Serial.println("Calling  "); // print response over serial port
  delay(1000);
}


void HangupCall()
{
  ssGSM.println("ATH");
  Serial.println("Hangup Call");
  delay(1000);
}

void ReceiveCall()
{
  ssGSM.println("ATA");
  delay(1000);
  {
    call=ssGSM.read();
    Serial.print(call);
  }
}

void RedialCall()
{
  ssGSM.println("ATDL");
  Serial.println("Redialing");
  delay(1000);
}
