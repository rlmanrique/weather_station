#include <Arduino.h>
#include <SoftwareSerial.h>
#include <string.h>

#define TOUTS 5000
#define TOUT  5000
#define DEBUG true

SoftwareSerial wifi(2,3);

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

void httpPost (String db, String table, String value) {

  String server= "192.168.43.112";
  String port= "8086";
  
    
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

  delay(500);

  if(wifi.find(">")) { Serial.println("Sending.."); wifi.print(postRequest);

    if( wifi.find("SEND OK")) { Serial.println("Packet sent");

      while (wifi.available()) {
        String tmpResp = wifi.readString();
        Serial.println(tmpResp);
      }
      wifi.println("AT+CIPCLOSE");
    }
  }
}


void setup ( )
{
  char buff[32];
  char msg[1024];
  long tout=5000;
  long t0, t;
 
  wifi.begin(9600);
  Serial.begin(9600);

  Serial.println("Modulo WIFI test");
  
  sendWifi("AT+GMR\r\n", TOUT, TOUTS);
  sendWifi("AT+RST\r\n",TOUT,TOUTS); // Borra la configuración que tenía el módulo

}

void loop ()
{
  httpPost("statsdemo","cpu", "1.7");
}


void loop ()
{

}
