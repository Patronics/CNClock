
#include <WiFi.h>
#include <iostream>
#include <stdio.h>
#include "time.h"
#include <cmath>




#define COMMAND_LENGTH 255





bool isHomed = false;
bool start = false;

//how long to wait in between updating time drawn, in mins
int updateInterval = 5;

/*
 aaa     
f   b
f   b
f   b
 ggg
e   c
e   c
e   c
 ddd
*/
//coordinates assume bottom left is 0, once homed
//segments starting y, top to bottom, in mm
const int aY = 350;
const int bY = 335;
const int cY = 200;
const int dY = 50;
const int eY = 200;
const int fY = 335;
const int gY = 200;

//segements starting x. left to right, in mm
const int aX = 20;
const int bX = 120;
const int cX = 120;
const int dX = 20;
const int eX = 0;
const int fX = 0;
const int gX = 20;

//segment lengths, short are a, d, g and long are b, c, e, f, in mm
const int shortLength = 100;
const int longLength = 120; 

//offsets for the different digits, 1 2 : 3 4, in mm
const int digitOneOffset = 50;
const int digitTwoOffset = 200;
const int digitThreeOffset = 400;
const int digitFourOffset = 550;
const int colonOffset = 350;

//offset for eraser to cover same ground as marker, in mm
const int eraserX = 10;
const int eraserY = 57;



void drawTime(int hrs, int mins, bool erase);
void drawDigit(int number, int digit, int erase);

void dividingColon(int digit, bool erase);

void digitZero(int digit, bool erase);
void digitOne(int digit, bool erase);
void digitTwo(int digit, bool erase);
void digitThree(int digit, bool erase);
void digitFour(int digit, bool erase);
void digitFive(int digit, bool erase);
void digitSix(int digit, bool erase);
void digitSeven(int digit, bool erase);
void digitEight(int digit, bool erase);
void digitNine(int digit, bool erase);

void segmentA(int digit, bool erase);
void segmentB(int digit, bool erase);
void segmentC(int digit, bool erase);
void segmentD(int digit, bool erase);
void segmentE(int digit, bool erase);
void segmentF(int digit, bool erase);
void segmentG(int digit, bool erase);

void markerHome();
void markerDown();
void eraseDown();
void capMarker();
void reHome();
void markerHome();
void goToXY(double x, double y);
void reHome(bool x, bool y);
void test();
void controlledSerial();
void clockLoop();
void eraseAll();
void drawNow();
void rest();
void wifiLoop();

struct tm currentTime;
struct tm lastUpdate;

tm getTime();

const char* ssid     = "abcd"; //replace with sierra college ssid;
const char* password = "12341234"; //psswd is student login for canvas

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -28800;
const int   daylightOffset_sec = 3600;

String header;
unsigned long currTime = millis();
unsigned long prevTime = 0;
const long timeoutTime = 2000;
WiFiServer server(80);



void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 36, 32);  

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2500);
    Serial.println(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");

  Serial.println("IP Adress: ");
  Serial.println(WiFi.localIP());
  server.begin();
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  currentTime = getTime();


  while (!currentTime.tm_hour){
    delay(2500);
    currentTime = getTime();
  }
  
}

void loop() {
  delay(500);
  wifiLoop();
  
}

void wifiLoop(){
  WiFiClient client = server.available();

  if(client){
    String currentLine = "";
    while(client.connected()){
      currTime = millis();
      if(start == true){
        clockLoop();
        Serial.println("checking for update");
      }
      if(client.available()){
        char c = client.read();
        Serial.write(c);
        header += c;
        if(c == '\n'){
          if(currentLine.length() == 0){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if(header.indexOf("GET /updateInterval/2") >= 0){
              Serial.println("update interval: 2 minutes");
              updateInterval = 2;
            }
            else if(header.indexOf("GET /updateInterval/5") >= 0){
              Serial.println("update interval: 5 minutes");
              updateInterval = 5;
            }
            else if(header.indexOf("GET /updateInterval/10") >= 0){
              Serial.println("update interval: 10 minutes");
              updateInterval = 10;
            }
            else if(header.indexOf("GET /updateInterval/20") >= 0){
              Serial.println("update interval: 20 minutes");
              updateInterval = 20;
            }
            else if(header.indexOf("GET /start") >= 0){
              Serial.println("starting the clock");
              start = true;
            }
            else if(header.indexOf("GET /erase") >= 0){
              Serial.println("eraseing the clock");
              eraseAll();
            }
            else if(header.indexOf("GET /now") >= 0){
              Serial.println("drawing current time");
              drawNow();
            }
            else if(header.indexOf("GET /stop") >= 0){
              Serial.println("stopping the clock");
              rest();
            }
          

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
  
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
  
            client.println("<body><h1> CNC Clock Controller</h1>");
  
            client.println("<p><a href=\"/start\"><button class=\"button button2\">start clock</button></a></p>");
            client.println("<p><a href=\"/erase\"><button class=\"button button2\">erase clock</button></a></p>");
            client.println("<p><a href=\"/now\"><button class=\"button button2\">draw time</button></a></p>");
            client.println("<p><a href=\"/stop\"><button class=\"button button2\">stop now</button></a></p>");
  
            switch(updateInterval){
              case 2:
                client.println("<p><a href=\"/updateInterval/2\"><button class=\"button\">2 Minute</button></a></p>");
                client.println("<p><a href=\"/updateInterval/5\"><button class=\"button button2\">5 Minute</button></a></p>");
                client.println("<p><a href=\"/updateInterval/10\"><button class=\"button button2\">10 Minute</button></a></p>");
                client.println("<p><a href=\"/updateInterval/20\"><button class=\"button button2\">20 Minute</button></a></p>");
                break;
  
              case 5:
                client.println("<p><a href=\"/updateInterval/2\"><button class=\"button button2\">2 Minute</button></a></p>");
                client.println("<p><a href=\"/updateInterval/5\"><button class=\"button\">5 Minute</button></a></p>");
                client.println("<p><a href=\"/updateInterval/10\"><button class=\"button button2\">10 Minute</button></a></p>");
                client.println("<p><a href=\"/updateInterval/20\"><button class=\"button button2\">20 Minute</button></a></p>");
                break;
  
              case 10:
                client.println("<p><a href=\"/updateInterval/2\"><button class=\"button button2\">2 Minute</button></a></p>");
                client.println("<p><a href=\"/updateInterval/5\"><button class=\"button button2\">5 Minute</button></a></p>");
                client.println("<p><a href=\"/updateInterval/10\"><button class=\"button\">10 Minute</button></a></p>");
                client.println("<p><a href=\"/updateInterval/20\"><button class=\"button button2\">20 Minute</button></a></p>");
                break;
  
              case 20:
                client.println("<p><a href=\"/updateInterval/2\"><button class=\"button button2\">2 Minute</button></a></p>");
                client.println("<p><a href=\"/updateInterval/5\"><button class=\"button button2\">5 Minute</button></a></p>");
                client.println("<p><a href=\"/updateInterval/10\"><button class=\"button button2\">10 Minute</button></a></p>");
                client.println("<p><a href=\"/updateInterval/20\"><button class=\"button\">20 Minute</button></a></p>");
                break;
  
              default:
                client.println("<p><a href=\"/updateInterval/2\"><button class=\"button button2\">2 Minute</button></a></p>");
                client.println("<p><a href=\"/updateInterval/5\"><button class=\"button button2\">5 Minute</button></a></p>");
                client.println("<p><a href=\"/updateInterval/10\"><button class=\"button button2\">10 Minute</button></a></p>");
                client.println("<p><a href=\"/updateInterval/20\"><button class=\"button button2\">20 Minute</button></a></p>");
                break;
              }
            }
            else{
              currentLine = "";
            }
          }
          else if(c != '\r'){
            currentLine += c;
          }  
      }
    }
  }
  header = "";
  client.stop();
  Serial.println("Client disconnected");
  Serial.println("");
}

void controlledSerial(){
  //updateTime(currentTime, lastUpdate);
  //currentTime = getTime();

  if(Serial.available()){
    int ch = Serial.read();
    if(ch == 'y'){
      currentTime = getTime();
      updateTime(currentTime, lastUpdate);
    }
  }
}

void test(){
  
  reHome();

  //drawDigit(0, digitOneOffset, false);
  //delay(500);
  drawDigit(0, digitTwoOffset, false);
  delay(500);
  dividingColon(colonOffset, false);
  delay(500);
  drawDigit(0, digitTwoOffset, true);
  delay(500);
  drawDigit(0, colonOffset, true);
  delay(500);
  
  reHome();
 
  capMarker();
  //markerCap();
  delay(2000);
}

//needs more thought
void clockLoop(){
  currentTime = getTime();
  updateTime(currentTime, lastUpdate);
  
}

void eraseAll(){
  reHome();
  drawDigit(8, digitOneOffset, true);
  delay(500);
  drawDigit(8, digitTwoOffset, true);
  delay(500);
  dividingColon(colonOffset, true);
  delay(500);
  drawDigit(8, digitTwoOffset, true);
  delay(500);
  drawDigit(8, colonOffset, true);
  delay(500);
  reHome();
  capMarker();
}

void drawNow(){
  reHome();
  drawTime(lastUpdate.tm_hour, lastUpdate.tm_min, true);
  drawTime(currentTime.tm_hour, currentTime.tm_min, false);
  lastUpdate = currentTime;
  reHome();
  capMarker();
}

void rest(){
  reHome();
  capMarker();
  start = false;
}

tm getTime(){
  struct tm date;
  if(!getLocalTime(&date)){
    Serial.println("Failed to get time");
    return date = {};
  }  
  Serial.printf("time: %d:%d \n", date.tm_hour, date.tm_min);
  return date;
}


void updateTime(tm currentTime, tm lastUpdate){
  //markerHome();
  reHome();

  if(!lastUpdate.tm_hour){
    drawTime(currentTime.tm_hour, currentTime.tm_min, false);
    lastUpdate = currentTime;
  } 
  else if(abs(currentTime.tm_min - lastUpdate.tm_min) > updateInterval){ 
    drawTime(lastUpdate.tm_hour, lastUpdate.tm_min, true);
    drawTime(currentTime.tm_hour, currentTime.tm_min, false);
    lastUpdate = currentTime;
  }

  reHome();
  capMarker();
}

void drawTime(int hrs, int mins, bool erase){
  hrs=hrs%12;   //convert 24 hour time to 12 hour time
  drawDigit((hrs/10)%10, digitOneOffset, erase);    // 10s place of hours
  drawDigit(hrs%10, digitTwoOffset, erase);        // 1s place of hours
  dividingColon(colonOffset, erase);
  drawDigit((mins/10)%10, digitThreeOffset, erase); // 10s place of mins
  drawDigit(mins%10, digitFourOffset, erase);      //1s place of mins
}


void sendCommand(char* command){
    Serial.print(command);
    Serial.print("\n");
    Serial1.print(command);
    Serial1.print("\n");
    delay(500);
}


void reHome(bool x, bool y){
  isHomed=true;
  markerHome();
  char xChar = ' ';
  if (x){xChar='X';}
  char yChar = ' ';
  if (y){yChar='Y';}
  char nextCommand[COMMAND_LENGTH];
  snprintf(nextCommand,COMMAND_LENGTH, "G28 %c%c",xChar,yChar);
  sendCommand(nextCommand); 
  sendCommand("M400");
}

void reHome(){
  reHome(true, true);
}


void goToXY(double x, double y){
  char nextCommand[COMMAND_LENGTH];
  snprintf(nextCommand,COMMAND_LENGTH, "G1 X%fY%f F10000",x,y);
  sendCommand(nextCommand);
  sendCommand("M400");
  //delay(2000);
}


//this is a guess, needs to be changed when eraser is put on
void eraserDown(){
  printf(";eraserDown\n");
  sendCommand("M280 P0 S10");
}

void markerHome(){
  printf(";markerRaised\n");
  sendCommand("M280 P0 S40");
}

void capMarker(){
  printf(";capMarker\n");
  sendCommand("G0 X1 Y4");
  markerCap();
 
}

void markerWrite(){
  printf(";markerWrite\n");
  sendCommand("M280 P0 S135");
}

void markerCap(){
  printf(";markerCap\n");
  sendCommand("M280 P0 S127");
  sendCommand("M84");
}

void drawDigit(int number, int digit, int erase){
  switch (number){
    case 0:
      digitZero(digit, erase);
      break;
    case 1:
      digitOne(digit, erase);
      break;
    case 2:
      digitTwo(digit, erase);
      break;
    case 3:
      digitThree(digit, erase);
      break;
    case 4:
      digitFour(digit, erase);
      break;
    case 5:
      digitFive(digit, erase);
      break;
    case 6:
      digitSix(digit, erase);
      break;
    case 7:
      digitSeven(digit, erase);
      break;
    case 8:
      digitEight(digit, erase);
      break;
    case 9:
      digitNine(digit, erase);
      break;
    default:
      printf(";error, invalid number passed to drawDigit");
  }
}

//does not home the cradle, does not check marker safety
void digitZero(int digit, bool erase){
  segmentF(digit, erase);
  segmentE(digit, erase);
  segmentD(digit, erase);
  segmentA(digit, erase);
  segmentB(digit, erase);
  segmentC(digit, erase);
}

void digitOne(int digit, bool erase){
  segmentB(digit, erase);
  segmentC(digit, erase);
}

void digitTwo(int digit, bool erase){
  segmentA(digit, erase);
  segmentB(digit, erase);
  segmentG(digit, erase);
  segmentE(digit, erase);
  segmentD(digit, erase);
}

void digitThree(int digit, bool erase){
  segmentA(digit, erase);
  segmentB(digit, erase);
  segmentC(digit, erase);
  segmentD(digit, erase);
  segmentG(digit, erase);
}

void digitFour(int digit, bool erase){
  segmentF(digit, erase);
  segmentG(digit, erase);
  segmentC(digit, erase);
  segmentB(digit, erase);
}

void digitFive(int digit, bool erase){
  segmentA(digit, erase);
  segmentF(digit, erase);
  segmentG(digit, erase);
  segmentC(digit, erase); 
  segmentD(digit, erase);
}

void digitSix(int digit, bool erase){
  segmentF(digit, erase);
  segmentG(digit, erase);
  segmentC(digit, erase); 
  segmentE(digit, erase);
  segmentD(digit, erase);
  segmentA(digit, erase);
}

void digitSeven(int digit, bool erase){
  segmentA(digit, erase);
  segmentB(digit, erase);
  segmentC(digit, erase);
}

void digitEight(int digit, bool erase){
  segmentF(digit, erase);
  segmentE(digit, erase);
  segmentD(digit, erase);
  segmentA(digit, erase);
  segmentB(digit, erase);
  segmentC(digit, erase);
  segmentG(digit, erase);
}

void digitNine(int digit, bool erase){
  segmentF(digit, erase);
  segmentG(digit, erase);
  segmentC(digit, erase); 
  segmentA(digit, erase);
  segmentB(digit, erase);
}

void dividingColon(int digit, bool erase){
  markerHome();
  
  int X = 15 + digit;
  int Y = 250;
  
  if(erase){
    X += eraserX;
    Y += eraserY;
    goToXY(X, Y);
    eraserDown();
  }
  else{
    goToXY(X, Y);
    markerWrite();
  }

  //now at start of top one
  sendCommand("G3 I0 J10 F3000");
  
  markerHome();

  Y -= 100;
  goToXY(X, Y);
  
  if(erase){eraserDown();}
  else{markerWrite();}
  
  //now at start of bottom one
  sendCommand("G2 I0 J10 F3000");

  markerHome();
}

//all segments check set the pen up before moving and before returning, does NOT re home cradle
void segmentA(int digit, bool erase){
  markerHome();

  int X = aX + digit;
  int Y = aY;
  
  if(erase){
    X += eraserX;
    Y += eraserY;
    goToXY(X, Y);
    eraserDown();
  }
  else{
    goToXY(X, Y);
    markerWrite();
  }

  X += shortLength;
  goToXY(X, Y);
  
  markerHome();
}

void segmentB(int digit, bool erase){
  markerHome();
  
  int X = bX + digit;
  int Y = bY;
  
  if(erase){
    X += eraserX;
    Y += eraserY;
    goToXY(X, Y);
    eraserDown();
  }
  else{
    goToXY(X, Y);
    markerWrite();
  }

  Y -= longLength;
  goToXY(X, Y);
  
  markerHome();
}

void segmentC(int digit, bool erase){
  markerHome();
  
  int X = cX + digit;
  int Y = cY;
  
  if(erase){
    X += eraserX;
    Y += eraserY;
    goToXY(X, Y);
    eraserDown();
  }
  else{
    goToXY(X, Y);
    markerWrite();
  }

  Y -= longLength;
  goToXY(X, Y);
  
  markerHome();
}

void segmentD(int digit, bool erase){
  markerHome();
  
  int X = dX + digit;
  int Y = dY;
  
  if(erase){
    X += eraserX;
    Y += eraserY;
    goToXY(X, Y);
    eraserDown();
  }
  else{
    goToXY(X, Y);
    markerWrite();
  }

  X += shortLength;
  goToXY(X, Y);
  
  markerHome();
}

void segmentE(int digit, bool erase){
  markerHome();
  
  int X = eX + digit;
  int Y = eY;
  
  if(erase){
    X += eraserX;
    Y += eraserY;
    goToXY(X, Y);
    eraserDown();
  }
  else{
    goToXY(X, Y);
    markerWrite();
  }

  Y -= longLength;
  goToXY(X, Y);
  
  markerHome();
}

void segmentF(int digit, bool erase){
  markerHome();
  
  int X = fX + digit;
  int Y = fY;
  
  if(erase){
    X += eraserX;
    Y += eraserY;
    goToXY(X, Y);
    eraserDown();
  }
  else{
    goToXY(X, Y);
    markerWrite();
  }

  Y -= longLength;
  goToXY(X, Y);
  
  markerHome();
}

void segmentG(int digit, bool erase){
  markerHome();
 
  int X = gX + digit;
  int Y = gY;
  
  if(erase){
    X += eraserX;
    Y += eraserY;
    goToXY(X, Y);
    eraserDown();
  }
  else{
    goToXY(X, Y);
    markerWrite();
  }

  X += shortLength;
  goToXY(X, Y);
  
  markerHome();
}
