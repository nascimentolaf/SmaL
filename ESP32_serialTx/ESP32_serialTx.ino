//sudo chmod a+rw /dev/ttyUSB0

#define B1 1
#define B0 0

//from MSB to LSB
byte getByte(byte b7, byte b6, byte b5, byte b4, byte b3, byte b2, byte b1, byte b0){
  return (b7 << 7) + (b6 << 6) + (b5 << 5) + (b4 << 4) + (b3 << 3) + (b2 << 2) + (b1 << 1) + b0;
}

void setup() {
  Serial.begin(115200);
}

void loop() {  
 byte byte1 = getByte(B0,B1,B0,B0,B0,B0,B0,B0);
 byte byte2 = getByte(B1,B1,B1,B1,B0,B0,B0,B0);
 byte byte3 = 0;
 byte byte4 = 0;

 byte bytes[] = { 
                  byte1, byte2, byte3, byte4,
                  byte1, byte2, byte3, byte4,
                  byte1, byte2, byte3, byte4,
                  byte1, byte2, byte3, byte4,
                  byte1, byte2, byte3, byte4,
                  byte1, byte2, byte3, byte4 }; 
                  
 Serial.write(bytes, sizeof(bytes));
 delay(100);
}
