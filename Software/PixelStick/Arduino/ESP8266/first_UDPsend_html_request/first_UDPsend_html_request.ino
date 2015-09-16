#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiClient client;
WiFiUDP port;

unsigned int serverPort = 2390;
IPAddress serverIP(178,62,187,251);
const char* host = "178.62.187.251";
char charBuf[50];

void setup() {
  WiFi.begin("lemilica-doma", "otvoreno");
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
}
}
void loop() {
  sendMessage("S");
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    sendMessage("connection failed");
    return;
  }
  // We now create a URI for the request
  String url = "/pixelstick.html";  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
   while(client.available()){
    String line = client.readStringUntil('\r');
    line.toCharArray(charBuf, 50);
    sendMessage(charBuf);
  }
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    sendMessage("disconnecting.");
    client.stop(); 
} 
}

void sendMessage(char data[]) {
  port.beginPacket(serverIP,serverPort);
  port.write(data);
  port.endPacket();
}
