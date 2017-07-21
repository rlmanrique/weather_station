#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2

#define Celsius 0
#define Fahr    1

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
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");
  // Start up the library
  tsensors.begin();
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
    
  delay(5000);
} 
