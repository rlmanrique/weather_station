#include <SoftwareSerial.h>
#define DEBUG true

#define accesspoint "AndroidAP"
#define password "jaime1980"

#define baud_rate 19200  //115200

// In Arduino> RX: pin 3, TX: pin 2

SoftwareSerial esp8266(3,2); 

void setup()
{
  int c;
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  esp8266.begin(baud_rate);
 
  delay(1000); 
  Serial.write("AT+CIOBAUD=19200\r\n");
 
  //for(;;);
  Serial.println("Hola radiola....");
  
  esp8266.write("AT+CIOBAUD=19200\r\n");
  while((c=esp8266.read())==-1);
Serial.println(c);
  //sendData("AT+RST\r\n",10000,DEBUG); // Deletes previous configuration
  
  for(;;);

  sendData("AT+CWJAP=\""accesspoint"\",\""password"\"\r\n\n", 2000, DEBUG);

  delay(5000); 
  sendData("AT+CWMODE=3\r\n",1000,DEBUG); //Client and server side  
  sendData("AT+CIFSR\r\n",1000,DEBUG); // Show client and server IP in Serial Monitor
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // Several connections
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // webserver port = 80
}

void loop()
{
  if(esp8266.available()) 
    {

      if(esp8266.find("+IPD,"))
	{
	  delay(1000);

	  int connectionId = esp8266.read()-48; 

	  String webpage = "<head><meta http-equiv=\"refresh\" content=\"4\"></head>";
	  webpage += "<h1>KIO4.COM</h1>";
	  webpage += "Switch 8 status: ";
	  int p8 = digitalRead(8);
	  webpage += p8;
	  webpage += "<br>Switch 9 status: ";
	  int p9 = digitalRead(9);
	  webpage += p9;

	  String cipSend = "AT+CIPSEND=";
	  cipSend += connectionId;
	  cipSend += ",";
	  cipSend +=webpage.length();
	  cipSend +="\r\n";

	  sendData(cipSend,1000,DEBUG);
	  sendData(webpage,1000,DEBUG);

	  // Close the connection
	  String closeCommand = "AT+CIPCLOSE="; 
	  closeCommand+=connectionId;
	  closeCommand+="\r\n";

	  sendData(closeCommand,3000,DEBUG);
	}
    }
}

String sendData(String command, long timeout, boolean debug)
{
  String response = "";
  esp8266.print(command); 
  long time = millis();
Serial.println("comando 0000");
  while( (time+timeout) > millis())
    {
      while(esp8266.available())
	{

	  char c = esp8266.read(); 
	  response+=c; 
	} 
    }
Serial.println("tout....");
  if(debug)
    {
      Serial.print(response);
    } 
  return response;
}
