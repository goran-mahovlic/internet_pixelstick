#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NeoPixelBus.h>

#define pixelCount 50
#define pixelPin 5
#define colorSaturation 128

WiFiClient client;
WiFiUDP port;

NeoPixelBus strip = NeoPixelBus(pixelCount, pixelPin);

RgbColor red = RgbColor(colorSaturation, 0, 0);
RgbColor green = RgbColor(0, colorSaturation, 0);
RgbColor blue = RgbColor(0, 0, colorSaturation);
RgbColor white = RgbColor(colorSaturation);
RgbColor black = RgbColor(0);

int width = 50;
int X;
int Y;
String oneLineRGB;
unsigned int serverPort = 2390;
IPAddress serverIP(178,62,187,251);
const char* host = "178.62.187.251";
char charBuf[400];
boolean pictureRecieved = false;

void setup() {
  // this resets all the neopixels to an off state
  strip.Begin();
  strip.Show();
  WiFi.begin("lemilica-doma", "otvoreno");
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
     strip.SetPixelColor(0, red);
     strip.Show();
}
strip.SetPixelColor(0, green);
strip.Show();
delay(1000);
strip.SetPixelColor(0, black);
strip.Show();
}

void loop() {
 
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    return;
  }
 
  // We now create a URI for the request
  String url = "/pixelstick.html"; 
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(200);
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    line.toCharArray(charBuf, 400);
    delay(10);
  if (line.indexOf("All Done")  > 0  || line.indexOf("No picture") > 0)
{
  turnOffAllLeds();
  strip.SetPixelColor(0, red);
  strip.SetPixelColor(1, red);
  strip.SetPixelColor(2, red);
  strip.SetPixelColor(3, red);
  strip.Show();
  delay(5000);
  sendMessage("Start");
}
  else if (line.indexOf("Picture is ready") > 0 )
  {
  strip.SetPixelColor(0, green);
  strip.SetPixelColor(1, green);
  strip.SetPixelColor(2, green);
  strip.SetPixelColor(3, green);
  strip.Show();
  sendMessage("Config");
  }
else if (line.indexOf("Size:") > 0 )
  {
sendMessage("GotSize");
line.replace(" ", "");
int configSize = line.length();
int findDelimiter = line.indexOf(",");
int findOpenBracket = line.indexOf("(");
int findCloseBracket = line.indexOf(")");
line.toCharArray(charBuf, 50);
String(configSize).toCharArray(charBuf, 50);
String(findDelimiter).toCharArray(charBuf, 50);
char tempWidth [10];
int i=0;
for (int j = findDelimiter + 1 ; j < findCloseBracket + 1; j++)
{
tempWidth[i] = charBuf [j];
i++;
}
line = String(tempWidth);
line.replace(")", "");
width = line.toInt();
sendMessage("Y;0");
  strip.SetPixelColor(0, black);
  strip.SetPixelColor(1, black);
  strip.SetPixelColor(2, black);
  strip.SetPixelColor(3, black);
  strip.Show();
X = 0;
Y = 0;
}
else if(line.indexOf("Pixel") > 0 ){
if (Y == width - 1){
sendMessage("Delete");
pictureRecieved = true;
}
else{
int firstCommaIndex = 0;
int secondCommaIndex = 0;
int openBracketIndex = 0;
int closedBracketIndex = 0;
char tempXY [11];
for (int i=0; i<50; i++)
{
openBracketIndex = line.indexOf(';', i);
closedBracketIndex = line.indexOf(';', openBracketIndex +1);
oneLineRGB = line.substring(openBracketIndex , closedBracketIndex +1);
String(oneLineRGB).toCharArray(charBuf, 50);
//sendMessage(charBuf);
firstCommaIndex = oneLineRGB.indexOf(',');
secondCommaIndex = oneLineRGB.indexOf(',',firstCommaIndex + 1);
openBracketIndex = oneLineRGB.indexOf(';');
closedBracketIndex = oneLineRGB.indexOf(';',openBracketIndex +1);
String R_data = oneLineRGB.substring(openBracketIndex + 1, firstCommaIndex);
String G_data = oneLineRGB.substring(firstCommaIndex +1, secondCommaIndex);
String B_data = oneLineRGB.substring(secondCommaIndex +1, closedBracketIndex); // To the end of the string
//String(R_data).toCharArray(charBuf, 50);
//sendMessage(charBuf);
//String(G_data).toCharArray(charBuf, 50);
//sendMessage(charBuf);
//String(B_data).toCharArray(charBuf, 50);
//sendMessage(charBuf);
int R_data_int = R_data.toInt();
int G_data_int = G_data.toInt();
int B_data_int = B_data.toInt();
// Finaly we Light a pixel
strip.SetPixelColor(i, R_data_int,G_data_int,B_data_int);
}
strip.Show();
Y++;
delay(100);
//turnOffAllLeds();
// And create next request
String tempXYstring;
tempXYstring = "Y;" + String(Y);
tempXYstring.toCharArray(tempXY, 11);
sendMessage(tempXY);
}

}}}
void sendMessage(char data[]) {
  port.beginPacket(serverIP,serverPort);
  port.write(data);
  port.endPacket();
}
void turnOffAllLeds(){
for (int i = 0; i < 51 ; i++){
  strip.SetPixelColor(i, black);
  strip.Show();
}
}
