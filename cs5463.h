#ifndef GEOMETRY_BASE_H
#define GEOMETRY_BASE_H

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define SPI_CHANNEL 0
#define SPI_SPEED 500000
#define RESET_PIN 3
#define IRQ_PIN 2

typedef unsigned char byte;

union _Register {
  unsigned int pageNumber;
  unsigned int number;
  unsigned int value;
  byte bytes[4];
};

typedef union _Register Register;

enum STATUS_TYPE {
  DREADY = 0x80,
  CRDY = 0x08,
  IOR = 0x02,
  VOR = 0x01,
  IROR = 0x40,
  VROR = 0x20,
  EOR = 0x10,
  IFAULT = 0x08,
  VSAG = 0x04,
  TUP = 0x80,
  TOD = 0x40,
  VOD = 0x10,
  IOD = 0x08,
  LSD = 0x04,
  FUP = 0x02,
  IC = 0x01
} _statusType;

void init(void);
int initSpi(int channel, int speed);
void resetChip(int pin);
int spiWR(int channel, unsigned char *data, int len);
unsigned int readRegister(unsigned char reg);
Register getRegister(byte reg);
void readAllRegister(void);
void readPage1(void);
void writePage1(int reg, int value);
void setPage0(void);
double getIstantaneusCurrent(void);
double getIstantaneusVolt(void);
double getIstantaneusPower(void);
double getRealPower(void);
double getRMSVolt(void);
double getRMSCurrent(void);
double getCurrentOffset(void);
void setCurrentOffset(int offset);
double getVoltageOffset(void);
void setVoltageOffset(int offset);
double getCurrentGain(void);
void setCurrentGain(double gain);
double getVoltageGain(void);
void setVoltageGain(double gain);
double pulseRage(void);
unsigned int getCycleCount(void);
void setCycleCount(unsigned int cycles);
double getEpsilon(void);
double getPowerOffset(void);
void getStatus(void);
void getMask(void);
double getCurrentACOffset(void);
double getVoltageACOffset(void);
void getOperationMode(void);
double getTemperature(void);
void performSingleComputation(void);
void performContinuousComputation(void);
int checkDataReady(void);
void measureSync(void);
void enableHighPassFilter();

double binConvert(Register * reg, double pow2);
double _range_1_sign(Register * reg);

#endif
