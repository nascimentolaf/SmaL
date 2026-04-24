#define B1 1
#define B0 0

// Assembles a byte from 8 individual bit arguments, ordered MSB (b7) to LSB (b0)
// Example: getByte(0,1,0,0,0,0,0,0) == 0b01000000 == 64
byte getByte(byte b7, byte b6, byte b5, byte b4, byte b3, byte b2, byte b1, byte b0)
{
  return (b7 << 7) + (b6 << 6) + (b5 << 5) + (b4 << 4) + (b3 << 3) + (b2 << 2) + (b1 << 1) + b0;
}

void setup()
{
  Serial.begin(115200); // Start serial communication at 115200 baud
}

void loop()
{
  byte byte1 = getByte(B0, B1, B0, B0, B0, B0, B0, B0); // 0b01000000 = 64
  byte byte2 = getByte(B1, B1, B1, B1, B0, B0, B0, B0); // 0b11110000 = 240
  byte byte3 = 0;
  byte byte4 = 0;

  // Build a packet by repeating the 4-byte pattern 6 times (24 bytes total)
  byte bytes[] = {
      byte1, byte2, byte3, byte4,
      byte1, byte2, byte3, byte4,
      byte1, byte2, byte3, byte4,
      byte1, byte2, byte3, byte4,
      byte1, byte2, byte3, byte4,
      byte1, byte2, byte3, byte4};

  Serial.write(bytes, sizeof(bytes)); // Send all 24 bytes over serial
  delay(100);                         // Wait 100ms before sending the next packet
}