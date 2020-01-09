# cs5463-pi
C Library for [CS5463 PowerMonitor IC](https://www.versasense.com/files/datasheets/CS5463_F3.pdf) to interface with Raspberry PI boards. Tested with Pi Zero, Pi Zero W, Pi 3 Model b+


## State

### Implemented Features

- getIstantaneusCurrent
- getIstantaneusVolt
- getIstantaneusPower
- getRealPower
- getInstantaneousReactivePower
- getAverageReactivePower
- getPeakCurrent
- getPeakVoltage
- getReactivePower
- getPowerFactor
- getApparentPower
- getRMSVolt
- getRMSCurrent
- getCurrentOffset
- setCurrentOffset
- getVoltageOffset
- setVoltageOffset
- getCurrentGain
- setCurrentGain
- getVoltageGain
- setVoltageGain
- getCycleCount
- setCycleCount
- getEpsilon
- getPowerOffset
- getStatus
- getStatusMask
- getCurrentACOffset
- getVoltageACOffset
- getOperationMode
- getTemperature
- performSingleComputation
- performContinuousComputation
- setIGain50
- setIGain10
- enableHighPassFilter
- clearStatusRegister


### Not implemented Features

- calibration
- setCalibrationParams
