#include <Arduino.h>
#include <SoftwareSerial.h>
#include <string.h>
//#include "DHT.h"
//#include "Wire.h"
//include <BMP180.h>
//#include <MAX44009.h>


#define DHTPIN 7   // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

SoftwareSerial wifi(2,3);
DHT dht(DHTPIN, DHTTYPE);
MAX44009 light;
BMP180 barometer;

void sendWifi(char *msg, long tout, long toutS)
{
   long t0, t;
   int c;
   
  wifi.write(msg);
  t0=millis()+tout;
  
  while(1) {
    t=millis();
    if (t>t0){
      break;
    }
    c=wifi.read();
    if (c!=-1){
     t0=t+toutS;
     Serial.write(c);
    }
  }
  
}


char * flo2str(char *a, float f, int prec)
{
  char *ret = a;
  char *sig = "";
  char format[12];
  long l;
  long e;
  long po10 = 1;

  if (prec<0){
    prec = -prec;
    sig = " ";
  }
  if (f < 0.0) {
    f = -f;
    sig = "-";
  }
  sprintf (format, "%s%%ld.%%0%dld", sig, prec);
  for (; prec > 0; prec--) {
    po10 *= 10;
  }
  l = (long) (f * (float) po10 + 0.5);
  e = l / po10;
  l = l - e * po10;
  sprintf (a, format, e, l);

  return ret;
}


#define TOUTS 5000
#define TOUT  500


void httpPost (String db, String table, String value) {

  String server= "<server_ip>";
  String port= "<port_ip"; //8086
  
    
  wifi.println("AT+CIPSTART=\"TCP\",\"" + server + "\"," + port);//start a TCP connection.

  if( wifi.find("OK")) {
    Serial.println("TCP connection ready");
  } delay(1000);
  
  String data= table + " value=" + value;
  String postRequest = "POST /write?db=" + db + " HTTP/1.1\r\n";
  postRequest += "Host: " + server + "\r\n";
  postRequest += "Accept: */*\r\n";
  postRequest += "Content-Length: " + String(data.length()) + "\r\n";
  postRequest += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
  postRequest += data; 
   
  Serial.println(postRequest);
  
  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.
  wifi.print(sendCmd);
  wifi.println(postRequest.length() );

  if(wifi.find(">")) { Serial.println("Sending.."); wifi.print(postRequest);

    if( wifi.find("SEND OK")) { Serial.println("Packet sent");

      while (wifi.available()) {
        String tmpResp = wifi.readString();
        Serial.println(tmpResp);
      }
      wifi.println("AT+CIPCLOSE");
      wifi.find("OK");
    }
  }
}

float seaLevelPressure = 101325;
float currentPressure;
long myPressure;
float lig;

void setup ( )
{
  char buff[32];
  char msg[1024];
  long tout=5000;
  long t0, t;

  float te, hu, pr;
  char conv[16];
  
  wifi.begin(19200);
  Serial.begin(19200);
  wifi.setTimeout(5000);
  dht.begin();
  light.begin();

  delay (1000);
   
  lig = light.get_lux();
  flo2str(conv, lig, 3);
  Serial.print("Lux ");
  Serial.println(conv);

  lig = light.get_lux();
  flo2str(conv, lig, 3);
  Serial.print("Lux ");
  Serial.println(conv);

  lig = light.get_lux();
  flo2str(conv, lig, 3);
  Serial.print("Lux ");
  Serial.println(conv);


  barometer = BMP180();
  // We check to see if we can connect to the sensor.
  if(barometer.EnsureConnected())
  {
    Serial.println("Connected to BMP180."); // Output we are connected to the computer.
    
     // When we have connected, we reset the device to ensure a clean start.
    barometer.SoftReset();
    // Now we initialize the sensor and pull the calibration data.
    barometer.Initialize();
  }else
  { 
    Serial.println("Barometer not found.");
  }  

  Serial.println("Modulo WIFI test");
  //sendWifi("AT+CIOBAUD=19200\r\n", TOUT, TOUTS);
  //for(;;);
  sendWifi("AT+GMR\r\n", TOUT, TOUTS);
  sendWifi("AT+RST\r\n",TOUT,TOUTS); // Borra la configuración que tenía el módulo
  
  //sendWifi("AT+CWMODE=3\r\n",TOUT, TOUTS);
  //sendWifi("AT+CWJAP=\"<wifiAP>\",\",<password>\"\r\n", TOUT,TOUTS);
  
  //sendData("AT+CIFSR\r\n",1000,DEBUG); // En el Serial Monitor aparece la IP de cliente y servidor.
  //sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // Multiples conexiones.
  //sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // El Puerto web es el 80
  
  //sendWifi("AT+CIPSTART=\"TCP\",\"192.168.43.1\",2221\r\n", TOUT , TOUTS);
  //sendWifi("AT+CIPSEND=5\r\n", TOUT, TOUTS);
  //sendWifi("HEL\r\n", TOUT, TOUTS);

  while(1){
    hu = dht.readHumidity();
    te = dht.readTemperature();
    
    if(barometer.IsConnected)
      {
	// Retrive the current pressure in Pascals.
	currentPressure = (float)barometer.GetPressure()/100.0;
	myPressure=  barometer.CompensatePressure(currentPressure);
	float altitude = barometer.GetAltitude(seaLevelPressure);
	float currentTemperature = barometer.GetTemperature();
      }

    lig = light.get_lux();

    flo2str(conv, currentPressure, 2);
    httpPost("statsdemo","press", conv);
    Serial.print("Pressure ");
    Serial.println(conv);

    flo2str(conv, hu, 2);
    httpPost("statsdemo","hum", conv);
    Serial.print("Hum ");
    Serial.println(conv);
    
    flo2str(conv, te, 2);
    httpPost("statsdemo","temp", conv);
    Serial.print("Temp ");
    Serial.println(conv);

    
    flo2str(conv, lig, 3);
    httpPost("statsdemo","lux", conv);
    Serial.print("Lux ");
    Serial.println(conv);

    
    delay(5000);
  }
  Serial.println("Se acabo");
}

void loop ()
{
   // your code here ...
}
