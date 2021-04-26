#include <iostream>
#include <stdio.h>


#define COMMAND_LENGTH 255

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
int reHome(bool x, bool y);
int closeX(int startX, int scale);
int farX(int startX, int scale);
int closeY(int startY, int scale);
int farY(int startY, int scale);

void setup() {
  Serial.begin(250000);

  // Serial1.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  // Serial2.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  // The txPin & rxPin can set to any output pin
  
  Serial1.begin(250000, SERIAL_8N1, 36, 32);
  //Serial2.begin(250000, SERIAL_8N1, 36, 37);
}

void loop() {

  if(Serial.available()) {
    int ch = Serial.read();
    if (ch == '~'){
      reHome();
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
}


int sendCommand(char* command){
  Serial1.print(command);
}


void segmentA(int startX, int startY, int scale){
  markerUp();
  goToXY(((startX + 5) * scale), startY);
  markerDown();
  goToXY((0.5 * scale + startX), startY);
  markerUp();
}

void segmentB(int startX, int startY, int scale){
  markerUp();
  goToXY((0.5 * scale + startX), (startY - 5 * scale));
  markerDown();
  goToXY(startX, (startY - 45 * scale));
  markerUp();  
}
// to do
void segmentC(int startX, int startY, int scale){
  markerUp();
  goToXY((0.5 * scale + startX), (startY - 5 * scale));
  markerDown();
  goToXY(startX, (startY - 45 * scale));
  markerUp();  
}
// to do
/*void segmentD(int startX, int startY, int scale){
  markerUp();
  goToXY((startX + 5) * scale), startY);
  markerDown();
  goToXY((0.5 * scale + startX), startY);
  markerUp();
}
// to do
void segmentE(int startX, int startY, int scale){
  markerUp();
  goToXY(startX, (startY - 5 * scale));
  markerDown();
  goToXY(startX, (startY - 45 * scale));
  markerUp();  
}*/

void segmentF(int startX, int startY, int scale){
  markerUp();
  goToXY(startX, ((startY - 5) * scale));
  markerDown();
  goToXY(startX, (0.5 * scale + startY));
  markerUp();
}
// to do
/*void segmentG(int startX, int startY, int scale){
  markerUp();
  goToXY((startX + 5) * scale), startY);
  markerDown();
  goToXY((0.5 * scale + startX), startY);
  markerUp();
}*/

void markerUp(){
  printf(";markerUp");
  sendCommand("M280 P0 S140");
}

void markerDown(){
  printf(";markerDown");
  sendCommand("M280 P0 S86");
}

int reHome(bool x, bool y){
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

int closeX(int startX, int scale){return ((startX + 5) * scale);}

int farX(int startX, int scale){return (0.5 * scale + closeX(startX, scale));}

int closeY(int startY, int scale){return ((startY + 5) * scale);}

int farY(int startY, int scale){return (0.5 * scale + closeY(startY, scale));}

/*void toPrintStart(int startX, int startY){
  markerUp();
  printf("G1 X%dY%d", startX, startY);
}*/
