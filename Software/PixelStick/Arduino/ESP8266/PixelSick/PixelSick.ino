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
int width;
int X;
int Y;
int R_data;
int G_data;
int B_data;

unsigned int serverPort = 2390;
IPAddress serverIP(178,62,187,251);
const char* host = "178.62.187.251";
char charBuf[50];

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
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    line.toCharArray(charBuf, 50);

  if (line.indexOf("All Done")  > 0  || line.indexOf("No picture") > 0)
{
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
//char[] charsToTrim = { ' ', '\t' ,'S' , 'i', 'z' , 'e' , ':'};
//line = line.Trim(charsToTrim);
line.replace(" ", ""); 
int configSize = line.length(); 
int findDelimiter = line.indexOf(",");
int findOpenBracket = line.indexOf("(");
int findCloseBracket = line.indexOf(")");
line.toCharArray(charBuf, 50);
sendMessage(charBuf);
String(configSize).toCharArray(charBuf, 50);
sendMessage(charBuf);
String(findDelimiter).toCharArray(charBuf, 50);
sendMessage(charBuf);
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
sendMessage("XY;0;0");
  strip.SetPixelColor(0, black);
  strip.SetPixelColor(1, black);
  strip.SetPixelColor(2, black);
  strip.SetPixelColor(3, black);
  strip.Show();
X = 0;
Y = 0;
}
else if(line.indexOf("Pixel") > 0 ){
if (X == 49){
Y++;
X=0;
delay(1000);
}
if (Y == width){
sendMessage('Delete');
}
else{
int XYSize = line.length();
int findDelimiter = line.indexOf(",");
int findOpenBracket = line.indexOf("(");
int findCloseBracket = line.indexOf(")");
line.toCharArray(charBuf, 50);
sendMessage(charBuf);
String(XYSize).toCharArray(charBuf, 50);
sendMessage(charBuf);
String(findDelimiter).toCharArray(charBuf, 50);
sendMessage(charBuf);
char tempXY [11];
int i=0;
for (int j = findDelimiter + 1 ; j < findCloseBracket + 1; j++)
{
tempXY[i] = charBuf [j];
i++;
}
line = String(tempXY);
line.replace(")", "");
int[] RGB = line.ConvertAll(s.Split(','), int.Parse);
R_data = RGB[0];
G_data = RGB[1];
B_data = RGB[2];
// Finaly we Light a pixel
strip.SetPixelColor(X, R_data,G_data,B_data);
// And create next request
String tempXYstring;
delay(1000);
tempXYstring = "XY;" + String(X) + ";" + String(Y);
tempXYstring.toCharArray(tempXY, 11);
sendMessage(tempXY);
}}}}

void sendMessage(char data[]) {
  port.beginPacket(serverIP,serverPort);
  port.write(data);
  port.endPacket();
}
