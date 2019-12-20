#include "cs5463.h"

int main() {
  init();
  /* performContinuousComputation(); */
  /* delay(1000); */
  double i, v, p, rmsI, rmsV, preal, temp = 0.0;
  unsigned int cycleCount = 0;

  setCycleCount(1000);

  cycleCount = getCycleCount();
  printf("Cycle Count: %d\n", cycleCount);

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
    temp = getTemperature();
    printf("Temperature: %f\n", temp);
    printf("\n");
  }
}
