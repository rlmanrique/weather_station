#include <SoftwareSerial.h>

// Pin 2: Rx, Pin 3: Tx
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

#define TOUTS 5000
#define TOUT  5000

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
  //sendWifi("AT+CWJAP=\"RodrigoAP\",\"xxxxxx\"\r\n", TOUT,TOUTS);

//delay(5000); // Espera un poco que conecte con el Router.
//sendData("AT+CWMODE=3\r\n",1000,DEBUG); // Modo de cliente y servidor.
//sendData("AT+CIFSR\r\n",1000,DEBUG); // En el Serial Monitor aparece la IP de cliente y servidor.
//sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // Multiples conexiones.
//sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // El Puerto web es el 80
    sendWifi("AT+CIPSTART=\"TCP\",\"192.168.43.1\",2221\r\n", TOUT , TOUTS);
    sendWifi("AT+CIPSEND=5\r\n", TOUT, TOUTS);
    sendWifi("HEL\r\n", TOUT, TOUTS);
  Serial.println("Se acabo");
}

void loop ()
{
   // your code here ...
}
