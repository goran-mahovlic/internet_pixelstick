#include <ESP8266WiFi.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <NeoPixelBus.h>

#define pixelCount 50
#define pixelPin 15
#define colorSaturation 128

File myFile;
WiFiUDP port;
WiFiClient client;

const char* ssid     = "lemilica-doma";
const char* password = "otvoreno";
unsigned int serverPort = 2390;
IPAddress serverIP(178, 62, 187, 251);
const char* host = "178.62.187.251";
const int httpPort = 80;
char charBuf[20];
const int chipSelect = 2;
char temp;

int R_data_int;
int G_data_int;
int B_data_int;
int firstDelimiterIndex;
int secondDelimiterIndex;
int thirdDelimiterIndex;
int fourthDelimiterIndex;
int line;
String oneLine;

NeoPixelBus strip = NeoPixelBus(pixelCount, pixelPin);

RgbColor red = RgbColor(colorSaturation, 0, 0);
RgbColor green = RgbColor(0, colorSaturation, 0);
RgbColor blue = RgbColor(0, 0, colorSaturation);
RgbColor white = RgbColor(colorSaturation);
RgbColor black = RgbColor(0);

void setup() {
  Wire.pins(4, 5); //on ESP-01.
  Wire.begin();
  strip.Begin();
  strip.Show();
  StartUp_OLED(); // Init Oled and fire up!
  sendStrXY("PixelSick...", 1, 0);
  delay(2000);
  connectToWifi();
  initializeSDcard();
}

void loop() {
  sendMessage("Start"); // send Start to server to convert new available image
  delay(5000);          //  we need to wait until server converts image
  clear_display();      // Clear OLED
  getDataFromWeb();
  turnRedLeds(4);
  delay(1000);
  turnOffLeds(4);
  turnGreenLeds(4);
  delay(1000);
  turnOffLeds(4);
  delay(500);
  clear_display(); // Clear OLED
  WaitForButtonPress();
  readSDfile("Picture.txt");
  //delay(1000);

}

void readSDfile(String file)
{
  line = 1;
  myFile = SD.open((file), O_READ);
  while (myFile.available()) {
    temp = myFile.read();
    if (temp == '\n') {
      line++;
      if (line > 12) {
        if (oneLine.length()>10)
          {
          LightTheStick(oneLine);
          }
      }
      oneLine = "";
    }
    else {
      oneLine = oneLine + String(temp);
    }
  }
  turnOffAllLeds();
  myFile.close();
}

void LightTheStick (String line)
{
  delay(30);
  for (int i = 0; i < 150; i = i + 3) {
    firstDelimiterIndex = line.indexOf(',', i);
    secondDelimiterIndex = line.indexOf(',', firstDelimiterIndex + 1);
    thirdDelimiterIndex = line.indexOf(',', secondDelimiterIndex + 1);
    fourthDelimiterIndex = line.indexOf(',', thirdDelimiterIndex + 1);
    R_data_int = (line.substring(firstDelimiterIndex + 1, secondDelimiterIndex)).toInt();
    G_data_int = (line.substring(secondDelimiterIndex + 1, thirdDelimiterIndex)).toInt();
    B_data_int = (line.substring(thirdDelimiterIndex + 1, fourthDelimiterIndex)).toInt(); // To the end of the string
    // Finaly we Light a pixel
    strip.SetPixelColor(i, R_data_int, G_data_int, B_data_int);
  }
  strip.Show();
}

void sendMessage(char data[]) {
  port.beginPacket(serverIP, serverPort);
  port.write(data);
  port.endPacket();
}
void turnOffAllLeds() {
  for (int i = 0; i < 50 ; i++) {
    strip.SetPixelColor(i, black);
  }
  strip.Show();
}

void initializeSDcard(){
  clear_display(); // Clear OLED
  sendStrXY("Initializing SD", 1, 0);
  if (!SD.begin(chipSelect)) {
    sendStrXY("SD failed!", 2, 0);
    strip.SetPixelColor(1, red);
    strip.Show();
    return;
  }
  else {
    sendStrXY("SD ready.", 2, 0);
    strip.SetPixelColor(1, green);
    strip.Show();
    delay(2000);
  }
}  
void connectToWifi(){
  clear_display(); // Clear OLED
  // We start by connecting to a WiFi network
  sendStrXY("Connecting to ", 1, 0);
  String(ssid).toCharArray(charBuf, 20) ;
  sendStrXY(charBuf, 3, 0);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    strip.SetPixelColor(0, red);
    strip.Show();
    delay(500);
  }
  strip.SetPixelColor(0, green);
  strip.Show();
  clear_display(); // Clear OLED
  sendStrXY("WiFi connected", 1, 0);
  sendStrXY("IP address: ", 2, 0);
  String(WiFi.localIP()).toCharArray(charBuf, 20) ;
  sendStrXY("10.254.151.134", 3, 0);
  delay(2000);
  }
void getDataFromWeb()
{
  // Use WiFiClient class to create TCP connections
  if (!client.connect(host, httpPort)) {
    sendStrXY("conn failed", 1, 0);
    return;
  }
  // We now create a URI for the request
  String url = "http://178.62.187.251/picture.text";
  clear_display(); // Clear OLED
  sendStrXY("Requesting URL: ", 1, 0);
  String(url).toCharArray(charBuf, 20) ;
  sendStrXY(charBuf, 2, 0);
  delay(2000);
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(100);
  clear_display(); // Clear OLED
  if (client.available()) {

    // Check to see if the file exists:
    if (SD.exists("Picture.txt")) {
      sendStrXY("File exists", 1, 0);
      sendStrXY("deleting old..", 2, 0);
      SD.remove("Picture.txt");
    }
    else {
      sendStrXY("New file", 1, 0);
    }
    myFile = SD.open("Picture.txt", FILE_WRITE);
    if (myFile) {
      sendStrXY("Writing to file...", 2, 0);
      while (client.connected()) {
        byte c = client.read();
        myFile.write(c);
      }
      // close the file:
      myFile.close();
      sendStrXY("done.", 3, 0);
    }
    else {
      // if the file didn't open, print an error:
      sendStrXY("error opening file", 2, 0);
    }
    client.stop();
  }
  }

void turnRedLeds(int leds){
  for (int j = 0 ; j < leds ; j++ )
  {
  strip.SetPixelColor(j, red);
  }
  strip.Show();
}

void turnGreenLeds (int leds){
  for (int j = 0 ; j < leds ; j++ )
  {
  strip.SetPixelColor(j, green);
  }
  strip.Show();
}

void turnOffLeds (int leds){
  for (int j = 0 ; j < leds ; j++ )
  {
  strip.SetPixelColor(j, black);
  }
  strip.Show();
}

void WaitForButtonPress()
{
  sendStrXY("All done", 1, 0);
  sendStrXY("Press button to", 2, 0);
  sendStrXY("Start PixelSick", 3, 0);
  sendMessage("Delete");
    while (analogRead(A0)>0){
      delay(100);
    }
    }
