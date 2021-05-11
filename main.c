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
  setCurrentOffset(0);//(int)*offsetI);
  setVoltageOffset(0);//(int)*offsetV);
  setCurrentACOffset(0);//(int)*offsetIac);
  setVoltageACOffset(0);//(int)*offsetVac);

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
  printf("Current AC Offset: %d (%f)\n", getCurrentACOffsetInt(),
    getCurrentACOffset() * I_FACTOR_RMS);
  printf("Voltage AC Offset: %d (%f)\n", getVoltageACOffsetInt(),
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

  //
  FILE *fcal;
  float voff, ioff, poff, qoff, vfac, ifac, vraw, iraw  = 0;
  float sumV, sumI = 0;
  int Nm = 0;
  fcal = fopen("calfile.txt", "r");
  printf("File opened!\n");
  fscanf(fcal, "voff=%f\n", &voff);
  fscanf(fcal, "ioff=%f\n", &ioff);
  fscanf(fcal, "poff=%f\n", &poff);
  fscanf(fcal, "qoff=%f\n", &qoff);
  fscanf(fcal, "vfac=%f\n", &vfac);
  fscanf(fcal, "ifac=%f\n", &ifac);
  fclose(fcal);
  printf("File closed!\n");
  printf("%f \n%f \n%f \n%f \n%f \n%f\n", voff, ioff, poff, qoff, vfac, ifac);
  //

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
    i = getInstantaneusCurrent();
    v = getInstantaneusVolt();
    p = getInstantaneusPower();
    rmsI = getRMSCurrent();
    rmsV = getRMSVolt();
    preal = getRealPower();
    q = getInstantaneousReactivePower();
    avgQ = getAverageReactivePower();
    peakI = getPeakCurrent();
    peakV = getPeakVoltage();
    qReact = getReactivePower();
    pf = getPowerFactor();
    s = getApparentPower();
    vraw = rmsV;
    iraw = rmsI;

    //preal = preal * P_REAL_FACTOR_RMS;
    //rmsI = rmsI * I_FACTOR_RMS;
    //rmsV = rmsV * V_FACTOR_RMS ;
    //avgQ = avgQ * P_REAL_FACTOR_RMS;
    //s = s * P_REAL_FACTOR_RMS;

    preal = preal * vfac * ifac - poff;
    avgQ = avgQ * vfac * ifac - qoff;
    s = s * vfac * ifac;
    rmsV = (rmsV - voff) * vfac;
    rmsI = (rmsI - ioff) * ifac;
    Nm = Nm + 1;
    if (Nm > 60){
      Nm = 1;
      sumV = 0;
      sumI = 0;
    }
    sumV = sumV + rmsV;
    sumI = sumI + rmsI;

    printf("V= %f I= %f P= %f Q= %f S= %f ---- raw v= %f i=%f ---- avg V= %f I= %f\n", rmsV, rmsI, preal, avgQ, s, vraw, iraw, sumV / Nm, sumI / Nm);

   string = make_json(preal, avgQ, preal, rmsI, rmsV, 50.0);
   status = socket_send_data(&sock_fd, string);
   if (status == 0) {
     break;
   }
  }
}
