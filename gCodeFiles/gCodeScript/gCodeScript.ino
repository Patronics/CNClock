#include <iostream>
#include <stdio.h>


#define COMMAND_LENGTH 255

//comment for actual device
//#define SIMULATE



void segmentA(int startX, int startY, int scale);
void segmentB(int startX, int startY, int scale);
void segmentC(int startX, int startY, int scale);
void segmentD(int startX, int startY, int scale);
void segmentE(int startX, int startY, int scale);
void segmentF(int startX, int startY, int scale);
void segmentG(int startX, int startY, int scale);
void markerUp();
void markerDown();
void toPrintStart(int startX, int startY);
int reHome();
int goToXY(double x, double y);
int reHome(bool x, bool y);
/* non used prototypes from previous algorithm
int leftX(int startX, int scale);
int rightX(int startX, int scale);
int topY(int startY, int scale);
int bottomY(int startY, int scale);
*/
void digitZero(int startX, int startY, int scale);
void digitOne(int startX, int startY, int scale);
void digitTwo(int startX, int startY, int scale);
void digitThree(int startX, int startY, int scale);
void digitFour(int startX, int startY, int scale);
void digitFive(int startX, int startY, int scale);
void digitSix(int startX, int startY, int scale);
void digitSeven(int startX, int startY, int scale);
void digitEight(int startX, int startY, int scale);
void digitNine(int startX, int startY, int scale);



void setup() {
  #ifndef SIMULATE
  Serial.begin(250000);

  // Serial1.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  // Serial2.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  // The txPin & rxPin can set to any output pin
  
  Serial1.begin(250000, SERIAL_8N1, 36, 32);
  //Serial2.begin(250000, SERIAL_8N1, 36, 37);
  #endif
  #ifdef SIMULATE
    #ifdef FILEOUT
      //TODO: open output file
    #else //Not FILEOUT
      printf("starting simulated output\n");
      doDemo();
    #endif
  #endif
}

void loop() {
  #ifndef SIMULATE
    if(Serial.available()) {
      int ch = Serial.read();
      if (ch == '~'){
        doDemo();
      }
      Serial1.write(ch);
      //Serial.write(ch);
    }
  
    /*if(Serial2.available()) {
      int ch = Serial2.read();
      Serial2.write(ch);
    }*/
    
    if(Serial1.available()) {
      int ch = Serial1.read();
      Serial.write(ch);
    }
  #else //ifdef SIMULATE
    //segmentA(0,0,10);
  #endif
}

void doDemo(){
  digitEight(0, 0, 50);
  digitTwo(100, 0, 50);
}

int sendCommand(char* command){
    delay(2000);
  #ifndef SIMULATE
    Serial1.print(command);
    Serial1.print("\n");
  #endif
  #ifdef SIMULATE
    #ifndef FILEOUT
      printf("%s\n",command);
    #endif
  #endif
}

void digitZero(int startX, int startY, int scale){
  int ms = 1;
  delay(ms);
  segmentA(startX, startY, scale);
  delay(ms);
  segmentB(startX, startY, scale);
  delay(ms);
  segmentC(startX, startY, scale);
  delay(ms);
  segmentD(startX, startY, scale);
  delay(ms);
  segmentE(startX, startY, scale);
  delay(ms);
  segmentF(startX, startY, scale);
  delay(ms);
}

void digitOne(int startX, int startY, int scale){
  segmentB(startX, startY, scale);
  segmentC(startX, startY, scale);
}

void digitTwo(int startX, int startY, int scale){
  segmentA(startX, startY, scale);
  segmentB(startX, startY, scale);
  segmentD(startX, startY, scale);
  segmentE(startX, startY, scale);
  segmentG(startX, startY, scale);
}

void digitThree(int startX, int startY, int scale){
  segmentA(startX, startY, scale);
  segmentB(startX, startY, scale);
  segmentC(startX, startY, scale);
  segmentD(startX, startY, scale);
  segmentG(startX, startY, scale);
}

void digitFour(int startX, int startY, int scale){
  segmentB(startX, startY, scale);
  segmentC(startX, startY, scale);
  segmentD(startX, startY, scale);
  segmentF(startX, startY, scale);
  segmentG(startX, startY, scale);
}

void digitFive(int startX, int startY, int scale){
  segmentA(startX, startY, scale);
  segmentC(startX, startY, scale);
  segmentD(startX, startY, scale);
  segmentF(startX, startY, scale);
  segmentG(startX, startY, scale);
}

void digitSix(int startX, int startY, int scale){
  segmentA(startX, startY, scale);
  segmentC(startX, startY, scale);
  segmentD(startX, startY, scale);
  segmentE(startX, startY, scale);
  segmentF(startX, startY, scale);
  segmentG(startX, startY, scale);
}

void digitSeven(int startX, int startY, int scale){
  segmentA(startX, startY, scale);
  segmentB(startX, startY, scale);
  segmentC(startX, startY, scale);
}

void digitEight(int startX, int startY, int scale){
  segmentA(startX, startY, scale);
  segmentB(startX, startY, scale);
  segmentC(startX, startY, scale);
  segmentD(startX, startY, scale);
  segmentE(startX, startY, scale);
  segmentF(startX, startY, scale);
  segmentG(startX, startY, scale);
}

void digitNine(int startX, int startY, int scale){
  segmentA(startX, startY, scale);
  segmentB(startX, startY, scale);
  segmentC(startX, startY, scale);
  segmentF(startX, startY, scale);
  segmentG(startX, startY, scale);
}


void segmentA(int startX, int startY, int scale){
  markerUp();
  //(x + space, y + 2 * scale)
  goToXY(startX + (0.2 * scale), startY + (2 * scale));
  markerDown();
  //(x + scale - space, y + 2 * scale)
  goToXY(startX + scale - (0.2 * scale), startY + (2 * scale));
  markerUp();
}

void segmentB(int startX, int startY, int scale){
  markerUp();
  //(x + scale, y + scale + space)
  goToXY(startX + scale, startY + scale + (0.2 * scale));
  markerDown();
  //(x + scale, y + 2 * scale - space)
  goToXY(startX + scale, startY + (2 * scale) - (0.2 * scale));
  markerUp(); 
}

void segmentC(int startX, int startY, int scale){
  markerUp();
  //(x + scale, y + space)
  goToXY(startX + scale, startY + (0.2 * scale));
  markerDown();
  //(x + scale, y + scale - space)
  goToXY(startX + scale, startY + scale - (0.2 * scale));
  markerUp();   
}

void segmentD(int startX, int startY, int scale){
  markerUp();
  //(x + space, y)
  goToXY(startX + (0.2 * scale), startY);
  markerDown();
  //(x + scale - space, y)
  goToXY(startX + scale - (0.2 * scale), startY);
  markerUp();
}

void segmentE(int startX, int startY, int scale){
  markerUp();
  //(x, y + space)
  goToXY(startX, startY + 0.2 * scale);
  markerDown();
  //(x, y + scale - space)
  goToXY(startX, startY + scale - 0.2 * scale);
  markerUp();   
}

void segmentF(int startX, int startY, int scale){
  markerUp();
  //(x, y + scale + space)
  goToXY(startX, startY + scale + (0.2 * scale));
  markerDown();
  //(x, y + 2 * scale - space)
  goToXY(startX, startY + (2 * scale) - (0.2 * scale));
  markerUp(); 
}

void segmentG(int startX, int startY, int scale){
  markerUp();
  //(x + space, y + scale)
  goToXY(startX + (0.2 * scale), startY + scale);
  markerDown();
  //(x + scale - space, y + scale)
  goToXY(startX + scale - (0.2 * scale), startY + scale);
  markerUp();
}

void markerUp(){
  printf(";markerUp\n");
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
    sendCommand("M280 P0 S150");
  #endif
}

void markerDown(){
  printf(";markerDown\n");
  #ifdef SIMULATE
    sendCommand("G1 Z0");
  #else
    sendCommand("M280 P0 S71");
  #endif
}

int reHome(bool x, bool y){
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
  snprintf(nextCommand,COMMAND_LENGTH, "G1 X%fY%f",x,y);
  return sendCommand(nextCommand);
}

/* non used  functions from previous algorithm
int leftX(int startX, int scale){return ((startX + scale));}

int rightX(int startX, int scale){return (0.5 * scale - leftX(startX, scale));}

int topY(int startY, int scale){return ((startY + scale));}

int bottomY(int startY, int scale){return (0.5 * scale - topY(startY, scale));}
*/

/*void toPrintStart(int startX, int startY){
  markerUp();
  printf("G1 X%dY%d", startX, startY);
}*/


#ifdef SIMULATE
  int main(void){
    setup();
    while(true){loop();}
  }
#endif
