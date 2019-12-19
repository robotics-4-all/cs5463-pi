#include "cs5463.h"

int main() {
  init();
  /* performContinuousComputation(); */
  /* delay(1000); */
  while(1) {
    performSingleComputation();
    delay(1200);
    /* waitForInterrupt(IRQ_PIN, -1); */
    getStatus();
    getIstantaneusCurrent();
    getIstantaneusVolt();
    getIstantaneusPower();
    getRMSCurrent();
    getRMSVolt();
    getRealPower();
    getTemperature();
  }
}
