#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiClient client;
WiFiUDP port;
WiFiUDP port2;

char packetBuffer[255];

char recievedBuffer[50];
String recievedData;

unsigned int serverPort = 2390;
IPAddress serverIP(178,62,187,251);
int picWidth = 51;
int picHight = 51;

void setup() {
  WiFi.begin("lemilica-doma", "otvoreno");
  port.begin(2390);
  pinMode(2, OUTPUT);
  digitalWrite(2,LOW);
}

void loop() {
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

  getDataFromServer();
  handleMessage(recievedData);
  delay(500);
  
}

void handleMessage(String data) {
  if (data.startsWith("PicSize")) {
    sprintf(recievedBuffer,"%s",packetBuffer);
    sendMessage(recievedBuffer);
    //now we need to get size and put it in Hight and Width variable
    for(int i = 1 ; i < picWidth  ; i++)
    {
    int len = port.read(packetBuffer, 255); 
    while (packetBuffer[len] == 0){
    len = port.read(packetBuffer, 255); 
    delay(10);
    }
    getDataFromServer();
    sprintf(recievedBuffer,"%s",packetBuffer);
    //sendMessage(recievedBuffer);
    for (int j = 1; j < picHight; j++)
    {
    delay(10);
    char sendBuffer[10];
    sprintf(sendBuffer,"X=%i-Y=%i;",i,j);
    sendMessage(sendBuffer);
    int len = port.read(packetBuffer, 255); 
    while (packetBuffer[len] == 0){
    len = port.read(packetBuffer, 255); 
    delay(10);
    }
    getDataFromServer();
    sprintf(recievedBuffer,"%s",packetBuffer);
    //sendMessage(recievedBuffer);
    }
    delay(1000); //show first row
  }
  } 
  //sendMessage("Stop");
  recievedData = ""; //clear recieved data

}

void sendMessage(char data[]) {
  port2.beginPacket(serverIP,serverPort);
  port2.write(data);
  port2.endPacket();
}

void getDataFromServer() {
  int packetSize = port.parsePacket();
  if (packetSize) {
    int len = port.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = 0;
    recievedData = String(packetBuffer);
  }
}

