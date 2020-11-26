#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "cs5463.h"
#include "ipc.h"


void initialize_cs5463(){
  unsigned int cycleCount = 4000;
  init();
  unsigned int* offsetI;
  unsigned int* offsetV;
  unsigned int* offsetIac;
  unsigned int* offsetVac;

  offsetI = malloc(sizeof(*offsetI));
  offsetV = malloc(sizeof(*offsetV));
  offsetIac = malloc(sizeof(*offsetIac));
  offsetVac = malloc(sizeof(*offsetVac));

  setCycleCount(cycleCount);
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
}


int main() {
  double i, v, p, rmsI, rmsV, preal, temp = 0.0;
  double gainI, gainV = 0.0;
  double q, avgQ = 0.0;
  double peakI, peakV, qReact, pf, s = 0.0;

  initialize_cs5463();

  double measTime;

  int sock_fd;
  int status;
  char buffer[512];
  char *string = NULL;
  connect_socket(&sock_fd);

  while(1) {
    status = waitDataReady2(30000);
    if (status < 0) {
      printf("Measurement timed out after 30 seconds!\n");
      initialize_cs5463();
      continue;
    }
    /* t = clock() - t; */
    /* measTime = ((double)t) / CLOCKS_PER_SEC; */
    /* t = clock(); */
    /* printf("----------------------------------------------\n"); */
    /* printf("Measurement ready after %f seconds\n", measTime * 10); */
    /* printf("----------------------------------------------\n"); */
    printf("Measurement ready!\n");
    i = getIstantaneusCurrent();
    v = getIstantaneusVolt();
    p = getIstantaneusPower();
    rmsI = getRMSCurrent();
    rmsV = getRMSVolt();
    preal = getRealPower();
    preal = (preal > 0.98 ? 0 : preal);
    q = getInstantaneousReactivePower();
    avgQ = getAverageReactivePower();
    peakI = getPeakCurrent();
    peakV = getPeakVoltage();
    qReact = getReactivePower();
    pf = getPowerFactor();
    s = getApparentPower();

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
