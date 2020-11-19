#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "cs5463.h"
#include "ipc.h"


int main() {
  init();
  double i, v, p, rmsI, rmsV, preal, temp = 0.0;
  unsigned int cycleCount, mask = 0;
  double gainI, gainV = 0.0;
  double q, avgQ = 0.0;
  double peakI, peakV, qReact, pf, s = 0.0;
  unsigned int* offsetI;
  unsigned int* offsetV;
  unsigned int* offsetIac;
  unsigned int* offsetVac;

  offsetI = malloc(sizeof(*offsetI));
  offsetV = malloc(sizeof(*offsetV));
  offsetIac = malloc(sizeof(*offsetIac));
  offsetVac = malloc(sizeof(*offsetVac));

  setCycleCount(4000);
  setCurrentGain(1.0);
  setVoltageGain(1.0);

  readCalibrationParams(
    "/home/pi/.config/cs5463/calibration.txt",
    offsetI, offsetV, offsetIac, offsetVac
    );

  // DC offset is not used when HPF (High Pass Filter) is enabled
  /* setCurrentOffset(offsetI); */
  /* setVoltageOffset(offsetV); */
  setCurrentACOffset((int)*offsetIac);
  setVoltageACOffset((int)*offsetVac);

  enableHighPassFilter();
  setIGain50();

  performContinuousComputation();

  cycleCount = getCycleCount();
  printf("Status mask: 0x%06d\n", getStatusMask());
  printf("Cycle Count: %d\n", cycleCount);
  printf("Current Gain: %f\n", getCurrentGain());
  printf("Voltage Gain: %f\n", getVoltageGain());
  printf("Current Offset: %f\n", getCurrentOffset());
  printf("Voltage Offset: %f\n", getVoltageOffset());
  printf("Current AC Offset: %f (%f)\n", getCurrentACOffset(),
    getCurrentACOffset() * I_FACTOR_RMS);
  printf("Voltage AC Offset: %f (%f)\n", getVoltageACOffset(),
    getVoltageACOffset() * V_FACTOR_RMS);
  getOperationMode();

  clock_t t = clock();
  double measTime;

  int sock_fd;
  int status;
  char buffer[512];
  char *string = NULL;
  connect_socket(&sock_fd);

  while(1) {
    waitDataReady();
    /* t = clock() - t; */
    /* measTime = ((double)t) / CLOCKS_PER_SEC; */
    /* t = clock(); */
    /* printf("----------------------------------------------\n"); */
    /* printf("Measurement ready after %f seconds\n", measTime * 10); */
    /* printf("----------------------------------------------\n"); */
    printf("Measurement ready!");
    i = getIstantaneusCurrent();
    /* printf("Instantaneous Current: %f (%f Ampere)\n", i, i * I_FACTOR); */
    v = getIstantaneusVolt();
    /* printf("Instantaneous Voltage: %f (%f Volt)\n", v, v * V_FACTOR); */
    p = getIstantaneusPower();
    /* printf("Instantaneous Power: %f (%f Watt)\n", p, p * P_REAL_FACTOR); */
    rmsI = getRMSCurrent();
    /* printf("RMS Current: %f (%f Ampere)\n", rmsI, rmsI * I_FACTOR_RMS); */
    rmsV = getRMSVolt();
    /* printf("RMS Voltage: %f (%f Volt)\n", rmsV, rmsV * V_FACTOR_RMS); */
    preal = getRealPower();
    preal = (preal > 0.98 ? 0 : preal);
    /* printf("Active (Real) Power: %f (%f Watt)\n", preal, preal * P_REAL_FACTOR_RMS); */
    q = getInstantaneousReactivePower();
    /* printf("Instantaneous Reactive Power (Q): %f\n", q); */
    avgQ = getAverageReactivePower();
    /* printf("Average Reactive Power (Qavg): %f\n", avgQ); */
    peakI = getPeakCurrent();
    /* printf("Peak Current (Ipeak): %f\n", peakI); */
    peakV = getPeakVoltage();
    /* printf("Peak Voltage (Vpeak): %f\n", peakV); */
    qReact = getReactivePower();
    /* printf("Reactive Power (Qtrig): %f\n", qReact); */
    pf = getPowerFactor();
    /* printf("Power Factor (PF): %f\n", pf); */
    s = getApparentPower();
    /* printf("Apparent Power (S): %f\n", s); */
    /* temp = getTemperature(); */
    /* printf("Temperature : %f\n", temp); */
    /* printf("\n"); */
    /* readAllRegister(); */
    /* printf("\n"); */
    preal = preal * P_REAL_FACTOR_RMS;
    rmsI = rmsI * I_FACTOR_RMS;
    rmsV = rmsV * V_FACTOR_RMS;
    string = make_json(preal, avgQ, preal, rmsI, rmsV, 50.0);
    status = socket_send_data(&sock_fd, string);
    if (status == 0) {
      break;
    }
  }
}
