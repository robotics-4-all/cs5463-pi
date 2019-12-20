#include "cs5463.h"

int main() {
  init();
  /* performContinuousComputation(); */
  /* delay(1000); */
  double i, v, p, rmsI, rmsV, preal, temp = 0.0;
  unsigned int cycleCount = 0;
  double offsetI, offsetV, gainI, gainV, offsetIac, offsetVac = 0.0;

  setCycleCount(4000);
  setVoltageGain(1 * 4194304);

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
  printf("Current Offset: %f\n", offsetI);
  printf("Voltage Offset: %f\n", offsetV);
  printf("Current AC Offset: %f\n", offsetIac);
  printf("Voltage AC Offset: %f\n", offsetVac);


  while(1) {
    measureSync();
    /* waitForInterrupt(IRQ_PIN, -1); */
    i = getIstantaneusCurrent();
    printf("Instantaneous Current: %f\n", i);
    v = getIstantaneusVolt();
    printf("Instantaneous Voltage: %f\n", v);
    p = getIstantaneusPower();
    printf("Instantaneous Power: %f\n", p);
    rmsI = getRMSCurrent();
    printf("RMS Current: %f\n", rmsI);
    rmsV = getRMSVolt();
    printf("RMS Voltage: %f\n", rmsV);
    preal = getRealPower();
    printf("Real Power: %f\n", preal);
    printf("\n");
    readAllRegister();
    printf("\n");
  }
}
