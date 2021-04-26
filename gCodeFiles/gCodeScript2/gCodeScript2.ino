#include <iostream>
#include <stdio.h>


#define COMMAND_LENGTH 255

void segmentA(int startX, int startY, int scale);
void segmentF(int startX, int startY, int scale);
void markerUp();
void markerDown();
void toPrintStart(int startX, int startY);
int reHome();
int reHome(bool x, bool y);

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
  int x = (startX + 5) * 5;
  toPrintStart(x, startY);
  markerDown();
  printf("G1 X%fY%d",(0.5 * scale + startX), startY);
  markerUp();
}

void segmentF(int startX, int startY, int scale){
  markerUp();
  goToXY(startX, ((startY - 5) * scale));
  markerDown();
  goToXY((0.5 * scale + startX), (0.5 * scale + startY));
  markerUp();
}

void markerUp(){
  printf(";markerUp");
}

void markerDown(){
  printf(";markerDown");
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

/*void toPrintStart(int startX, int startY){
  markerUp();
  printf("G1 X%dY%d", startX, startY);
}*/
