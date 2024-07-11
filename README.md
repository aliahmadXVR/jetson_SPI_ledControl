# Running PWM on Jetson Agx Orin / Orin Nano
This code has been forked from ```https://github.com/Rubberazer/JETGPIO```. 

# Installtion for Running PWM 
Follow the below instructions:

Clone the repo using ```git clone -b pwm_test https://github.com/aliahmadXVR/jetson_SPI_ledControl.git```

```console
    cd jetson_SPI_ledControl/
    sudo make
    sudo make install
```

# Usage 
```console
  cd EXAMPLES_C++/
  sudo ./jetgpio_PWM_example
```
The above example will run for 1 minute and will vary the PWM on Jetson Orin Pin 32 from 100% to 0% and vice versa.
