# weather_station
Weather station for arduino.

It can be used to measure:
* Pressure
* Temperature
* Humidity
* Light intensity

It sends all data by WiFi to a database, in this case Influxdb.
Graphana can be used to visualize the measurements. 

## Components

* Wifi module: ESP8266
* Pressure sensor : BMP180
* Temperature and humidity: DTH22
* Light sensor: TSL237

## Links

[wifi_tutorial_spa_1](http://kio4.com/arduino/57modulowifi_2.htm)

[wifi_tutorial_spa_2](http://www.prometec.net/arduino-wifi/)
