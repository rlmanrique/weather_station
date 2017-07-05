#include <SoftwareSerial.h>
#define DEBUG true

#define accesspoint "AndroidAP"
#define password "1234"
q
#define baud_rate 115200

SoftwareSerial esp8266(3,2); 
// TX to terminal 3
// RX to terminal 2

void setup()
{
  pinMode(13,OUTPUT);
  Serial.begin(baud_rate);
  esp8266.begin(baud_rate); 

  sendData("AT+RST\r\n",2000,DEBUG); // Deletes previous configuration

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

String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  esp8266.print(command); 
  long int time = millis();

  while( (time+timeout) > millis())
    {
      while(esp8266.available())
	{

	  char c = esp8266.read(); 
	  response+=c; 
	} 
    }

  if(debug)
    {
      Serial.print(response);
    } 
  return response;
}
