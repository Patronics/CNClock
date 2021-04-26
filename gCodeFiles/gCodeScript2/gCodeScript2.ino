#include <iostream>
#include <stdio.h>


#define COMMAND_LENGTH 255

void segmentA(int startX, int startY, int scale);
void segmentF(int startX, int startY, int scale);
void markerUp();
void markerDown();
void toPrintStart(int startX, int startY);


void setup(){
  
}

void loop(){
  printf("; actual stuff");
}


int sendCommand(char* command){
  Serial.print(command);
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

int goToXY(double x, double y){
  char nextCommand[COMMAND_LENGTH];
  snprintf(nextCommand,COMMAND_LENGTH, "G1 X%fY%f",x,y);
  sendCommand(nextCommand);
}

/*void toPrintStart(int startX, int startY){
  markerUp();
  printf("G1 X%dY%d", startX, startY);
}*/
