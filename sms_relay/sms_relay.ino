/*
 * author: alibigdeli
 * website: icc-aria.ir
 * linkedin: Ali Bigdeli
 * email: bigdeli.ali3@gmail.com
 */

// including needed ibraries
#include <SoftwareSerial.h>

// creating softwareserial object as GSMA6 or what ever you wanna call it
SoftwareSerial GSMA6(D5, D6); // Pins D5, D6 are used as software serial pins

// needed varibales as global
String incomingData; // for storing incoming serial data
String number = "";
String timeSMS = "";
String message = ""; // A String for storing the message
int relay_pin = D7; // Initialized a pin for relay module
char numbers[5][13];
bool flag = false;
char *n = "+989123456789";
int counterOfNumber = 1;

void setup()
{
  strcpy(numbers[0], n);
  Serial.begin(9600);
  GSMA6.begin(9600);
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, LOW);
  Serial.println("Initializing...");
  delay(1000);
  GSMA6.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  GSMA6.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  GSMA6.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  GSMA6.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
  updateSerial();
}

void loop()
{
  MassageReceiver();
  if (flag) {
    if (incomingData.indexOf("reset") >= 0)
    {
      digitalWrite(relay_pin, HIGH);
      delay(5000);
      digitalWrite(relay_pin, LOW);
      flag = false;
      //message = "Led is turned ON";
      // send_message(message);
    }
  }
}

void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    GSMA6.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (GSMA6.available())
  {
    Serial.write(GSMA6.read());//Forward what Software Serial received to Serial Port
  }
}

void MassageReceiver()
{
  delay(500);
  if (GSMA6.available() > 0)
  {
    incomingData = GSMA6.readString();
    Serial.print(incomingData);
    number = incomingData.substring(incomingData.indexOf("+98"), incomingData.indexOf("+98") + 13);
    timeSMS = incomingData.substring(incomingData.indexOf("+98") + 28, incomingData.indexOf("+98") + 36);
    if (number.indexOf("+98") >= 0) {
      Serial.print("Number Of Sender: ");
      Serial.println(number);
      Serial.print("In Time: ");
      Serial.println(timeSMS);
      for (int i = 0; i < counterOfNumber; i++) {
        bool f = false;

        for (int j = 0; j < 13; j++)if (numbers[i][j] != number[j])f = true;

        if (!f) {
          flag = true;
          break;
        }
      }
    }
    delay(10);
  }
}

void send_message(String message)
{
  GSMA6.println("AT+CMGF=1"); //Set the GSM Module in Text Mode
  delay(100);
  GSMA6.println("AT+CMGS=\"+989123456789\""); // Replace it with your mobile number
  delay(100);
  GSMA6.println(message); // The SMS text you want to send
  delay(100);
  GSMA6.println((char)26); // ASCII code of CTRL+Z
  delay(100);
  GSMA6.println();
  delay(1000);
}
