//sudo chmod a+rw /dev/ttyACM0

#define rx_pin 13
#define B1 1
#define B0 0

volatile uint16_t SAMPLING_TIME = 500*1.5; //Microseconds

bool BERmode = true; //se for enviar dados, muda para falso
int bitErrado = 0, total = 192;
String pkt = "010000001111000000000000000000000100000100011100011110101110000101000000010101010001111010111000010000010001101010001111010111000100001001111011001100110011001101000011101100100000000000000000";

byte buff[193] = {};
uint8_t ONEs = 0;
uint8_t pack = 0;

bool checkParity(byte *bitsPKT);
void interruption();
void getBit(bool Bit);
void sendData();
byte getByte(byte b7, byte b6, byte b5, byte b4, byte b3, byte b2, byte b1, byte b0);

void setup() {
  Serial.begin(115200);
  pinMode(rx_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(rx_pin), interruption, CHANGE);
}

void loop() {
  int vmedia = analogRead(A5);
  analogWrite(A1, vmedia / 4);
}

void interruption() {
  delayMicroseconds(SAMPLING_TIME);
  getBit(digitalRead(rx_pin)); //leitura do pino digital para definir o bit
}

void getBit(bool Bit) {  
  if (ONEs == 8) {
    if (pack < 193) {
      buff[pack] = !Bit; //guarda bit no buff
      pack++;
      return;
    }
//    if (checkParity(buff))
    if (true)
      sendData();
    ONEs = 0;
    pack = 0;
    buff[193] = {};
    return;
  }

  if (!Bit)
    ONEs++;
  else
    ONEs = 0;
}

bool checkParity(byte *bitsPKT) {
  char par;
  int counter = 0;
  char test = bitsPKT[192];

  for (int i = 0; i < 192; i++) {
    if (bitsPKT[i] == B1) {
      counter++;
    }
  }

  if (counter % 2)
    par = B0;
  else
    par = B1;

  if (par == test)
    return true;
  else
    return false;
}

void printBits(){
  for (int i = 0; i < sizeof(buff); i++) {
    byte byteHere = buff[i];
    if (!byteHere) {
      Serial.print("0");
      continue;
    }
    Serial.print("1");
  }
  Serial.println("");
}

void calculaBer(){
  total = total+192;
  
  char bitHere;
  
  for(uint8_t j = 0; j < 192; j++){
    if(buff[j])
     bitHere = '1';
    else bitHere = '0';
    
    if(pkt[j] != bitHere)
      bitErrado++;
  }
  
  float BER = (float)bitErrado/total;
  Serial.print("Bits errados: ");
  Serial.println(bitErrado);
  Serial.print("N bits: ");
  Serial.println(total);
  Serial.print("BER: ");
  Serial.println(BER);
}

void sendData() { 
  byte bytes[4 * 6]; //4 * nPayload
  
  uint8_t j = 0;
  for (uint8_t i = 0; i < 192; i += 32) {
    bytes[(i / 32) + 0 + j] = getByte(buff[i], buff[i + 1], buff[i + 2], buff[i + 3], buff[i + 4], buff[i + 5], buff[i + 6], buff[i + 7]);
    bytes[(i / 32) + 1 + j] = getByte(buff[i + 8], buff[i + 9], buff[i + 10], buff[i + 11], buff[i + 12], buff[i + 13], buff[i + 14], buff[i + 15]);
    bytes[(i / 32) + 2 + j] = getByte(buff[i + 16], buff[i + 17], buff[i + 18], buff[i + 19], buff[i + 20], buff[i + 21], buff[i + 22], buff[i + 23]);
    bytes[(i / 32) + 3 + j] = getByte(buff[i + 24], buff[i + 25], buff[i + 26], buff[i + 27], buff[i + 28], buff[i + 29], buff[i + 30], buff[i + 31]);
    j += 3;
  }

  if(BERmode) //saida em monitor serial
    calculaBer();
  else
    Serial.write(bytes, sizeof(bytes)); //sending bits via serial port

  //printBits(); //debug
}

byte getByte(byte b7, byte b6, byte b5, byte b4, byte b3, byte b2, byte b1, byte b0) {
  return (b7 << 7) + (b6 << 6) + (b5 << 5) + (b4 << 4) + (b3 << 3) + (b2 << 2) + (b1 << 1) + b0;
}
