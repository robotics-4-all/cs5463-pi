#include <time.h>
#include "cs5463.h"

int initSpi(int channel, int speed){
  int spi =  wiringPiSPISetup (channel, speed) ;
  return spi;
}

int spiWR(int channel, unsigned char *data, int len) {
  wiringPiSPIDataRW(channel, data, len);
}

void resetChip(int rstPin) {
  pinMode(rstPin, OUTPUT);
  digitalWrite(rstPin, 0);
  delay(100);
  digitalWrite(rstPin, 1);
  delay(1000);
}

void init(void) {
  wiringPiSetup();
  pinMode(IRQ_PIN, INPUT);
  resetChip(RESET_PIN);
  initSpi(SPI_CHANNEL, SPI_SPEED);
  while (getCycleCount() == 0) {
    delay(100);
  }
  setPage0();
}

void setPage0(){
  unsigned char buffer[4] = {0x7E,0x00,0x00,0x0};
  spiWR(0,buffer,4);
}

void setPage1() {
  unsigned char buffer[4] = {0x7E,0x00,0x00,0x01};
  spiWR(0, buffer, 4);
}

void setPage2() {
  unsigned char buffer[4] = {0x7E,0x00,0x00,0x02};
  spiWR(0, buffer, 4);
}

void setPage3() {
  unsigned char buffer[4] = {0x7E,0x00,0x00,0x03};
  spiWR(0, buffer, 4);
}

double getIstantaneusCurrent(){
  Register reg = getRegister(7);
  /* printf("Insta-Current: %0.9f\n", range_1_sign(&reg)); */
  return range_1_sign(&reg);
}

double getIstantaneusVolt(){
  Register reg = getRegister(8);
  double val, v = 0;
  /* printf("Insta-Volt: %0.9f\n", range_1_sign(&reg)); */
  val = range_1_sign(&reg);
  v = val * 297;
  return v;
}

double getIstantaneusPower(){
  Register reg = getRegister(9);
  /* printf("Insta-Power: %0.9f\n", range_1_sign(&reg)); */
  return range_1_sign(&reg);
}

double getRealPower(void) {
  Register reg = getRegister(10);
  /* printf("Real-Power: %0.9f\n", range_1_sign(&reg)); */
  return range_1_sign(&reg);
}

double getRMSVolt(void){
  Register reg = getRegister(12);
  double val, vrms = 0;
  /* printf("RMS Volt: %0.9f\n", binConvert(&reg, 0.5)); */
  val = binConvert(&reg, 0.5);
  vrms = val * 297 / 0.707;
  return vrms;
}

double getRMSCurrent(void){
  Register reg = getRegister(11);
  /* printf("RMS Current: %0.9f\n", binConvert(&reg, 0.5)); */
  return binConvert(&reg, 0.5);
}

double getCurrentOffset(void) {
  Register reg = getRegister(1);
  /* printf("Current offset: %0.9f\n", range_1_sign(&reg)); */
  return range_1_sign(&reg);
}

void setCurrentOffset(int offset) {
  writeRegister(1, offset);
}

double getVoltageOffset(void) {
  Register reg = getRegister(3);
  /* printf("Voltage offset: %0.9f\n", range_1_sign(&reg)); */
  return range_1_sign(&reg);
}

void setVoltageOffset(int offset) {
  writeRegister(3, offset);
}

double getCurrentGain(void) {
  Register reg = getRegister(2);
  /* printf("Current Gain: %0.9f\n", binConvert(&reg, 2)); */
  return binConvert(&reg, 2);
}

void setCurrentGain(unsigned int gain) {
  writeRegister(2, (int)gain);
}

double getVoltageGain(void) {
  Register reg = getRegister(4);
  /* printf("Voltage Gain: %0.9f\n", binConvert(&reg, 2)); */
  return binConvert(&reg, 2);
}

void setVoltageGain(unsigned int gain) {
  writeRegister(4, (int)gain);
}

double getTemperature(void) {
  Register reg = getRegister(19);
  int sign = (reg.bytes[3] & 0x80) ? -1 : 1;
  reg.bytes[3] = reg.bytes[3] & 0x7F;
  double val = binConvert(&reg, 128) * sign;;
  /* printf("Temperature: %0.9f\n", val); */
  return val;
}

unsigned int getCycleCount(void) {
  Register reg = getRegister(5);
  unsigned int val = (((unsigned int)reg.bytes[1]) * 0x10000)+(((unsigned int)reg.bytes[2]) * 0x100)+(((unsigned int)reg.bytes[3]));
  /* printf("Cycle Count: %d\n", val); */
  return val;
}

void setCycleCount(unsigned int cycles) {
  writeRegister(5, (int)cycles);
}

double pulseRage(void) {
  Register reg = getRegister(6);
  /* printf("Pulse Rate-E: %0.9f\n", range_1_sign(&reg)); */
  return range_1_sign(&reg);
}

double getEpsilon(void){
  Register reg = getRegister(13);
  /* printf("Epsion: %0.9f\n", range_1_sign(&reg)); */
  return range_1_sign(&reg);
}

double getPowerOffset(void) {
  Register reg = getRegister(14);
  /* printf("Power Offset: %0.9f\n", range_1_sign(&reg)); */
  return range_1_sign(&reg);
}

void setPowerOffset(unsigned int offset) {
  writeRegister(14, (int)offset);
}

double getCurrentACOffset(void) {
  Register reg = getRegister(16);
  /* printf("Current AC Offset: %0.9f\n", range_1_sign(&reg)); */
  return range_1_sign(&reg);
}

void setCurrentACOffset(unsigned int offset) {
  writeRegister(16, (int)offset);
}

double getVoltageACOffset(void) {
  Register reg = getRegister(17);
  /* printf("Voltage AC Offset: %0.9f\n", range_1_sign(&reg)); */
  return range_1_sign(&reg);
}

void setVoltageACOffset(unsigned int offset) {
  writeRegister(17, (int)offset);
}

void getOperationMode(void){
  Register status = getRegister(18);
  if (status.bytes[2] & 0x02){
    printf("Operation Mode: E2MODE\n");
  }
  if (status.bytes[2] & 0x01){
    printf("Operation Mode: XVDEL\n");
  }
  if (status.bytes[3] & 0x80){
    printf("Operation Mode: XIDEL\n");
  }
  if (status.bytes[3] & 0x40){
    printf("Operation Mode: IHPF\n");
  }
  if (status.bytes[3] & 0x20){
    printf("Operation Mode: VHPF\n");
  }
  if (status.bytes[3] & 0x10){
    printf("Operation Mode: IIR\n");
  }
  if (status.bytes[3] & 0x08){
    printf("Operation Mode: E3MODE1\n");
  }
  if (status.bytes[3] & 0x04){
    printf("Operation Mode: E3MODE0\n");
  }
  if (status.bytes[3] & 0x02){
    printf("Operation Mode: POS\n");
  }
  if (status.bytes[3] & 0x01){
    printf("Operation Mode: AFC\n");
  }
}

int checkDataReady(void) {
  Register status = getRegister(15);
  if (status.bytes[1] & 0x80){
    return 1;
  }
  return 0;
}

int checkConvReady(void) {
  Register status = getRegister(15);
  if (status.bytes[1] & 0x08){
    return 1;
  }
  return 0;
}

void getStatus(void) {
  Register status = getRegister(15);
  if (status.bytes[1] & 0x80){
    printf("Status: DRDY\n");
  }
  if (status.bytes[1] & 0x08){
    printf("Status: CRDY\n");
  }
  if (status.bytes[1] & 0x02){
    printf("Status: IOR\n");
  }
  if (status.bytes[1] & 0x01){
    printf("Status: VOR\n");
  }
  if (status.bytes[2] & 0x40){
    printf("Status: IROR\n");
  }
  if (status.bytes[2] & 0x20){
    printf("Status: VROR\n");
  }
  if (status.bytes[2] & 0x10){
    printf("Status: EOR\n");
  }
  if (status.bytes[2] & 0x08){
    printf("Status: IFAULT\n");
  }
  if (status.bytes[2] & 0x04){
    printf("Status: VSAG\n");
  }
  if (status.bytes[3] & 0x80){
    printf("Status: TUP\n");
  }
  if (status.bytes[3] & 0x40){
    printf("Status: TOD\n");
  }
  if (status.bytes[3] & 0x10){
    printf("Status: VOD\n");
  }
  if (status.bytes[3] & 0x08){
    printf("Status: IOD\n");
  }
  if (status.bytes[3] & 0x04){
    printf("Status: LSD\n");
  }
  if (status.bytes[3] & 0x02){
    printf("Status: FUP\n");
  }
  if (!(status.bytes[3] & 0x01)){
    printf("Status: IC\n");
  }
}

void getMask(void) {
  Register countRegister = getRegister(26);
  unsigned int val = (((unsigned int)countRegister.bytes[1]) * 0x10000)+(((unsigned int)countRegister.bytes[2]) * 0x100)+(((unsigned int)countRegister.bytes[3]));
  printf("Status mask: 0x%06d", val);
}

double range_1_sign(Register * reg){
  int sign = reg->bytes[1] & 0x80;
  reg->bytes[1] = reg->bytes[1] & 0x7F;
  double current = binConvert(reg, 1);
  if (reg->bytes[1] & 0x80){
    current = -current;
  }
  return current;
}

void measureSync(void) {
  performSingleComputation();
  clock_t t = clock();
  while (checkDataReady() == 0) {
    /* printf("Data not ready - DRDY=0\n"); */
    delay(1);
  }
  t = clock() - t;
  double measTime = ((double)t) / CLOCKS_PER_SEC;
  printf("Measurement ready after %f seconds\n", measTime * 10);
}

void performSingleComputation(void) {
  unsigned char buffer[4];
  buffer[0] = 0xE0;
  spiWR(0, buffer, 1);
}

void performContinuousComputation(void) {
  unsigned char buffer[4];
  buffer[0] = 0xE8;
  spiWR(0, buffer, 1);
}

double binConvert(Register * reg, double pow2) {
  unsigned char mask = 0x80;
  double res=0;

  do {
    if ((reg->bytes[1] & mask) != 0){
      res += pow2;
    }
    pow2 = pow2 / 2;
    mask = mask >> 1;
  } while (mask != 0);

  mask = 0x80;
  do {
    if ((reg->bytes[2] & mask) != 0){
      res += pow2;
    }
    pow2 = pow2 / 2;
    mask = mask >> 1;
  } while (mask != 0);
  mask = 0x80;
  do {
    if ((reg->bytes[3] & mask) != 0){
      res += pow2;
    }
    pow2 = pow2 / 2;
    mask = mask >> 1;
  } while (mask != 0);
  return res;
}

void writePage1(int reg, int value){
  unsigned char bufferPage[4] = {0x7E,0x00,0x00,0x1};
  const unsigned char msb = (value & 0xFF0000) >> 16;
  const unsigned char mediumByte = (value & 0xFF00) >> 8;
  const unsigned char lowByte = value & 0xFF;
  unsigned char bufferReg[4] = {0x40 | (reg << 1), msb, mediumByte, lowByte};
  /* printf("%02X %02X %02X %02X\n", bufferReg[0], bufferReg[1], bufferReg[2], bufferReg[3]); */
  spiWR(0, bufferPage, 4);
  spiWR(0, bufferReg, 4);
  bufferPage[3] = 0x0;  // Reset Page pointer.
  spiWR(0, bufferPage, 4);
}

void readPage1(void){
  unsigned char buffer[4] = {0x7E,0x00,0x00,0x1};
  spiWR(0,buffer,4);
  printf("Energy Pulse Output Width: -->");
  readRegister(0);
  printf("No Load Threshold: --> ");
  readRegister(1);
  printf("Temperature sensor gain: --> ");
  readRegister(2);
  printf("Temperature Sensor offset: --> ");
  readRegister(3);
}

void readAllRegister(void) {
  int i;
  unsigned char buffer[4] = {0x7E,0x00,0x00,0x0};
  spiWR(0,buffer,4);

  for (i=0; i <= 31; i++){
    printf("read register %d) --> ", i);
    readRegister(i);
  }
}

unsigned int readRegister(unsigned char reg){
  unsigned char buffer[4] = {reg<<1, 0xFF, 0xFF,0xFF};

  spiWR(0,buffer, 4);
  printf("%02X%02X%02X\n", buffer[1], buffer[2], buffer[3]);
  return (((unsigned int)buffer[1]) << 16) + (((unsigned int)buffer[2]) << 8) + buffer[3];
}

Register getRegister(byte reg) {
  unsigned char buffer[4] = {reg<<1, 0xFF, 0xFF,0xFF};
  Register result;

  spiWR(0, buffer, 4);

  result.bytes[0] = 0;
  result.bytes[1] = buffer[1];
  result.bytes[2] = buffer[2];
  result.bytes[3] = buffer[3];
  return result;
}

void writeRegister(int reg, int value) {
  const unsigned char msb = (value & 0xFF0000) >> 16;
  const unsigned char mediumByte = (value & 0xFF00) >> 8;
  const unsigned char lowByte = value & 0xFF;
  unsigned char bufferReg[4] = {0x40 | (reg << 1), msb, mediumByte, lowByte};
  /* printf("%02X %02X %02X %02X\n", bufferReg[0], bufferReg[1], bufferReg[2], bufferReg[3]); */
  spiWR(0, bufferReg, 4);
}
