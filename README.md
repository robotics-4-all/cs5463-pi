# cs5463-pi
C Library for [CS5463 PowerMonitor IC](https://www.versasense.com/files/datasheets/CS5463_F3.pdf) to interface with Raspberry PI boards. Tested with Pi Zero, Pi Zero W, Pi 3 Model b+

## Perform Calibration

First build the calibration program.

```bash
make calibration
```

Run calibration with given number of CycleCounts. The calibration
algorithms are depended on the value of CycleCounts. As the number
increases, the accuracy of calibration results will increase.

For example, the below example will perform calibration with CycleCounts=32000

```bash
./calibration 32000
```

The output should look something like this:

```bash
[*] - Starting Calibration Process with CycleCount=512000
[*] - This might take a while, depending of the value of CycleCount...
[*] - Starting DC Offset Calibration process...
[*] - Finish DC Offset Calibration process.
[*] - Starting AC Offset Calibration process...
[*] - Finish AC Offset Calibration process.

Cycle Count: 512000
Current Gain: 1.000000
Voltage Gain: 1.000000
Current DC Offset: 0.984063
Voltage DC Offset: 17.413815
Current AC Offset: 110.240279
Voltage AC Offset: 0.000000

Calibration Parameters:
- Current DC Offset: 16643528
- Voltage DC Offset: 491844
- Current AC Offset: 16776330
- Voltage AC Offset: 0
```

Take the values of Calibration Parameters and place them in the calibration
cfg file at `~/.config/cs5463/calibration.txt`.

## State

### Implemented Features

#### Measurements

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

#### Calibration (Offset & Gain)
- getCurrentOffset
- getCurrentOffsetInt
- setCurrentOffset
- getVoltageOffset
- getVoltageOffsetInt
- setVoltageOffset
- getCurrentACOffset
- getCurrentACOffsetInt
- setCurrentACOffset
- getVoltageACOffset
- getVoltageACOffsetInt
- setVoltageACOffset
- getCurrentGain
- setCurrentGain
- getVoltageGain
- setVoltageGain
- getPowerOffset
- readCalibrationParams
- calibrationOffsetDC
- calibrationOffsetAC

#### Functional
- getCycleCount
- setCycleCount
- getEpsilon
- getStatus
- getStatusMask
- getOperationMode
- getTemperature
- performSingleComputation
- performContinuousComputation
- setIGain50
- setIGain10
- enableHighPassFilter
- clearStatusRegister
