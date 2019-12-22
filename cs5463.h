#ifndef CS5463_H
#define CS5464_H

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define SPI_CHANNEL 0
#define SPI_SPEED 500000
#define RESET_PIN 3
#define IRQ_PIN 2

#define SET_BIT(value, pos) (value |= (1U<< pos))
#define CLEAR_BIT(value, pos) (value &= (~(1U<< pos)))

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

/** Initialize device and SPI communication.
 */
void init(void);
/** Initialize SPI communication.
 */
int initSpi(int channel, int speed);

/** Reset cs5463 chip via RESET pin.
 */
void resetChip(int pin);

/** Reset cs5463 chip via 0x80 command.
 */
void softReset(void);

/** Reset cs5463 chip via SYNC1x3 + SYNC0 commands.
 */
void resetCom(void);

/** SPI Write-Read operation.
 */
int spiWR(int channel, unsigned char *data, int len);
Register getRegister(byte reg);
void setRegister(unsigned char reg, unsigned char* value);
unsigned int readRegister(unsigned char reg);
void readAllRegister(void);
void writeRegister(int reg, int value);
void readPage1(void);
void writePage1(int reg, int value);

/** Set Page register to point to Page0.
 */
void setPage0(void);

/** Set Page register to point to Page1.
 */
void setPage1(void);

/** Set Page register to point to Page2.
 */
void setPage2(void);

/** Set Page register to point to Page3.
 */
void setPage3(void);
double getIstantaneusCurrent(void);
double getIstantaneusVolt(void);
double getIstantaneusPower(void);
double getRealPower(void);
double getInstantaneousReactivePower(void);
double getAverageReactivePower(void);
double getPeakCurrent(void);
double getPeakVoltage(void);
double getReactivePower(void);
double getPowerFactor(void);
double getApparentPower(void);
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
void sendSync0(void);
void sendSync1(void);
int checkDataReady(void);
int checkTUP(void);
void measureSync(void);
void enableHighPassFilter();
void setIGain10(void);
void setIGain50(void);

double binConvert(Register * reg, double pow2);
double _range_1_sign(Register * reg);

#endif
