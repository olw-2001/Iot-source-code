#include <SPI.h> 
#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <SoftwareSerial.h>

// HC-06 Module RXD and TXD pins, pin 0 and 1
SoftwareSerial BTserial(0, 1); // RX | TX

int sensorPin = A0;
int sensorValue = 0;

const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor
int ledPin = 2;

char ssid[] = "SKYV6Q2U";  
char pass[] = "GXp9BFNVfVxA";
const unsigned long channel_id = "1600308";
const char write_api_key[] = "8VAISG6CARMNG5D4";
int status = WL_IDLE_STATUS;
WiFiClient client;

void setup() {
  BTserial.begin(9600);
  Serial.begin(9600); 
   // attempt to connect using WPA2 encryption: 
  Serial.print("Attempting to connect to ");
  Serial.println(ssid);
  status = WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected.");
  ThingSpeak.begin(client);
}

void loop() {
  long duration, inches, cm;
  pinMode(ledPin, OUTPUT);
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(100);
  
  sensorValue = analogRead(sensorPin);
  //IMPORTANT: The complete String has to be of the Form: 1234,1234,1234,1234;
  //Each line has to be separate, and separated by a comma
  BTserial.print(cm);
  BTserial.println(" cm");
  delay(20);

  if(cm <= 50) {
    Serial.print("Someone is at the door");
    BTserial.println("Someone is at the door");
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
    Serial.print("");
    BTserial.println("Clear");
  }

  ThingSpeak.setField(1, cm);
  ThingSpeak.writeFields(channel_id, write_api_key);
  delay(500);
}
  long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
