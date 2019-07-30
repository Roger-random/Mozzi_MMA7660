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

This repository shows how to avoid this error and communicate with a MMA7660 without using the Wire library.

For more details see [here](https://newscrewdriver.com/2019/07/23/aborted-attempt-at-arduino-accelerometer-mozzi-mma7660/).
