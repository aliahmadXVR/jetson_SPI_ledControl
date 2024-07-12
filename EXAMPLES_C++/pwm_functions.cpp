#include <iostream>
#include <unistd.h>
#include <jetgpio.h>

// Function prototypes
void enablePWM(int pin, int frequency);
void disablePWM(int pin);
void setPWMDutyCycle(int pin, int dutyCycle);

int main(int argc, char *argv[])
{
    int Init;

    Init = gpioInitialise();
    if (Init < 0)
    {
        /* jetgpio initialisation failed */
        std::cout << "Jetgpio initialisation failed. Error code: " << Init << std::endl;
        return Init;
    }
    else
    {
        /* jetgpio initialised okay */
        std::cout << "Jetgpio initialisation OK. Return code: " << Init << std::endl;
    }

    int pin = 18;
    int frequency = 10000;

    // Enable PWM on pin 18 with a frequency of 10kHz
    enablePWM(pin, frequency);

    int x = 0;
    int pwm_value = 255;
    std::cout << "PWM going at pin " << pin << " for 60 seconds" << std::endl;
    while (x < 30)
    {
        sleep(1);
        x++;
        setPWMDutyCycle(pin, pwm_value);
        pwm_value -= 50;
        if (pwm_value <= 0)
        {
            pwm_value = 256;
        }
    }

    // Disable PWM on pin 18
    disablePWM(pin);

    // Terminating library
    gpioTerminate();
    std::cout << "PWM stopped, bye!" << std::endl;
    return 0;
}

void enablePWM(int pin, int frequency)
{
    int PWMstat = gpioSetPWMfrequency(pin, frequency);

    if (PWMstat < 0)
    {
        /* PWM frequency set up failed */
        std::cout << "PWM frequency set up failed. Error code: " << PWMstat << std::endl;
        exit(PWMstat);
    }
    else
    {
        /* PWM frequency set up okay */
        std::cout << "PWM frequency set up okay at pin " << pin << ". Return code: " << PWMstat << std::endl;
    }
}

void disablePWM(int pin)
{
    // Set PWM duty cycle to 0 to stop PWM
    int PWMstat = gpioPWM(pin, 0);

    if (PWMstat < 0)
    {
        /* PWM disable failed */
        std::cout << "PWM disable failed. Error code: " << PWMstat << std::endl;
        exit(PWMstat);
    }
    else
    {
        /* PWM disabled okay */
        std::cout << "PWM disabled at pin " << pin << ". Return code: " << PWMstat << std::endl;
    }
}

void setPWMDutyCycle(int pin, int dutyCycle)
{
    int PWMstat = gpioPWM(pin, dutyCycle);

    if (PWMstat < 0)
    {
        /* PWM duty cycle set up failed */
        std::cout << "PWM duty cycle set up failed. Error code: " << PWMstat << std::endl;
        exit(PWMstat);
    }
    else
    {
        /* PWM duty cycle set up okay */
        std::cout << "PWM duty cycle set up okay at pin " << pin << ". Duty cycle: " << dutyCycle << ". Return code: " << PWMstat << std::endl;
    }
}
