#include <iostream>
#include <stdio.h>


#define COMMAND_LENGTH 255

//comment for actual device
#define SIMULATE



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
int leftX(int startX, int scale);
int rightX(int startX, int scale);
int topY(int startY, int scale);
int bottomY(int startY, int scale);
void digitZero(int startX, int startY, int scale);


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
      digitZero(0, 0, 50);
    #endif
  #endif
}

void loop() {
  #ifndef SIMULATE
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
  #else //ifdef SIMULATE
    //segmentA(0,0,10);
  #endif
}


int sendCommand(char* command){
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
  segmentA(startX, startY, scale);
  segmentB(startX, startY, scale);
  segmentC(startX, startY, scale);
  segmentD(startX, startY, scale);
  segmentE(startX, startY, scale);
  segmentF(startX, startY, scale);
  segmentG(startX, startY, scale);
}


void segmentA(int startX, int startY, int scale){
  markerUp();
  goToXY(leftX(startX, scale), topY(startY, scale));
  markerDown();
  goToXY(rightX(startX, scale), topY(startY, scale));
  markerUp();
}

void segmentB(int startX, int startY, int scale){
  markerUp();
  goToXY(rightX(startX, scale), topY(startY, scale));
  markerDown();
  goToXY(rightX(startX, scale), bottomY(startY, scale));
  markerUp();  
}

void segmentC(int startX, int startY, int scale){
  markerUp();
  goToXY(rightX(startX, scale), bottomY(startY, scale));
  markerDown();
  goToXY(rightX(startX, scale), bottomY(startY, scale) * 2);
  markerUp();  
}

void segmentD(int startX, int startY, int scale){
  markerUp();
  goToXY(leftX(startX, scale), bottomY(startY, scale) * 2);
  markerDown();
  goToXY(rightX(startX, scale), bottomY(startY, scale) * 2);
  markerUp();
}

void segmentE(int startX, int startY, int scale){
  markerUp();
  goToXY(leftX(startX, scale), bottomY(startY, scale));
  markerDown();
  goToXY(leftX(startX, scale), bottomY(startY, scale) * 2);
  markerUp();   
}

void segmentF(int startX, int startY, int scale){
  markerUp();
  goToXY(leftX(startX, scale), topY(startY, scale));
  markerDown();
  goToXY(leftX(startX, scale), bottomY(startY, scale));
  markerUp(); 
}

void segmentG(int startX, int startY, int scale){
  markerUp();
  goToXY(leftX(startX, scale), bottomY(startY, scale));
  markerDown();
  goToXY(rightX(startX, scale), bottomY(startY, scale));
  markerUp();
}

void markerUp(){
  printf(";markerUp\n");
  sendCommand("M280 P0 S150");
}

void markerDown(){
  printf(";markerDown\n");
  sendCommand("M280 P0 S71");
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

int leftX(int startX, int scale){return ((startX + 5) * scale);}

int rightX(int startX, int scale){return (0.5 * scale + leftX(startX, scale));}

int topY(int startY, int scale){return ((startY + 5) * scale);}

int bottomY(int startY, int scale){return (0.5 * scale + topY(startY, scale));}

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
