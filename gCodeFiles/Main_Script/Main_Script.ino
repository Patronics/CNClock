
//#include <WiFi.h>
#include <iostream>
#include <stdio.h>
#include "time.h"
#include <cmath>

#ifdef ARDUINO
  #include <WiFi.h>
#endif
#ifndef ARDUINO
  //uncomment this line if testing
 // #define SIMULATE
#endif

#define COMMAND_LENGTH 255

//function declarations
void drawTime(int hrs, int mins, int baseline, bool erase);
void drawDigit(int startX, int startY, int scale, int number, bool erase);

//void digitForm(int startX, int startY, int scale); //digitForm identical to digitEight
void digitZero(int startX, int startY, int scale, bool erase);
void digitOne(int startX, int startY, int scale, bool erase);
void digitTwo(int startX, int startY, int scale, bool erase);
void digitThree(int startX, int startY, int scale, bool erase);
void digitFour(int startX, int startY, int scale, bool erase);
void digitFive(int startX, int startY, int scale, bool erase);
void digitSix(int startX, int startY, int scale, bool erase);
void digitSeven(int startX, int startY, int scale, bool erase);
void digitEight(int startX, int startY, int scale, bool erase);
void digitNine(int startX, int startY, int scale, bool erase);

void dividingColon(int startX, int startY, int scale, bool erase);

void segmentA(int startX, int startY, int scale, bool erase);
void segmentB(int startX, int startY, int scale, bool erase);
void segmentC(int startX, int startY, int scale, bool erase);
void segmentD(int startX, int startY, int scale, bool erase);
void segmentE(int startX, int startY, int scale, bool erase);
void segmentF(int startX, int startY, int scale, bool erase);
void segmentG(int startX, int startY, int scale, bool erase);

void markerHome();
void markerDown();
int reHome();
void markerHome();
int goToXY(double x, double y);
int fastToXY(double x, double y);
int reHome(bool x, bool y);

//globals
bool isHomed = false;

struct tm currentTime;
struct tm lastUpdate;

const char* ssid     = ""; //replace with sierra college ssid;
const char* password = ""; //psswd is student login for canvas

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -28800;
const int   daylightOffset_sec = 3600;
const int scale = 200; // modify this to change the scale of the segments drawn


void setup(){
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  //WiFi.setSleep(false);
  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2500);
    Serial.println(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  Serial1.begin(115200, SERIAL_8N1, 36, 32);

  currentTime = getTime();
}


void loop(){
  delay(500);
  updateTime(currentTime, lastUpdate);
  currentTime = getTime();
}

void control(){
  delay(1000);
  currentTime = getTime();
  //updateTime(currentTime, lastUpdate);
  Serial.printf("time1: %d:%d\n", currentTime.tm_hour, currentTime.tm_min);
}

int sendCommand(char* command){
    delay(500);
    Serial.print(command);
    Serial.print("\n");
    Serial1.print(command);
    Serial1.print("\n");
}

void updateTime(tm currentTime, tm lastUpdate){
  if(!lastUpdate.tm_hour){
    drawTime(currentTime.tm_hour, currentTime.tm_min, 20, false);
    markerHome();
    lastUpdate = currentTime;
    return;
  }
  else if(abs(currentTime.tm_min - lastUpdate.tm_min) > 1){ //the right side of the conditional controls how frequently the clock draws out the time, currently every minute
    drawTime(currentTime.tm_hour, currentTime.tm_min, 20, true);
    markerHome();
    drawTime(currentTime.tm_hour, currentTime.tm_min, 20, false);
    markerHome();
    lastUpdate = currentTime;
    return;
  }
  else{
    return;
  }
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

//test for serial connection between esp 32 and BTT SKR
void test(){
  Serial1.print("M280 P0 S130");
  Serial1.print("\n");
  delay(2000);
  Serial1.print("M280 P0 S80");
  Serial1.print("\n");
  delay(2000);
}

void drawTime(int hrs, int mins, int baseline, bool erase){
  hrs=hrs%12;   //convert 24 hour time to 12 hour time
  drawDigit((hrs/10)%10, 0, baseline, scale, erase);    // 10s place of hours
  drawDigit(hrs%10, 70, baseline, scale, erase);        // 1s place of hours
  dividingColon(110, baseline, scale, erase);
  drawDigit((mins/10)%10, 150, baseline, scale, erase); // 10s place of mins
  drawDigit(mins%10, 225, baseline, scale, erase);      //1s place of mins
}

void drawDigit(int number, int startX, int startY, int scale, bool erase){
  switch (number){
    case 0:
      digitZero(startX, startY, scale, erase);
      break;
    case 1:
      digitOne(startX, startY, scale, erase);
      break;
    case 2:
      digitTwo(startX, startY, scale, erase);
      break;
    case 3:
      digitThree(startX, startY, scale, erase);
      break;
    case 4:
      digitFour(startX, startY, scale, erase);
      break;
    case 5:
      digitFive(startX, startY, scale, erase);
      break;
    case 6:
      digitSix(startX, startY, scale, erase);
      break;
    case 7:
      digitSeven(startX, startY, scale, erase);
      break;
    case 8:
      digitEight(startX, startY, scale, erase);
      break;
    case 9:
      digitNine(startX, startY, scale, erase);
      break;
    default:
      printf(";error, invalid number passed to drawDigit");
  }
}

void digitZero(int startX, int startY, int scale, bool erase){
  markerHome();
  fastToXY(startX,startY);
  delay(500);
  segmentF(startX, startY, scale, erase);
  segmentA(startX, startY, scale, erase);
  segmentB(startX, startY, scale,erase);
  //delay here? Marker up through segment G
  segmentE(startX, startY, scale, erase);
  segmentD(startX, startY, scale, erase);
  segmentC(startX, startY, scale, erase);
 
}

void digitOne(int startX, int startY, int scale, bool erase){
  markerHome();
  fastToXY(startX,startY);
  delay(500);
  segmentB(startX, startY, scale, erase);
  //delay here? Skips a bunch, goes all the way down, then up to the middle
  segmentC(startX, startY, scale, erase);
}

void digitTwo(int startX, int startY, int scale, bool erase){
  markerHome();
  fastToXY(startX,startY);
  delay(500);
  segmentA(startX, startY, scale, erase);
  segmentB(startX, startY, scale, erase);
  segmentG(startX, startY, scale, erase);
  segmentE(startX, startY, scale, erase);
  segmentD(startX, startY, scale, erase);
}

void digitThree(int startX, int startY, int scale, bool erase){
  markerHome();
  fastToXY(startX,startY);
  delay(500);
  segmentA(startX, startY, scale, erase);
  segmentB(startX, startY, scale, erase);
  segmentG(startX, startY, scale, erase);
  segmentD(startX, startY, scale, erase);
  segmentC(startX, startY, scale, erase);
}

void digitFour(int startX, int startY, int scale, bool erase){
  markerHome();
  fastToXY(startX,startY);
  delay(500);
  segmentF(startX, startY, scale, erase);
  segmentB(startX, startY, scale, erase);
  segmentG(startX, startY, scale, erase);
  segmentC(startX, startY, scale, erase);
}

void digitFive(int startX, int startY, int scale, bool erase){
  markerHome();
  fastToXY(startX,startY);
  delay(500);
  segmentF(startX, startY, scale, erase);
  segmentA(startX, startY, scale, erase);
  segmentG(startX, startY, scale, erase);
  segmentD(startX, startY, scale, erase);
  segmentC(startX, startY, scale, erase);
}

void digitSix(int startX, int startY, int scale, bool erase){
  markerHome();
  fastToXY(startX,startY);
  delay(500);
  segmentF(startX, startY, scale, erase);
  segmentA(startX, startY, scale, erase);
  segmentG(startX, startY, scale, erase);
  segmentE(startX, startY, scale, erase);
  segmentD(startX, startY, scale, erase);
  segmentC(startX, startY, scale, erase);
}

void digitSeven(int startX, int startY, int scale, bool erase){
  markerHome();
  fastToXY(startX,startY);
  delay(500);
  segmentA(startX, startY, scale, erase);
  segmentB(startX, startY, scale, erase);
  segmentC(startX, startY, scale, erase);
}

void digitEight(int startX, int startY, int scale, bool erase){
  markerHome();
  fastToXY(startX,startY);
  delay(500);
  segmentF(startX, startY, scale, erase);
  segmentA(startX, startY, scale, erase);
  segmentB(startX, startY, scale, erase);
  segmentG(startX, startY, scale, erase);
  segmentE(startX, startY, scale, erase);
  segmentD(startX, startY, scale, erase);
  segmentC(startX, startY, scale, erase);
}

void digitNine(int startX, int startY, int scale, bool erase){
  markerHome();
  fastToXY(startX,startY);
  delay(500);
  segmentF(startX, startY, scale, erase);
  segmentA(startX, startY, scale, erase);
  segmentB(startX, startY, scale, erase);
  segmentG(startX, startY, scale, erase);
  //delay here? marker up through segments E and D
  segmentC(startX, startY, scale, erase);
}

void dividingColon(int startX, int startY, int scale, bool erase){
  markerHome();
  fastToXY(startX+scale/2,startY+scale+scale/2);
  if(erase) eraserDown();
  else markerDown();
  sendCommand("G3 I0 J4");
  markerHome();
  fastToXY(startX+scale/2,startY+scale/2);
  if(erase) eraserDown();
  else markerDown();
  sendCommand("G2 I0 J4");
  markerHome();
}

void segmentA(int startX, int startY, int scale, bool erase){
  markerHome();
  //(x + space, y + 2 * scale)
  fastToXY(startX + (0.2 * scale), startY + (2 * scale));
  if(erase) eraserDown();
  else markerDown();
  markerDown();
  //(x + scale - space, y + 2 * scale)
  goToXY(startX + scale - (0.2 * scale), startY + (2 * scale));
  markerHome();
}

void segmentB(int startX, int startY, int scale, bool erase){
  markerHome();
  //(x + scale, y + 2 * scale - space)
  fastToXY(startX + scale, startY + (2 * scale) - (0.2 * scale));
  if(erase) eraserDown();
  else markerDown();
  //(x + scale, y + 2 * scale - space)
  goToXY(startX + scale, startY + scale + (0.2 * scale));
  markerHome();
}

void segmentC(int startX, int startY, int scale, bool erase){
  markerHome();
  //(x + scale, y + space)
  fastToXY(startX + scale, startY + (0.2 * scale));
  if(erase) eraserDown();
  else markerDown();
  //(x + scale, y + scale - space)
  goToXY(startX + scale, startY + scale - (0.2 * scale));
  markerHome();
}

void segmentD(int startX, int startY, int scale, bool erase){
  markerHome();
  //(x + space, y)
  fastToXY(startX + (0.2 * scale), startY);
  if(erase) eraserDown();
  else markerDown();
  //(x + scale - space, y)
  goToXY(startX + scale - (0.2 * scale), startY);
  markerHome();
}

void segmentE(int startX, int startY, int scale, bool erase){
  markerHome();
  //(x, y + scale - space)
  fastToXY(startX, startY + scale - 0.2 * scale);
  if(erase) eraserDown();
  else markerDown();
  //(x, y + space)
  goToXY(startX, startY + 0.2 * scale);
  markerHome();
}

void segmentF(int startX, int startY, int scale, bool erase){
  markerHome();
  //(x, y + scale + space)
  fastToXY(startX, startY + scale + (0.2 * scale));
  if(erase) eraserDown();
  else markerDown();
  //(x, y + 2 * scale - space)
  goToXY(startX, startY + (2 * scale) - (0.2 * scale));
  markerHome();
}

void segmentG(int startX, int startY, int scale, bool erase){
  markerHome();
  //(x + scale - space, y + scale)
  fastToXY(startX + scale - (0.2 * scale), startY + scale);
  if(erase) eraserDown();
  else markerDown();
  //(x + space, y + scale)
  goToXY(startX + (0.2 * scale), startY + scale);
  markerHome();
}

void eraserDown(){
  printf(";markerHome\n");
  #ifdef SIMULATE
    sendCommand("G1 Z50");
  #else
    sendCommand("M280 P0 S80");
  #endif
}

void markerHome(){
  printf(";markerRaised\n");
  #ifdef SIMULATE
    sendCommand("G1 Z100");
  #else
    sendCommand("M280 P0 105");
  #endif
}

void markerDown(){
  printf(";markerDown\n");
  #ifdef SIMULATE
    sendCommand("G1 Z0");
  #else
    sendCommand("M280 P0 S130");
  #endif
}

int reHome(bool x, bool y){
  isHomed=true;
  markerHome();
  char xChar = ' ';
  if (x){xChar='X';}
  char yChar = ' ';
  if (y){yChar='Y';}
  char nextCommand[COMMAND_LENGTH];
  snprintf(nextCommand,COMMAND_LENGTH, "G28 %c%c",xChar,yChar);
  return sendCommand(nextCommand);
  
}

int reHome(){
  return reHome(true, true);
  
}

int goToXY(double x, double y){
  char nextCommand[COMMAND_LENGTH];
  snprintf(nextCommand,COMMAND_LENGTH, "G1 X%fY%f F8000",x,y);
  return sendCommand(nextCommand);
}

int fastToXY(double x, double y){
  char nextCommand[COMMAND_LENGTH];
  snprintf(nextCommand,COMMAND_LENGTH, "G0 X%fY%f F15000",x,y);
  return sendCommand(nextCommand);
}
