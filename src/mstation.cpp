#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2

#define Celsius 0
#define Fahr    1


// Set up Bluetooth device RX and TX
SoftwareSerial btsensor(10, 11);

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to Dallas Temperature.
DallasTemperature tsensors(&oneWire);

float readTemp(int unit)
{
  tsensors.requestTemperatures();
  
  switch(unit){
  case Celsius:
    return tsensors.getTempCByIndex(0);
  case Fahr:
    return tsensors.getTempFByIndex(0);
  default:
    return -273;
  }
}

void setup(void)
{

  /* Prepare pins for BT  */
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(9,HIGH);
  delay(500); //TODO:review this delay
 
  // start serial port
  Serial.begin(38400);
  Serial.println("Dallas Temperature IC Control Library Demo");
  // Start up the library
  tsensors.begin();
  Serial.println("Start BT module");
  digitalWrite (8, HIGH);
  btsensor.begin(38400);
}

void loop(void)
{
  float temp;

  temp = readTemp(Celsius);
  if (temp == -273) {
    Serial.println("Temperature is pretty cold. Check the sensor!");
  }
  Serial.print("Temperature is: ");
  Serial.println(temp);
  if (btsensor.available()) {
      Serial.println("BT sensor is on");
    }
  if (!btsensor.available()) {
    Serial.println("BT not available");
  }
  if (btsensor.available())
    Serial.write(btsensor.read());
  if (Serial.available())
    btsensor.write(Serial.read());
  
  delay(1000);
}
