/*************************************************************

  You’ll need:
   - Blynk IoT app (download from App Store or Google Play)
   - WeMos D1 board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "BLYNK TEMPLATE ID HERE"
#define BLYNK_DEVICE_NAME "BLYNK DEVICE NAME HERE"
#define BLYNK_AUTH_TOKEN "BLYNK AUTH TOKEN HERE"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SSID HERE";
char pass[] = "PASS HERE";

int pwrSwitchOut = D1;
int rstSwitchOut = D2;
int pwrLEDout = D3;
int hddLEDout = D4;
int hddLEDin = D7;
int pwrLEDin = D8;
int pwrState = 0;
int hddState = 0;
int passthrough = 0;

//handle state change in virtual button V0(power)
BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();
  
  //if virtual button pressed
  if (pinValue == 1)
  {
    Serial.println("Push(Power)");
    //read the state of the power led from the motherboard
    pwrState = digitalRead(pwrLEDin);

    //if the power led is off
    if (pwrState == LOW)
    {
      //activate the power switch octocoupler
      digitalWrite(pwrSwitchOut, HIGH);
      Serial.println("ON(Power)");   
      delay(250);
      //deactivate power switch octocoupler after 500ms delay
      digitalWrite(pwrSwitchOut, LOW);
      Serial.println("Delayed Release(Power)");   
    }
    //if the computer is already turned on, dont activate power switch octocoupler
    else
    {
      digitalWrite(pwrSwitchOut, LOW);
      Serial.println("Blocked(Power)");
    }
  }
  else
  {
    digitalWrite(pwrSwitchOut, LOW);
    Serial.println("Release(Power)");
  }

}

//handle state change in virtual button V2(reset)
BLYNK_WRITE(V2)
{
  int pinValue = param.asInt();

  //if virtual button is pressed
  if (pinValue == 1)
  {
    //activate the reset switch octocoupler
    digitalWrite(rstSwitchOut, HIGH);
    Serial.println("Push(Reset)");
    delay(250);
    digitalWrite(rstSwitchOut, LOW);
    Serial.println("Delayed Release(Reset)");
  }  
  else
  {
    digitalWrite(rstSwitchOut, LOW);
    Serial.println("Release(Reset)");
  }
}

BLYNK_WRITE(V4)
{
  int pinValue = param.asInt();

  if (pinValue == 1)
  {
    passthrough = 1;
  }
  else
  {
    passthrough = 0;
  }
}

//updates the virtual led's
void updateRemoteLEDs()
{
  if (pwrState == HIGH)
  {
    Blynk.virtualWrite(V1,1);
  }
  else
  {
    Blynk.virtualWrite(V1,0);
  }
  if (hddState == HIGH)
  {
    Blynk.virtualWrite(V3,1);
  }
  else
  {
    Blynk.virtualWrite(V3,0);
  }
}

void updateLocalLEDs()
{
  if (passthrough == 1)
  {
    if (pwrState == HIGH)
    {
      digitalWrite(pwrLEDout, HIGH);
    }
    else
    {
      digitalWrite(pwrLEDout, LOW);
    }
    if (hddState == HIGH)
    {
      digitalWrite(hddLEDout, HIGH);
    }
    else
    {
      digitalWrite(hddLEDout, LOW);
    }
  }
  else
  {
    digitalWrite(pwrLEDout, LOW);
    digitalWrite(hddLEDout, LOW);
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(pwrSwitchOut, OUTPUT);
  pinMode(rstSwitchOut, OUTPUT);
  pinMode(pwrLEDout, OUTPUT);
  pinMode(hddLEDout, OUTPUT);
  pinMode(hddLEDin, INPUT);
  pinMode(pwrLEDin, INPUT);

  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!

  //check to see if the power led is on and display its status
  //**need to turn this into function**
  pwrState = digitalRead(pwrLEDin);
  hddState = digitalRead(hddLEDin);
  updateLocalLEDs();
  updateRemoteLEDs();
}