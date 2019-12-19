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
void setPage0(void);
double getIstantaneusCurrent(void);
double getIstantaneusVolt(void);
double getIstantaneusPower(void);
double getRealPower(void);
double getRMSVolt(void);
double getRMSCurrent(void);
double getCurrentOffset(void);
double getVoltOffset(void);
double getCurrentGain(void);
double getVoltGain(void);
double pulseRage(void);
unsigned int getCycleCount(void);
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

double binConvert(Register * reg, double pow2);
double range_1_sign(Register * reg);

#endif
