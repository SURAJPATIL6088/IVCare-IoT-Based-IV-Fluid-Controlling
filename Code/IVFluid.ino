#define BLYNK_TEMPLATE_ID "TMPL3UCUopjVH"
#define BLYNK_TEMPLATE_NAME "ServoControl"
#define BLYNK_AUTH_TOKEN "j4J9mZU9xGBJ6EtVQ0bEkMUMnxK6iI2z"

#include <ESP32Servo.h>
#include "HX711.h"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <OneWire.h>
#include <DallasTemperature.h>

char auth[] = BLYNK_AUTH_TOKEN;
// char ssid[] = "POCO X3 Pro";
// char pass[] = "atharva67890";
// char ssid[] = "George w Bush";
// char pass[] = "parth123";
// char ssid[] = "Vivo";
// char pass[] = "huhj6088";

Servo myServo;
int servoPin = 18;
const int LOADCELL_DOUT_PIN = 23;
const int LOADCELL_SCK_PIN = 19;
HX711 scale;

// Change this to the desired GPIO pin
#define ONE_WIRE_BUS 5 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Blynk slider widget on Virtual Pin V0
BLYNK_WRITE(V1) { 
  int angle = param.asInt();
  myServo.write(angle);
}

void setup() {
  myServo.attach(servoPin);
  Serial.begin(115200);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(407);
  scale.tare();
  sensors.begin();
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
  
  // Read and print weight every second
  Serial.print("Weight: ");
  float weight = scale.get_units(10);
  Serial.println(weight, 1);
  Blynk.virtualWrite(V4, weight); // Send weight to Blynk Gauge Widget on V2
  
  // Read and print temperature every second
  sensors.requestTemperatures();

  float temperatureC = sensors.getTempCByIndex(0);
  if (temperatureC != DEVICE_DISCONNECTED_C) 
  {
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.println(" °C");
    Blynk.virtualWrite(V5, temperatureC); // Send temperature to Blynk Display Widget on V3
  } 
  else 
  {
    Serial.println("Error reading temperature!");
  }
  
  // Check if there is input for the servo
  if (Serial.available()) 
  {
    int angle = Serial.parseInt();
    myServo.write(angle);
  }
  
  // Wait for 1 second (1000 milliseconds)
  delay(1000); 
}
