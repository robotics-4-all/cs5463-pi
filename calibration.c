#include <stdlib.h>

#include "cs5463.h"


void collect_calibration_params()
{
  unsigned int cycleCount;
  double offsetI, offsetV, gainI, gainV, offsetIac, offsetVac = 0.0;
  unsigned char bytes[4];

  cycleCount = getCycleCount();
  gainI = getCurrentGain();
  gainV = getVoltageGain();
  offsetI = getCurrentOffset();
  offsetV = getVoltageOffset();
  offsetIac = getCurrentACOffset();
  offsetVac = getVoltageACOffset();
  printf("Cycle Count: %d\n", cycleCount);
  printf("Current Gain: %f\n", gainI);
  printf("Voltage Gain: %f\n", gainV);
  printf("Current DC Offset: %f\n", offsetI * I_FACTOR, getCurrentOffsetInt());
  printf("Voltage DC Offset: %f\n", offsetV * V_FACTOR, getVoltageOffsetInt());
  printf("Current AC Offset: %f\n", offsetIac * I_FACTOR_RMS, getCurrentACOffsetInt());
  printf("Voltage AC Offset: %f\n", offsetVac * V_FACTOR_RMS, getVoltageACOffsetInt());
}


int main() {
  init();
  unsigned int cycleCount, mask = 0;

  setCycleCount(4000);
  setCurrentGain(1.0);
  setVoltageGain(1.0);

  collect_calibration_params();

  // Max cycle counts to perform calibration.
  setCycleCount(32000);

  calibrationOffsetDC();
  calibrationOffsetAC();

  collect_calibration_params();

  printf("\n");
  printf("Set Value for Current DC Offset: %d\n", getCurrentOffsetInt());
  printf("Set Value for Voltage DC Offset: %d\n", getVoltageOffsetInt());
  printf("Set Value for Current AC Offset: %d\n", getCurrentACOffsetInt());
  printf("Set Value for Voltage AC Offset: %d\n", getVoltageACOffsetInt());
}
