#include <time.h>
#include "cs5463.h"

int main() {
  init();
  double i, v, p, rmsI, rmsV, preal, temp = 0.0;
  unsigned int cycleCount, mask = 0;
  double offsetI, offsetV, gainI, gainV, offsetIac, offsetVac = 0.0;
  double q, avgQ = 0.0;
  double peakI, peakV, qReact, pf, s = 0.0;

  setCycleCount(4000);
  setCurrentGain(1.0);
  setVoltageGain(1.0);
  enableHighPassFilter();
  setIGain10();

  performContinuousComputation();

  cycleCount = getCycleCount();
  gainI = getCurrentGain();
  gainV = getVoltageGain();
  offsetI = getCurrentOffset();
  offsetV = getVoltageOffset();
  offsetIac = getCurrentACOffset();
  offsetVac = getVoltageACOffset();
  mask = getStatusMask();
  printf("Status mask: 0x%06d\n", mask);
  printf("Cycle Count: %d\n", cycleCount);
  printf("Current Gain: %f\n", gainI);
  printf("Voltage Gain: %f\n", gainV);
  printf("Current Offset: %f\n", offsetI);
  printf("Voltage Offset: %f\n", offsetV);
  printf("Current AC Offset: %f\n", offsetIac);
  printf("Voltage AC Offset: %f\n", offsetVac);
  getOperationMode();

  clock_t t = clock();
  double measTime;

  while(1) {
    waitDataReady();
    t = clock() - t;
    measTime = ((double)t) / CLOCKS_PER_SEC;
    t = clock();
    printf("Measurement ready after %f seconds\n", measTime * 10);
    printf("----------------------------------------------\n");
    /* waitDataReady(); */
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
    q = getInstantaneousReactivePower();
    printf("Instantaneous Reactive Power (Q): %f\n", q);
    avgQ = getAverageReactivePower();
    printf("Average Reactive Power (Qavg): %f\n", avgQ);
    peakI = getPeakCurrent();
    printf("Peak Current (Ipeak): %f\n", peakI);
    peakV = getPeakVoltage();
    printf("Peak Voltage (Vpeak): %f\n", peakV);
    qReact = getReactivePower();
    printf("Reactive Power (Qtrig): %f\n", qReact);
    pf = getPowerFactor();
    printf("Power Factor (PF): %f\n", pf);
    s = getApparentPower();
    printf("Apparent Power (S): %f\n", s);
    /* temp = getTemperature(); */
    /* printf("Temperature : %f\n", temp); */
    printf("\n");
    readAllRegister();
    printf("\n");
  }
}
