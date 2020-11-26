#ifndef CS5463_H
#define CS5463_H

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define SPI_CHANNEL 0
#define SPI_SPEED 250000
#define RESET_PIN 3
#define IRQ_PIN 2

#define V_FACTOR 293.5
#define I_FACTOR 77.94  // Might need *2
#define V_FACTOR_RMS (V_FACTOR / 0.707)
#define I_FACTOR_RMS (I_FACTOR / 0.707)
#define P_REAL_FACTOR (V_FACTOR * I_FACTOR)
#define P_REAL_FACTOR_RMS (V_FACTOR_RMS * I_FACTOR_RMS)
#define Q_FACTOR P_REAL_FACTOR
#define Q_FACTOR_RMS P_REAL_FACTOR_RMS

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
void writeRegister(int reg, unsigned int value);
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
unsigned int getCurrentOffsetInt(void);
void getCurrentOffsetBytes(unsigned char *bytes);
void setCurrentOffset(unsigned int offset);
double getVoltageOffset(void);
unsigned int getVoltageOffsetInt(void);
void getVoltageOffsetBytes(unsigned char *bytes);
void setVoltageOffset(unsigned int offset);
double getCurrentGain(void);
void setCurrentGain(double gain);
double getVoltageGain(void);
void setVoltageGain(double gain);
double getCurrentACOffset(void);
unsigned int getCurrentACOffsetInt(void);
double getVoltageACOffset(void);
unsigned int getVoltageACOffsetInt(void);
void setCurrentACOffset(unsigned int offset);
void setVoltageACOffset(unsigned int offset);

double pulseRage(void);
unsigned int getCycleCount(void);
void setCycleCount(unsigned int cycles);
double getEpsilon(void);
double getPowerOffset(void);
void getStatus(void);
unsigned int getStatusMask(void);
void getOperationMode(void);
double getTemperature(void);
double getHarmonicActivePower(void);
double getFundamentalActivePower(void);
double getFundamentalReactivePower(void);

/** Sends Single computation cycle StartConversion command.
 */
void performSingleComputation(void);

/** Sends Continuous computation cycle StartConversion command.
 */
void performContinuousComputation(void);

/** Sends SYNC0 command.
 */
void sendSync0(void);

/** Sends SYNC1 command.
 */
void sendSync1(void);

/** Check status of DREADY (Data Ready) bit in status register (15).
 */
int checkDataReady(void);

/** Check status of TUP (Temperature updated) bit in status register (15).
 */
int checkTUP(void);

/** Wait for Data Ready. This is a blocking operation.
 *  Polling status of DRDY bit in status register (15).
 */
void waitDataReady(void);

int waitDataReady2(int timeout_ms);

/** Wait for Conversation  Ready. This is a blocking operation.
 *  Polling status of CRDY bit in status register (15).
 */
void waitConvReady(void);

/** Performs synchronous measurement operation.
 *  Sends PerformSingleCompution command and waits for data to be ready.
 */
void measureSync(void);

/** Enables high-pass-filter or current and voltage channels.
 *  Sets IHPF and VHPF bits in operational mode register (18).
 */
void enableHighPassFilter();

/** Disables high-pass-filter or current and voltage channels.
 *  Sets IHPF and VHPF bits in operational mode register (18).
 */
void disableHighPassFilter();

/** Sets the gain of the current PGA to x10
 */
void setIGain10(void);

/** Sets the gain of the current PGA to x50
 */
void setIGain50(void);

/** Perform DC Offset Calibration (Voltage + Current)
 */
void calibrationOffsetDC(void);

/** Perform DC Gain Calibration (Voltage + Current)
 */
void calibrationGainDC(void);

/** Perform AC Offset Calibration (Voltage + Current)
 */
void calibrationOffsetAC(void);

/** Perform AC Gain Calibration (Voltage + Current)
 */
void calibrationGainAC(void);

long timediff(clock_t t1, clock_t t2);

void clearStatusRegister(void);

double _binConvert(Register * reg, double pow2);
double _range_1_sign(Register * reg);

void readCalibrationParams(
  char *fpath,
  unsigned int *offsetI,
  unsigned int *offsetV,
  unsigned int *offsetIac,
  unsigned int *offsetVac
  );

#endif
