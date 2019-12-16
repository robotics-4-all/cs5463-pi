#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define SPI_CHANNEL 0
#define SPI_SPEED 500000
#define RESET_PIN 3
#define IRQ_PIN 2

typedef unsigned char byte;

union _Register {
	unsigned int value;
	byte bytes[4];
};

typedef union _Register Register;

void init(void);
int initSpi(int channel, int speed);
void resetChip(int pin);
int spiWR(int channel, unsigned char *data, int len);
unsigned int readRegister(unsigned char reg);
Register getRegister(byte reg);
void readAllRegister(void);
void readPage1(void);
void writePage1(int reg, int value);
char getcmd(void);
void printHelp(void);
void setPage0(void);
void printIstantaneusCurrent(void);
void printIstantaneusVolt(void);
void printIstantaneusPower(void);
void printRealPower(void);
void printRMSVolt(void);
void printRMSCurrent(void);
void printCurrentOffset(void);
void printVoltOffset(void);
void printCycleCount(void);
void printCurrentGain(void);
void printVoltGain(void);
void pulseRage(void);
void printCycleCount(void);
void printEpsilon(void);
void printPowerOffset(void);
void printStatus(void);
void printMask(void);
void printCurrentACOffset(void);
void printVoltageACOffset(void);
void printOperationMode(void);
void printTemperature(void);
void performSingleComputation(void);
void performContinuousComputation(void);

double binConvert(Register * reg, double pow2);
double range_1_sign(Register * reg);

int main() {
  init();
  /* performContinuousComputation(); */
  /* delay(1000); */
  while(1) {
    performSingleComputation();
    delay(1200);
    /* waitForInterrupt(IRQ_PIN, -1); */
    printStatus();
    printIstantaneusCurrent();
    printIstantaneusVolt();
    printIstantaneusPower();
    printRMSCurrent();
    printRMSVolt();
    printRealPower();
    printTemperature();
  }
}

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
}

void setPage0(){
  unsigned char buffer[4] = {0x7E,0x00,0x00,0x0};
  spiWR(0,buffer,4);
}

void printIstantaneusCurrent(){
  Register reg = getRegister(7);
  printf("Insta-Current: %0.9f\n", range_1_sign(&reg));
}

void printIstantaneusVolt(){
  Register reg = getRegister(8);
  printf("Insta-Volt: %0.9f\n", range_1_sign(&reg));
}

void printIstantaneusPower(){
  Register reg = getRegister(9);
  printf("Insta-Power: %0.9f\n", range_1_sign(&reg));
}

void printRealPower(void) {
  Register reg = getRegister(10);
  printf("Real-Power: %0.9f\n", range_1_sign(&reg));
}

void printRMSVolt(void){
  Register reg = getRegister(11);
  printf("RMS Volt: %0.9f\n", binConvert(&reg, 0.5));
}

void printRMSCurrent(void){
  Register reg = getRegister(12);
  printf("RMS Current: %0.9f\n", binConvert(&reg, 0.5));
}

void printCurrentOffset(void) {
  Register reg = getRegister(1);
  printf("Current offset: %0.9f\n", range_1_sign(&reg));
}

void printVoltOffset(void) {
  Register reg = getRegister(3);
  printf("Voltage offset: %0.9f\n", range_1_sign(&reg));
}

void printCurrentGain(void) {
  Register reg = getRegister(2);
  printf("Current Gain: %0.9f\n", binConvert(&reg, 2));
}

void printVoltGain(void) {
  Register reg = getRegister(4);
  printf("Voltage Gain: %0.9f\n", binConvert(&reg, 2));
}

void printTemperature(void) {
  Register reg = getRegister(19);
  int sign = (reg.bytes[3] & 0x80) ? -1 : 1;
  reg.bytes[3] = reg.bytes[3] & 0x7F;
  double val = binConvert(&reg, 128) * sign;;
  printf("Temperature: %0.9f\n", val);
}

void printCycleCount(void) {
  Register reg = getRegister(5);
  unsigned int val = (((unsigned int)reg.bytes[1]) * 0x10000)+(((unsigned int)reg.bytes[2]) * 0x100)+(((unsigned int)reg.bytes[3]));
  printf("Cycle Count: %d\n", val);
}

void pulseRage(void) {
  Register reg = getRegister(6);
  printf("Pulse Rate-E: %0.9f\n", range_1_sign(&reg));
}

void printEpsilon(void){
  Register reg = getRegister(13);
  printf("Epsion: %0.9f\n", range_1_sign(&reg));
}

void printPowerOffset(void) {
  Register reg = getRegister(14);
  printf("Power Offset: %0.9f\n", range_1_sign(&reg));
}

void printCurrentACOffset(void) {
  Register reg = getRegister(16);
  printf("Current AC Offset: %0.9f\n", range_1_sign(&reg));
}

void printVoltageACOffset(void) {
  Register reg = getRegister(17);
  printf("Voltage AC Offset: %0.9f\n", range_1_sign(&reg));
}

void printOperationMode(void){
  int row=6;
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

void printStatus(void) {
  int row=5;
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

void printMask(void) {
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
  unsigned char bufferReg[4] = {0x40 | (reg << 1),msb, mediumByte, lowByte};
  printf("%02X %02X %02X %02X\n", bufferReg[0], bufferReg[1], bufferReg[2], bufferReg[3]);
  spiWR(0,bufferPage,4);
  spiWR(0,bufferReg,4);
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
