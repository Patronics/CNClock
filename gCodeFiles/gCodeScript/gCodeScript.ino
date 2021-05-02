#include <iostream>
#include <stdio.h>


#define COMMAND_LENGTH 255

//comment for actual device
//#define SIMULATE

#define SIMULATE_TIME

#ifdef SIMULATE
  #include <unistd.h>
  void delay(int mill);
#endif




void segmentA(int startX, int startY, int scale);
void segmentB(int startX, int startY, int scale);
void segmentC(int startX, int startY, int scale);
void segmentD(int startX, int startY, int scale);
void segmentE(int startX, int startY, int scale);
void segmentF(int startX, int startY, int scale);
void segmentG(int startX, int startY, int scale);
void markerUp();
void markerDown();
int reHome();
void doDemo();
void markerHome();
int goToXY(double x, double y);
int fastToXY(double x, double y);
int reHome(bool x, bool y);

void drawDigit(int startX, int startY, int scale, int number);


//void digitForm(int startX, int startY, int scale); //digitForm identical to digitEight
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
  printf(";commands:\n Valid GCode, \n @ to rehome, \n ~ to draw demo\n");
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
      else if (ch == '@'){
        reHome();
      } else {
        Serial1.write(ch);
      }
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
  
  digitOne(0, 0, 55);
  digitTwo(70, 0, 55);
  digitFive(145, 0, 55);
  digitNine(220, 0, 55);
  markerHome();

  /*used to check all gigits are properly formed
  digitZero(0, 0, 65);
  digitOne(100, 0, 65);
  digitTwo(200, 0, 65);
  digitThree(300, 0, 65);
  digitFour(400, 0, 65);
  digitFive(500, 0, 65);
  digitSix(600, 0, 65);
  digitSeven(700, 0, 65);
  digitEight(800, 0l 65);
  digitNine(900, 0, 65);
  */
}

int sendCommand(char* command){
  
    delay(500);
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

void drawDigit(int startX, int startY, int scale, int number){
  switch (number){
    case 0:
      digitZero(startX, startY, scale);
      break;
    case 1:
      digitOne(startX, startY, scale);
      break;
    case 2:
      digitTwo(startX, startY, scale);
      break;
    case 3:
      digitThree(startX, startY, scale);
      break;
    case 4:
      digitFour(startX, startY, scale);
      break;
    case 5:
      digitFive(startX, startY, scale);
      break;
    case 6:
      digitSix(startX, startY, scale);
      break;
    case 7:
      digitSeven(startX, startY, scale);
      break;
    case 8:
      digitEight(startX, startY, scale);
      break;
    case 9:
      digitNine(startX, startY, scale);
      break;
    default:
      printf(";error, invalid number passed to drawDigit");
  }
  
  
}


void digitForm(int startX, int startY, int scale){
  markerUp();
  fastToXY(startX,startY);
  delay(500);
  segmentF(startX, startY, scale);
  segmentA(startX, startY, scale);
  segmentB(startX, startY, scale);
  segmentG(startX, startY, scale);
  segmentE(startX, startY, scale);
  segmentD(startX, startY, scale);
  segmentC(startX, startY, scale);
}


void digitZero(int startX, int startY, int scale){
  markerUp();
  fastToXY(startX,startY);
  delay(500);
  segmentF(startX, startY, scale);
  segmentA(startX, startY, scale);
  segmentB(startX, startY, scale);
  //delay here? Marker up through segment G
  segmentE(startX, startY, scale);
  segmentD(startX, startY, scale);
  segmentC(startX, startY, scale);
 
  
  /* previous implimentation of digit zero
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
  */
}

void digitOne(int startX, int startY, int scale){
  markerUp();
  fastToXY(startX,startY);
  delay(500);
  segmentB(startX, startY, scale);
  //delay here? Skips a bunch, goes all the way down, then up to the middle
  segmentC(startX, startY, scale);
}

void digitTwo(int startX, int startY, int scale){
  markerUp();
  fastToXY(startX,startY);
  delay(500);
  segmentA(startX, startY, scale);
  segmentB(startX, startY, scale);
  segmentG(startX, startY, scale);
  segmentE(startX, startY, scale);
  segmentD(startX, startY, scale);
}

void digitThree(int startX, int startY, int scale){
  markerUp();
  fastToXY(startX,startY);
  delay(500);
  segmentA(startX, startY, scale);
  segmentB(startX, startY, scale);
  segmentG(startX, startY, scale);
  // delay here? marker up through segment E 
  segmentD(startX, startY, scale);
  segmentC(startX, startY, scale);
}

void digitFour(int startX, int startY, int scale){
  markerUp();
  fastToXY(startX,startY);
  delay(500);
  segmentF(startX, startY, scale);
  //delay here? marker up through segment A
  segmentB(startX, startY, scale);
  segmentG(startX, startY, scale);
  //delay here? marker up through segments E and D
  segmentC(startX, startY, scale);
}

void digitFive(int startX, int startY, int scale){
  markerUp();
  fastToXY(startX,startY);
  delay(500);
  segmentF(startX, startY, scale);
  segmentA(startX, startY, scale);
  //delay here? marker up through segment B
  segmentG(startX, startY, scale);
  //delay here? marker up through segment g
  segmentD(startX, startY, scale);
  segmentC(startX, startY, scale);
}

void digitSix(int startX, int startY, int scale){
  markerUp();
  fastToXY(startX,startY);
  delay(500);
  segmentF(startX, startY, scale);
  segmentA(startX, startY, scale);
  //delay here? marker up through segment B
  segmentG(startX, startY, scale);
  segmentE(startX, startY, scale);
  segmentD(startX, startY, scale);
  segmentC(startX, startY, scale);
}

void digitSeven(int startX, int startY, int scale){
  markerUp();
  fastToXY(startX,startY);
  delay(500);
  segmentA(startX, startY, scale);
  segmentB(startX, startY, scale);
  //delay here? marker up through segments G, E and D
  segmentC(startX, startY, scale);
}

void digitEight(int startX, int startY, int scale){
  markerUp();
  fastToXY(startX,startY);
  delay(500);
  segmentF(startX, startY, scale);
  segmentA(startX, startY, scale);
  segmentB(startX, startY, scale);
  segmentG(startX, startY, scale);
  segmentE(startX, startY, scale);
  segmentD(startX, startY, scale);
  segmentC(startX, startY, scale);
}

void digitNine(int startX, int startY, int scale){
  markerUp();
  fastToXY(startX,startY);
  delay(500);
  segmentF(startX, startY, scale);
  segmentA(startX, startY, scale);
  segmentB(startX, startY, scale);
  segmentG(startX, startY, scale);
  //delay here? marker up through segments E and D
  segmentC(startX, startY, scale);
}


void segmentA(int startX, int startY, int scale){
  markerUp();
  //(x + space, y + 2 * scale)
  fastToXY(startX + (0.2 * scale), startY + (2 * scale));
  markerDown();
  //(x + scale - space, y + 2 * scale)
  goToXY(startX + scale - (0.2 * scale), startY + (2 * scale));
  markerUp();
}

void segmentB(int startX, int startY, int scale){
  markerUp();
  //(x + scale, y + 2 * scale - space)
  fastToXY(startX + scale, startY + (2 * scale) - (0.2 * scale));
  markerDown();
  markerDown();
  //(x + scale, y + 2 * scale - space)
  goToXY(startX + scale, startY + scale + (0.2 * scale));
  markerUp(); 
}

void segmentC(int startX, int startY, int scale){
  markerUp();
  //(x + scale, y + space)
  fastToXY(startX + scale, startY + (0.2 * scale));
  markerDown();
  //(x + scale, y + scale - space)
  goToXY(startX + scale, startY + scale - (0.2 * scale));
  markerUp();   
}

void segmentD(int startX, int startY, int scale){
  markerUp();
  //(x + space, y)
  fastToXY(startX + (0.2 * scale), startY);
  markerDown();
  //(x + scale - space, y)
  goToXY(startX + scale - (0.2 * scale), startY);
  markerUp();
}

void segmentE(int startX, int startY, int scale){
  markerUp();
  //(x, y + scale - space)
  fastToXY(startX, startY + scale - 0.2 * scale);
  markerDown();
  //(x, y + space)
  goToXY(startX, startY + 0.2 * scale);
  markerUp();   
}

void segmentF(int startX, int startY, int scale){
  markerUp();
  //(x, y + scale + space)
  fastToXY(startX, startY + scale + (0.2 * scale));
  markerDown();
  //(x, y + 2 * scale - space)
  goToXY(startX, startY + (2 * scale) - (0.2 * scale));
  markerUp(); 
}

void segmentG(int startX, int startY, int scale){
  markerUp();


  //(x + scale - space, y + scale)
  fastToXY(startX + scale - (0.2 * scale), startY + scale);
  markerDown();
  //(x + space, y + scale)
  goToXY(startX + (0.2 * scale), startY + scale);
  markerUp();
}

void markerUp(){
  printf(";markerUp\n");
  #ifdef SIMULATE
    sendCommand("G1 Z50");
  #else
    sendCommand("M280 P0 S85");
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
    sendCommand("M280 P0 S74");
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
  snprintf(nextCommand,COMMAND_LENGTH, "G1 X%fY%f F8000",x,y);
  return sendCommand(nextCommand);
}

int fastToXY(double x, double y){
  char nextCommand[COMMAND_LENGTH];
  snprintf(nextCommand,COMMAND_LENGTH, "G0 X%fY%f F15000",x,y);
  return sendCommand(nextCommand);
}


#ifdef SIMULATE
  int main(void){
    setup();
    while(true){loop();}
  }

  void delay(int mill){
    #ifdef SIMULATE_TIME
      usleep(mill*1000);
    #endif
  }
#endif
