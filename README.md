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

A sample `calibration.txt` file would look like below:

```
CurrentOffsetDC=16643505
VoltageOffsetDC=491583
CurrentOffsetAC=16776330
VoltageOffsetAC=0
```

## Define VFactor and IFactor Values

In order to measure proper Voltage and Current values in SI (Volt/Ampere) the `VFactor` and `IFactor` constants have to be defined. These constants are unique per device and depend on the hardware setup (R_shunt, R1_divider, R2_divider, etc).
In order to properly transform obtained measurements to SI, the values should be placed in the configuration file located at `~/.config/cs5463/config.txt`

A sample `config.txt` file would look like below:

```
VFactor=292
IFactor=77.94
```

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
