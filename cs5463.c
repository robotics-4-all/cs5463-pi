#include <time.h>
#include "cs5463.h"

int initSpi(int channel, int speed){
  int spi =  wiringPiSPISetup(channel, speed) ;
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

void softReset(void) {
  unsigned char buffer[1];
  buffer[0] = 0x80;
  spiWR(0, buffer, 1);
}

void resetCom(void) {
  sendSync1();
  sendSync1();
  sendSync1();
  sendSync0();
}

void init(void) {
  wiringPiSetup();
  pinMode(IRQ_PIN, INPUT);
  resetChip(RESET_PIN);
  initSpi(SPI_CHANNEL, SPI_SPEED);
  softReset();
  resetCom();
  while (getCycleCount() == 0) {
    delay(100);
  }
  setPage0();
  clearStatusRegister();
}

void enableHighPassFilter() {
  unsigned int regNumber = 18;
  unsigned char regBytes[3];
  Register reg = getRegister(18);

  regBytes[0] = reg.bytes[1];
  regBytes[1] = reg.bytes[2];
  regBytes[2] = reg.bytes[3];

  regBytes[2] = regBytes[2] | 0x60;  // IHPF and VHPF in operational mode reg.
  setRegister(regNumber, regBytes);
}

void disableHighPassFilter() {
  unsigned int regNumber = 18;
  unsigned char regBytes[3];
  Register reg = getRegister(18);

  regBytes[0] = reg.bytes[1];
  regBytes[1] = reg.bytes[2];
  regBytes[2] = reg.bytes[3];

  regBytes[2] = regBytes[2] & 0x9F;  // IHPF and VHPF in operational mode reg.
  setRegister(regNumber, regBytes);
}

void setPage0(){
  unsigned char buffer[4] = {0x7E, 0x00, 0x00, 0x00};
  spiWR(0, buffer, 4);
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
  double val = _range_1_sign(&reg);
  val =  val>= 0.99 ? 0 : val;
  return val;
}

double getIstantaneusVolt(){
  Register reg = getRegister(8);
  double val = _range_1_sign(&reg);
  val =  val>= 0.99 ? 0 : val;
  return val;
}

double getIstantaneusPower(){
  Register reg = getRegister(9);
  double val = _range_1_sign(&reg);
  val =  val>= 0.99 ? 0 : val;
  return val;
}

double getRealPower(void) {
  Register reg = getRegister(10);
  double val = _range_1_sign(&reg);
  val =  val>= 0.99 ? 0 : val;
  return val;
}

double getRMSCurrent(void){
  Register reg = getRegister(11);
  double val = _binConvert(&reg, 0.5);
  val =  val>= 0.99 ? 0 : val;
  return val;
}

double getRMSVolt(void){
  Register reg = getRegister(12);
  double val = _binConvert(&reg, 0.5);
  val = val >= 0.99 ? 0 : val;
  return val;
}

double getCurrentOffset(void) {
  Register reg = getRegister(1);
  return _range_1_sign(&reg);
}

unsigned int getCurrentOffsetInt(void){
  Register reg = getRegister(1);
  unsigned int val = (((unsigned int)reg.bytes[1]) * 0x10000)+(((unsigned int)reg.bytes[2]) * 0x100)+(((unsigned int)reg.bytes[3]));
  return val;
}

void getCurrentOffsetBytes(unsigned char *bytes){
  Register reg = getRegister(1);
  bytes[0] = reg.bytes[1];
  bytes[1] = reg.bytes[2];
  bytes[2] = reg.bytes[3];
  bytes[3] = 0;
}

void setCurrentOffset(unsigned int offset) {
  writeRegister(1, offset);
}

double getVoltageOffset(void) {
  Register reg = getRegister(3);
  return _range_1_sign(&reg);
}

unsigned int getVoltageOffsetInt(void) {
  Register reg = getRegister(3);
  unsigned int val = (((unsigned int)reg.bytes[1]) * 0x10000)+(((unsigned int)reg.bytes[2]) * 0x100)+(((unsigned int)reg.bytes[3]));
  return val;
}

void getVoltageOffsetBytes(unsigned char *bytes) {
  Register reg = getRegister(3);
  bytes[0] = reg.bytes[1];
  bytes[1] = reg.bytes[2];
  bytes[2] = reg.bytes[3];
  bytes[3] = 0;
}

void setVoltageOffset(unsigned int offset) {
  writeRegister(3, offset);
}

double getCurrentGain(void) {
  Register reg = getRegister(2);
  return _binConvert(&reg, 2);
}

void setCurrentGain(double gain) {
  // 0.0<=Gain<=3.999
  writeRegister(2, (unsigned int)(gain * 4194304));
}

double getVoltageGain(void) {
  Register reg = getRegister(4);
  return _binConvert(&reg, 2);
}

void setVoltageGain(double gain) {
  // 0.0<=Gain<=3.999
  writeRegister(4, (unsigned int)(gain * 4194304));
}

double getTemperature(void) {
  Register reg = getRegister(19);
  int sign = (reg.bytes[3] & 0x80) ? -1 : 1;
  reg.bytes[3] = reg.bytes[3] & 0x7F;
  double val = _binConvert(&reg, 128) * sign;;
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
  writeRegister(5, cycles);
}

double pulseRage(void) {
  Register reg = getRegister(6);
  /* printf("Pulse Rate-E: %0.9f\n", _range_1_sign(&reg)); */
  return _range_1_sign(&reg);
}

double getEpsilon(void){
  Register reg = getRegister(13);
  /* printf("Epsion: %0.9f\n", _range_1_sign(&reg)); */
  return _range_1_sign(&reg);
}

double getPowerOffset(void) {
  Register reg = getRegister(14);
  /* printf("Power Offset: %0.9f\n", _range_1_sign(&reg)); */
  return _range_1_sign(&reg);
}

void setPowerOffset(unsigned int offset) {
  writeRegister(14, offset);
}

double getCurrentACOffset(void) {
  Register reg = getRegister(16);
  return _range_1_sign(&reg);
}

unsigned int getCurrentACOffsetInt(void) {
  Register reg = getRegister(16);
  unsigned int val = (((unsigned int)reg.bytes[1]) * 0x10000)+(((unsigned int)reg.bytes[2]) * 0x100)+(((unsigned int)reg.bytes[3]));
  return val;
}

void setCurrentACOffset(unsigned int offset) {
  writeRegister(16, offset);
}

double getVoltageACOffset(void) {
  Register reg = getRegister(17);
  return _range_1_sign(&reg);
}

unsigned int getVoltageACOffsetInt(void) {
  Register reg = getRegister(17);
  unsigned int val = (((unsigned int)reg.bytes[1]) * 0x10000)+(((unsigned int)reg.bytes[2]) * 0x100)+(((unsigned int)reg.bytes[3]));
  return val;
}

void setVoltageACOffset(unsigned int offset) {
  writeRegister(17, offset);
}

double getAverageReactivePower(void) {
  // Range -1<=Qavg<=1
  Register reg = getRegister(20);
  return _range_1_sign(&reg);
}

double getInstantaneousReactivePower(void) {
  // Range -1<=Q<=1
  Register reg = getRegister(21);
  return _range_1_sign(&reg);
}

double getPeakCurrent(void) {
  // Range -1<=Ipeak<=1
  Register reg = getRegister(22);
  return _range_1_sign(&reg);
}

double getPeakVoltage(void) {
  // Range -1<=Vpeak<=1
  Register reg = getRegister(23);
  return _range_1_sign(&reg);
}

double getReactivePower(void) {
  // Range 0<=Qtrig<=1
  Register reg = getRegister(24);
  return _binConvert(&reg, 2);
}

double getPowerFactor(void) {
  // Range -1<=PF<=1
  Register reg = getRegister(25);
  return _range_1_sign(&reg);
}

double getApparentPower(void) {
  // Range 0<=S<=1
  Register reg = getRegister(27);
  return _binConvert(&reg, 2);
}

double getHarmonicActivePower(void) {
  // Range -1<=S<=1
  Register reg = getRegister(29);
  return _range_1_sign(&reg);
}

double getFundamentalActivePower(void) {
  // Range -1<=S<=1
  Register reg = getRegister(30);
  return _range_1_sign(&reg);
}

double getFundamentalReactivePower(void) {
  // Range -1<=S<=1
  Register reg = getRegister(31);
  return _range_1_sign(&reg);
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

int checkTUP(void) {
  Register status = getRegister(15);
  if (status.bytes[3] & 0x80){
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

unsigned int getStatusMask(void) {
  Register countRegister = getRegister(26);
  unsigned int val = (((unsigned int)countRegister.bytes[1]) * 0x10000)+(((unsigned int)countRegister.bytes[2]) * 0x100)+(((unsigned int)countRegister.bytes[3]));
  return val;
}

void clearStatusRegister(void) {
  unsigned int regNumber = 15;
  unsigned char regBytes[3] = {0xFF, 0xFF, 0xFF};
  setRegister(regNumber, regBytes);
}

void waitDataReady(void) {
  while (checkDataReady() == 0) {
    delay(1);
  }
  unsigned int regNumber = 15;
  unsigned char regBytes[3];
  Register reg = getRegister(regNumber);

  regBytes[0] = reg.bytes[1];
  regBytes[1] = reg.bytes[2];
  regBytes[2] = reg.bytes[3];
  // Clear DRDY bit in Status Register
  regBytes[0] = SET_BIT(regBytes[0], 7);  // DRDY
  setRegister(regNumber, regBytes);
}

int waitDataReady2(int timeout_ms) {
  clock_t begin = clock();
  double elapsed = 0.0;
  while (checkDataReady() == 0) {
    delay(1);
    elapsed = (double)(clock() - begin) * 1000;
    if (elapsed > timeout_ms) {
      return - 1;
    }
  }
  unsigned int regNumber = 15;
  unsigned char regBytes[3];
  Register reg = getRegister(regNumber);

  regBytes[0] = reg.bytes[1];
  regBytes[1] = reg.bytes[2];
  regBytes[2] = reg.bytes[3];
  // Clear DRDY bit in Status Register
  regBytes[0] = SET_BIT(regBytes[0], 7);  // DRDY
  setRegister(regNumber, regBytes);
  return 1;
}

void waitConvReady(void) {
  while (checkConvReady() == 0) {
    delay(1);
  }
}

void measureSync(void) {
  performSingleComputation();
  waitDataReady();
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

void sendSync0(void) {
  unsigned char buffer[4];
  buffer[0] = 0xFE;
  spiWR(0, buffer, 1);
}

void sendSync1(void) {
  unsigned char buffer[4];
  buffer[0] = 0xFF;
  spiWR(0, buffer, 1);
}

void setIGain50(void) {
  unsigned int regNumber = 0;
  unsigned char regBytes[3];
  Register reg = getRegister(regNumber);

  regBytes[0] = reg.bytes[1];
  regBytes[1] = reg.bytes[2];
  regBytes[2] = reg.bytes[3];

  regBytes[0] = SET_BIT(regBytes[0], 0);  // Igain x10 in configuration register
  setRegister(regNumber, regBytes);
}

void setIGain10(void) {
  unsigned int regNumber = 0;
  unsigned char regBytes[3];
  Register reg = getRegister(regNumber);

  regBytes[0] = reg.bytes[1];
  regBytes[1] = reg.bytes[2];
  regBytes[2] = reg.bytes[3];

  regBytes[0] = CLEAR_BIT(regBytes[0], 0);  // Igain x10 in configuration register
  setRegister(regNumber, regBytes);
}

void calibrationOffsetDC(void) {
  // |1|1|0|CAL4|CAL3|CAL2|CAL1|CAL0|
  // Current and Voltage DC Offset: CAL[4:0] = 11001 = 0x19
  unsigned char cal = 0x19;
  unsigned char buffer[4] = {0xC0 | cal, 0xFF, 0xFF,0xFF};
  spiWR(0, buffer, 1);
  // Synchronous. Wait for DRDY bit to be set in Status Register.
  // Alternatively, set the DRDY bit in Mask Register to output to INT pin.
  waitDataReady();
}

void calibrationGainDC(void) {
  // |1|1|0|CAL4|CAL3|CAL2|CAL1|CAL0|
  // Current and Voltage DC GAIN: CAL[4:0] = 11010 = 0x1A
  unsigned char cal = 0x1A;
  unsigned char buffer[4] = {0xC0 | cal, 0xFF, 0xFF,0xFF};
  spiWR(0, buffer, 1);
  // Synchronous. Wait for DRDY bit to be set in Status Register.
  // Alternatively, set the DRDY bit in Mask Register to output to INT pin.
  waitDataReady();
}

void calibrationOffsetAC(void) {
  // |1|1|0|CAL4|CAL3|CAL2|CAL1|CAL0|
  // Current and Voltage AC Offset: CAL[4:0] = 11101 = 0x1D
  unsigned char cal = 0x1D;
  unsigned char buffer[4] = {0xC0 | cal, 0xFF, 0xFF,0xFF};
  spiWR(0, buffer, 1);
  // Synchronous. Wait for DRDY bit to be set in Status Register.
  // Alternatively, set the DRDY bit in Mask Register to output to INT pin.
  waitDataReady();
}

void calibrationGainAC(void) {
  // |1|1|0|CAL4|CAL3|CAL2|CAL1|CAL0|
  // Current and Voltage AC Offset: CAL[4:0] = 11110 = 0x1E
  unsigned char cal = 0x1E;
  unsigned char buffer[4] = {0xC0 | cal, 0xFF, 0xFF,0xFF};
  spiWR(0, buffer, 1);
  // Synchronous. Wait for DRDY bit to be set in Status Register.
  // Alternatively, set the DRDY bit in Mask Register to output to INT pin.
  waitDataReady();
}

double _binConvert(Register * reg, double pow2) {
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

double _range_1_sign(Register * reg){
  int sign = reg->bytes[1] & 0x80;
  if (sign){
    reg->bytes[1] = reg->bytes[1] ^ 0xFF;
    reg->bytes[2] = reg->bytes[2] ^ 0xFF;
    reg->bytes[3] = reg->bytes[3] ^ 0xFF;
  }
  double current = _binConvert(reg, 1);
  if (sign){
    current = -current;
  }
  return current;
}

void writePage1(int reg, int value){
  unsigned char bufferPage[4] = {0x7E,0x00,0x00,0x1};
  const unsigned char msb = (value & 0xFF0000) >> 16;
  const unsigned char mediumByte = (value & 0xFF00) >> 8;
  const unsigned char lowByte = value & 0xFF;
  unsigned char bufferReg[4] = {0x40 | (reg << 1), msb, mediumByte, lowByte};
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
  unsigned int regVal = 0;
  spiWR(0, buffer, 4);

  for (i=0; i <= 31; i++){
    regVal = readRegister(i);
    printf("[Register %d --> %06X]\n", i, regVal);
  }
}

unsigned int readRegister(unsigned char reg){
  unsigned char buffer[4] = {reg<<1, 0xFF, 0xFF,0xFF};

  spiWR(0, buffer, 4);
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

void setRegister(unsigned char reg, unsigned char* value) {
  // 0x40 -> write 1 to B6 for write control
  unsigned char bufferReg[4] = {0x40 | (reg << 1), value[0], value[1], value[2]};
  spiWR(0, bufferReg, 4);
}

void writeRegister(int reg, unsigned int value) {
  const unsigned char msb = (value & 0xFF0000) >> 16;
  const unsigned char mediumByte = (value & 0xFF00) >> 8;
  const unsigned char lowByte = value & 0xFF;
  unsigned char bufferReg[4] = {0x40 | (reg << 1), msb, mediumByte, lowByte};
  /* printf("%02X %02X %02X %02X\n", bufferReg[0], bufferReg[1], bufferReg[2], bufferReg[3]); */
  spiWR(0, bufferReg, 4);
}

void readCalibrationParams(
  char *fpath,
  unsigned int *offsetI,
  unsigned int *offsetV,
  unsigned int *offsetIac,
  unsigned int *offsetVac
  )
{
  FILE *fp;

  fp = fopen(fpath, "r");
  printf("[*] - Reading Calibration Params from file: %s\n", fpath);
  fscanf(fp, "CurrentOffsetDC=%u\n", offsetI);
  fscanf(fp, "VoltageOffsetDC=%u\n", offsetV);
  fscanf(fp, "CurrentOffsetAC=%u\n", offsetIac);
  fscanf(fp, "VoltageOffsetAC=%u\n", offsetVac);
}

void readConfigParams(
  char *fpath,
  unsigned int *vFactor,
  unsigned int *iFactor,
  unsigned int *iGain
  )
{
  FILE *fp;

  fp = fopen(fpath, "r");
  printf("[*] - Reading Calibration Params from file: %s\n", fpath);
  fscanf(fp, "VFactor=%u\n", vFactor);
  fscanf(fp, "IFactor=%u\n", iFactor);
  fscanf(fp, "IGain=%u\n", iGain);
}
