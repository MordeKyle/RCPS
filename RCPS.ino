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
int pwrLEDin = D2;
int pwrState = 0;

//handle state change in virtual button V0
BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();
  
  //if virtual button pressed
  if (pinValue == 1)
  {
    Serial.println("Push");
    //read the state of the power led from the motherboard
    pwrState = digitalRead(pwrLEDin);

    //if the power led is off
    if (pwrState == LOW)
    {
      //activate the power switch octocoupler
      digitalWrite(pwrSwitchOut, HIGH);
      Serial.println("ON");   
      delay(500);
      //deactivate power switch octocoupler after 500ms delay
      digitalWrite(pwrSwitchOut, LOW);
      Serial.println("Delayed Release");   
    }
    //if the computer is already turned on, dont activate power switch octocoupler
    else
    {
      digitalWrite(pwrSwitchOut, LOW);
      Serial.println("OFF");
    }
  }
  else
  {
    Serial.println("Release");
    digitalWrite(pwrSwitchOut, LOW);
  }

}

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(pwrSwitchOut, OUTPUT);
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

  if (pwrState == HIGH)
  {
    Blynk.virtualWrite(V1,1);
    //Serial.println("ON");
  }
  else
  {
    Blynk.virtualWrite(V1,0);
    //Serial.println("OFF");
  }
}