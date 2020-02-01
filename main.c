#include <time.h>
#include <stdlib.h>
#include <cjson/cJSON.h>

#include "cs5463.h"
#include "ipc.h"



char *make_json(
  float active,
  float reactive,
  float powerl1,
  float current,
  float voltage,
  float phaseanglecurrentvoltagel1
  )
{
  char *string = NULL;
  cJSON *root  = cJSON_CreateObject();

  cJSON_AddItemToObject(
    root,
    "active",
    cJSON_CreateNumber(active)
    );
  cJSON_AddItemToObject(
    root,
    "reactive",
    cJSON_CreateNumber(active)
    );
  cJSON_AddItemToObject(
    root,
    "powerl1",
    cJSON_CreateNumber(powerl1)
    );
  cJSON_AddItemToObject(
    root,
    "current",
    cJSON_CreateNumber(current)
    );
  cJSON_AddItemToObject(
    root,
    "voltage",
    cJSON_CreateNumber(voltage)
    );
  cJSON_AddItemToObject(
    root,
    "phaseanglecurrentvoltagel1",
    cJSON_CreateNumber(phaseanglecurrentvoltagel1)
    );

  string = cJSON_Print(root);
  if (string == NULL) {
    fprintf(stderr, "Failed to print monitor.\n");
  }
   /* free all objects under root and root itself */
   cJSON_Delete(root);
   return string;
}


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

  setCurrentOffset(16643505);
  setVoltageOffset(491583);
  setCurrentACOffset(16776330);
  setVoltageACOffset(0);
  /* enableHighPassFilter(); */
  setIGain50();

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

  int sock_fd;
  char buffer[512];
  char *string = NULL;
  connect_socket(&sock_fd);

  strcpy (buffer, "Hello");


  while(1) {
    waitDataReady();
    t = clock() - t;
    measTime = ((double)t) / CLOCKS_PER_SEC;
    t = clock();
    printf("Measurement ready after %f seconds\n", measTime * 10);
    printf("----------------------------------------------\n");
    i = getIstantaneusCurrent();
    printf("Instantaneous Current: %f (%f Ampere)\n", i, i * I_FACTOR);
    v = getIstantaneusVolt();
    printf("Instantaneous Voltage: %f (%f Volt)\n", v, v * V_FACTOR);
    p = getIstantaneusPower();
    printf("Instantaneous Power: %f (%f )\n", p, p * P_FACTOR);
    rmsI = getRMSCurrent();
    printf("RMS Current: %f - %f Ampere\n", rmsI, rmsI * I_FACTOR_RMS);
    rmsV = getRMSVolt();
    printf("RMS Voltage: %f - %f Volt\n", rmsV, rmsV * V_FACTOR_RMS);
    preal = getRealPower();
    preal = (preal > 0.98 ? 0 : preal);
    printf("Active (Real) Power: %f - %f Watt\n", preal, preal * V_FACTOR_RMS * I_FACTOR_RMS);
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
    /* readAllRegister(); */
    printf("\n");
    string = make_json(preal, qReact, preal, rmsI, rmsV, 0.0);
    printf("%s\n", string);
    socket_send_data(&sock_fd, string);
  }
}
