#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiClient client;
WiFiUDP port2;

char packetBuffer[255];

unsigned int serverPort = 2390;
IPAddress serverIP(178,62,187,251);
int picWidth = 0;
int picHight = 0;

void setup() {
  WiFi.begin("otvorenamreza.org", "otvoreno");
  pinMode(2, OUTPUT);
  digitalWrite(2,LOW);
  delay(10000);
}

void loop() {
  sendMessage("S");
  delay(1000);
  sendMessage("C");
  delay(1000);
  sendMessage("XY;1;1");
  delay(1000);
  sendMessage("D");
  delay(1000);  
  //wait for first message
  //send first message and wait for config replay
  //from first message get picture size (x and y)
  //check if size is correct if yes continue to process pixels
  //ask for pixel in format getPixel(X;Y)
  //send message to photo ESP to start recording (this can be on server side)
  //get pixel in format (X;Y;R;G;B)
  //set pixel on RGB strip
  //on new ROW move motor for 0,5s
  //on last row move motor back to home position switch
  //send message to photo ESP to stop recording (this could be on server side)

}

void sendMessage(char data[]) {
  port2.beginPacket(serverIP,serverPort);
  port2.write(data);
  port2.endPacket();
}

