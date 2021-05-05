#include <iostream>
#include <stdio.h>
#include <time.h>

#ifdef ARDUINO
  #include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#endif

#define COMMAND_LENGTH 255

#ifndef ARDUINO
  //comment for actual device
  #define SIMULATE
#endif

#define SIMULATE_TIME

#ifdef SIMULATE
  #include <unistd.h>
  void delay(int mill);
#endif

#ifdef WiFiManager_h
  WiFiManager wifiManager;
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

  #ifdef WiFiManager_h
    wifiManager.setCleanConnect(true); // disconnect before connect, clean connect
    wifiManager.setConnectRetries(4);
    wifiManager.setConfigPortalBlocking(false);
    if(wifiManager.autoConnect("CNClock", "SierraRobotics")){
      printf(";connected to network");
      getTime();
    } else {
      printf(";hosting portal, awaiting network connection");
    }
  #endif
}


void loop() {
  #ifdef WiFiManager_h
    wifiManager.process();
  #endif
  
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
  
  drawDigit(1, 0, 0, 55);
  drawDigit(2, 70, 0, 55);
  drawDigit(5, 145, 0, 55);
  drawDigit(9, 220, 0, 55);
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


void getTime() {
  #ifdef WiFiManager_h
    int tz           = -8;   //seems to be ignored?
    int dst          = 1;    //seems to be ignored?
    time_t now       = time(nullptr);
    unsigned timeout = 5000; // try for timeout
    unsigned start   = millis();
    configTime(tz * 3600, dst * 3600, "pool.ntp.org", "time.nist.gov");
    Serial.print("Waiting for NTP time sync: ");
    while (now < 8 * 3600 * 2 ) { // what is this ?
      delay(100);
      Serial.print(".");
      now = time(nullptr);
      if((millis() - start) > timeout){
        Serial.println("\n;[ERROR] Failed to get NTP time.");
        return;
      }
    }
    Serial.println("");
    struct tm timeinfo;
    //gmtime_r(&now, &timeinfo); // @NOTE doesnt work in esp2.3.0
    getLocalTime(&timeinfo, 5000); //see https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-time.c
    //Serial.print("Current time: ");
    int dstShift=0;
      /*if(timeinfo.tm_isdst>0){
        dstShift=1;
        Serial.printf(";dstShifting");
      }*/
      Serial.printf("time: %d/%d/%d %d:%d:%d",timeinfo.tm_year+1900, timeinfo.tm_mon, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  
    //Serial.print(asctime(&timeinfo));
  #endif
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

void drawDigit(int number, int startX, int startY, int scale){
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
