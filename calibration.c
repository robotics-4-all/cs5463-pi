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
  printf("\n");
  printf("Cycle Count: %d\n", cycleCount);
  printf("Current Gain: %f\n", gainI);
  printf("Voltage Gain: %f\n", gainV);
  printf("Current AC Offset: %f\n", offsetIac);
  printf("Voltage AC Offset: %f\n", offsetVac);
}


int main(int argc, char *argv[]) {
  unsigned int cycleCount;

  if(argc<2) {
    printf("Missing CycleCount argument!\n");
    exit(1);
  }
  cycleCount = atoi(argv[1]);

  init();

  setCurrentGain(1.0);
  setVoltageGain(1.0);
  setCurrentACOffset(0);
  setVoltageACOffset(0);

  // Max cycle counts to perform calibration.
  printf("[*] - Starting Calibration Process with CycleCount=%d\n", cycleCount);
  printf("[*] - This might take a while, depending of the value of CycleCount...\n");
  setCycleCount(cycleCount);

  printf("[*] - Starting AC Offset Calibration process...\n");
  calibrationOffsetAC();
  printf("[*] - Finish AC Offset Calibration process.\n");

  collect_calibration_params();

  printf("\n");
  printf("Calibration Parameters:\n");
  /* printf("- Current DC Offset: %d\n", getCurrentOffsetInt()); */
  /* printf("- Voltage DC Offset: %d\n", getVoltageOffsetInt()); */
  printf("- Current AC Offset: %d\n", getCurrentACOffsetInt());
  printf("- Voltage AC Offset: %d\n", getVoltageACOffsetInt());
}
