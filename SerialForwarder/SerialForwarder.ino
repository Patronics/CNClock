/*
 * Heltec Automation ESP32 Serial 1 & 2 example.
 * shows the usage of all 3 hardware uarts
 * work with ESP32's IO MUX
 * 
 * to test the software and hardware
 * wire Rx1 to Tx1 and Rx2 to Tx2
 * type text in serial monitor
 * text will walk trough all 2 serials
 * result is echoed to serial (usb)
 * 
 * 
 * Modified to only use 2 UART pairs, 
 * Useful to verify connection between EESP32 and marlin,
 * and for sending gcode commands manually
*/


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
