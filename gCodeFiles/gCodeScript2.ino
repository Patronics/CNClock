#include <iostream>
#include <stdio.h>


int main() {
  setup();
}

void setup(){
  while(true){
    loop();
  }
}

void loop(){
  printf("; actual stuff");
}


void segmentA(int startX, int startY, int scale){
  toPrintStart(((startX + 5)) * scale), startY);
  markerDown();
  printf("G1 X%fY%d",(0.5 * scale + startX), startY);
  markerUp();
}

void segmentF(int startX, int startY, int scale){
  toPrintStart(startX, ((startY - 5) * scale));
  markerDown();
  printf("G1 X%fY%f",(0.5 * scale + startX), (0.5 * scale + startY));
  markerUp();
}

void markerUp(){
  printf(";markerUp");
}

void markerDown(){
  printf(";markerDown");
}

void toPrintStart(int startX, int startY){
  markerUp();
  printf("G1 X%dY%d", startX, startY);
  }
