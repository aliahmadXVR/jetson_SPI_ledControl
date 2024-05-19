# SPI Loop-Back Test with Jetson Nano/Orin Nano using SPIDEV Library (C++ version)

  
  <h2 align="left">SPIDEV Library Setup:</h2>

- Step1: Clone the repo using 
``` 
git clone -b https://github.com/aliahmadXVR/jetson_SPI_ledControl.git"
```

- Step2: sudo pip install spidev
- Step3: sudo modprobe spidev
- Step4: Make sure you dont get any error after step3.
- Step5: cd jetson_SPI_ledControl/SPIDEV_C++
- Step6: ./spi_loop_spidev
- To compile the code after any changes do:
```
g++ -o spi_loop_spidev spi_loop_spidev.cpp
```







