#define BLYNK_TEMPLATE_ID "TMPLgihD0BWQ"
#define BLYNK_DEVICE_NAME "TEST ULTRASONIC"
#define BLYNK_AUTH_TOKEN "qDgRws8jt0MGBRkSBWNJunAaje6kcyQq"

#include <HCSR04.h>
#include <Servo.h>

Servo myservo;

int irsensor = 4; // irsensor

int valueirsensor = 0;

int totallevel = 22;

int valuelevel = 0;

int valueultrasonic = 0;

int state = 0;

HCSR04 hc(5,6); // (trig, echo)

#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

// Hardware Serial on Mega, Leonardo, Micro...
//#define EspSerial Serial1

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

BlynkTimer timer;

void myTimerEvent()
{
  valueirsensor = digitalRead(irsensor);
  
  Serial.print("sensor 1 = "); // untuk bukak pintu
  Serial.println(hc.dist());

  if (valueirsensor == LOW){
    Serial.println ("pintu bukak");
    myservo.write(100);
    }
  else {
    Serial.println("pintu tutup");
    myservo.write(0);
    }

  valueultrasonic = hc.dist();

  valuelevel = totallevel - valueultrasonic;

  valuelevel = map(valuelevel, 0, 22, 0, 100);

  if (valuelevel >= 90 && state == 0){
    Blynk.logEvent("level_sampah", "sampah penuh buang skrg");
    state = 1;
    }

  if (valuelevel <= 90 && state == 1){
    state = 0;
    }
  
  Blynk.virtualWrite(V1, valuelevel);
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(irsensor, INPUT);
  myservo.attach(9);
  myservo.write(0);
  delay(10);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);

  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
}
