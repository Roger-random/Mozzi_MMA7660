# Mozzi + MMA7660 Without Arduino Wire Library

Mozzi I2C example code converted to use MMA7660 accelerometer instead of ADXL345

* Link to [Mozzi library](https://github.com/sensorium/Mozzi)
* Link to [Original Mozzi I2C example with ADXL345](https://github.com/sensorium/Mozzi/tree/master/examples/11.Communication/TwoWire_Read_ADXL345).
* Link to [MMA7660 breakout board](http://wiki.seeedstudio.com/Grove-3-Axis_Digital_Accelerometer-1.5g/)
* Link to [MMA7660 Arduino example using Wire library](https://github.com/Seeed-Studio/Accelerometer_MMA7660)

But Wire library is incompatible with Mozzi. Attempt to use both in the same Arduino sketch would result in the following error

```
libraries/Wire/utility/twi.c.o (symbol from plugin): In function `twi_init':
(.text+0x0): multiple definition of `__vector_24'
libraries/Mozzi/twi_nonblock.cpp.o (symbol from plugin):(.text+0x0): first defined here
collect2: error: ld returned 1 exit status
exit status 1
```

This repository shows how to communicate with a MMA7660 using Mozzi's own I2C communication library instead of Arduino's Wire library.
All the changes to switch from ADXL345 to MMA7660 has been
[squashed into a single commit](https://github.com/Roger-random/Mozzi_MMA7660/commit/67f0d38209dffe1e35d3374c59134d74bae5151b)
for easier reference.

For more details see the writeups [Part 1](https://newscrewdriver.com/2019/07/23/aborted-attempt-at-arduino-accelerometer-mozzi-mma7660/)
and [Part 2](https://newscrewdriver.com/2019/07/30/second-attempt-success-mozzi-mma7660/).

## More

* [__Mozzi_MPU6050__](https://github.com/algomusic/Mozzi_MPU6050):
@algomusic adapted this example to use MPU6050 accelerometer. Heard through
[Mozzi-users Google Group](https://groups.google.com/g/mozzi-users/c/vnPNIInrD88/m/fvc1XDk1CwAJ)
