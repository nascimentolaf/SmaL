// sudo chmod a+rw /dev/ttyACM0

#define rx_pin 13 // Pin used to receive serial data
#define B1 1      // Bit value 1
#define B0 0      // Bit value 0

volatile uint16_t SAMPLING_TIME = 500 * 1.5; // Sampling delay in microseconds

bool BERmode = true;                                                                                                                                                                                             // If true, calculates BER instead of forwarding data over serial
int bitError = 0, total = 192;                                                                                                                                                                                   // Accumulated error count and total bits processed for BER calculation
String pkt = "010000001111000000000000000000000100000100011100011110101110000101000000010101010001111010111000010000010001101010001111010111000100001001111011001100110011001101000011101100100000000000000000"; // Reference packet (192 bits) used to compare against received bits when calculating BER

byte buff[193] = {}; // Bit buffer: 192 data bits + 1 parity bit
uint8_t ONEs = 0;    // Counts consecutive 1s to detect the 8-ones sync/flag sequence
uint8_t pack = 0;    // Index of the next bit to be written into buff[]

// Forward declarations
bool checkParity(byte *bitsPKT);
void interruption();
void getBit(bool Bit);
void sendData();
byte getByte(byte b7, byte b6, byte b5, byte b4, byte b3, byte b2, byte b1, byte b0);

void setup()
{
  Serial.begin(115200);
  pinMode(rx_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(rx_pin), interruption, CHANGE); // Trigger on any edge (rising or falling)
}

void loop()
{
  // Read analog voltage on A5 and mirror it scaled to A1 (vAverage / 4 maps 0-1023 to 0-255 for analogWrite)
  int vAverage = analogRead(A5);
  analogWrite(A1, vAverage / 4);
}

// ISR: triggered on any edge of rx_pin
void interruption()
{
  delayMicroseconds(SAMPLING_TIME); // Wait until the middle of the bit window before sampling
  getBit(digitalRead(rx_pin));      // Read and process the bit
}

// Processes one received bit
void getBit(bool Bit)
{
  if (ONEs == 8)
  { // 8 consecutive 1s detected — this marks the start of a data frame
    if (pack < 193)
    {
      buff[pack] = !Bit; // Invert bit (line encoding convention) and store in buffer
      pack++;
      return;
    }
    // Buffer is full (193 bits received): validate and dispatch the packet
    // if (true)
    if (checkParity(buff)) // Parity check
      sendData();
    // Reset state for next packet
    ONEs = 0;
    pack = 0;
    buff[193] = {};
    return;
  }

  // Count consecutive 1s to detect the sync sequence; any 0 resets the counter
  if (!Bit)
    ONEs++;
  else
    ONEs = 0;
}

// Checks even parity: counts 1s in the first 192 bits and compares to the parity bit at index 192
bool checkParity(byte *bitsPKT)
{
  char parityBit;
  int counter = 0;
  char test = bitsPKT[192]; // The received parity bit

  for (int i = 0; i < 192; i++)
  {
    if (bitsPKT[i] == B1)
    {
      counter++;
    }
  }

  // Even parity: parity bit should be 0 if count is odd, 1 if even
  if (counter % 2)
    parityBit = B0;
  else
    parityBit = B1;

  if (parityBit == test)
    return true;
  else
    return false;
}

// Prints the entire bit buffer to Serial as a string of '0's and '1's (used for debugging)
void printBits()
{
  for (int i = 0; i < sizeof(buff); i++)
  {
    byte byteHere = buff[i];
    if (!byteHere)
    {
      Serial.print("0");
      continue;
    }
    Serial.print("1");
  }
  Serial.println("");
}

// Compares received bits against the reference packet and prints BER statistics
void calculateBER()
{
  total = total + 192; // Accumulate total bit count across packets

  char bitHere;

  for (uint8_t j = 0; j < 192; j++)
  {
    if (buff[j])
      bitHere = '1';
    else
      bitHere = '0';

    if (pkt[j] != bitHere)
      bitError++; // Count mismatched bits
  }

  float BER = (float)bitError / total;
  Serial.print("Bit errors: ");
  Serial.println(bitError);
  Serial.print("Total bits: ");
  Serial.println(total);
  Serial.print("BER: ");
  Serial.println(BER);
}

// Packs the 192-bit buffer into 24 bytes and either computes BER or sends the bytes over serial
void sendData()
{
  byte bytes[4 * 6]; // Output buffer: 192 bits / 8 = 24 bytes (4 bytes * 6 payloads)

  // Walk through buff[] in 32-bit chunks, packing every 8 bits into one byte
  uint8_t j = 0;
  for (uint8_t i = 0; i < 192; i += 32)
  {
    bytes[(i / 32) + 0 + j] = getByte(buff[i], buff[i + 1], buff[i + 2], buff[i + 3], buff[i + 4], buff[i + 5], buff[i + 6], buff[i + 7]);             // bits 0-7
    bytes[(i / 32) + 1 + j] = getByte(buff[i + 8], buff[i + 9], buff[i + 10], buff[i + 11], buff[i + 12], buff[i + 13], buff[i + 14], buff[i + 15]);   // bits 8-15
    bytes[(i / 32) + 2 + j] = getByte(buff[i + 16], buff[i + 17], buff[i + 18], buff[i + 19], buff[i + 20], buff[i + 21], buff[i + 22], buff[i + 23]); // bits 16-23
    bytes[(i / 32) + 3 + j] = getByte(buff[i + 24], buff[i + 25], buff[i + 26], buff[i + 27], buff[i + 28], buff[i + 29], buff[i + 30], buff[i + 31]); // bits 24-31
    j += 3;                                                                                                                                            // Advance output index by 3 extra to account for the 4 bytes written per iteration
  }

  if (BERmode) // BER mode: print statistics to Serial Monitor
    calculateBER();
  else
    Serial.write(bytes, sizeof(bytes)); // Data mode: forward packed bytes over serial

  // printBits(); // Uncomment to debug raw bit buffer
}

// Assembles a byte from 8 individual bit arguments, ordered MSB (b7) to LSB (b0)
byte getByte(byte b7, byte b6, byte b5, byte b4, byte b3, byte b2, byte b1, byte b0)
{
  return (b7 << 7) + (b6 << 6) + (b5 << 5) + (b4 << 4) + (b3 << 3) + (b2 << 2) + (b1 << 1) + b0;
}