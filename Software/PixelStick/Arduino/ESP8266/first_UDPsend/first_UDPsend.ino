#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiClient client;
WiFiUDP port;

unsigned int serverPort = 2390;
IPAddress serverIP(178,62,187,251);

void setup() {
  WiFi.begin("lemilica-doma", "otvoreno");
}

void loop() {
 sendMessage("test");
 delay(10000);
}

void sendMessage(char data[]) {
  port.beginPacket(serverIP,serverPort);
  port.write(data);
  port.endPacket();
}
