#include <iostream>
#include <unistd.h>
#include <jetgpio.h>
#include <algorithm>
#include <bitset>
#include <iomanip>
#include <bitset>
#include <cstring>


class LEDController {
private:
    const int NUM_OF_LEDS;
    const int ONE_LED_DATA;
    const int BYTE_FOR_ONE_LED;
    const char ON;
    const char OFF;
    int SPI_init;

public:
    LEDController(int numOfLEDs, int oneLedData, int byteForOneLed, char onValue, char offValue) 
        : NUM_OF_LEDS(numOfLEDs), ONE_LED_DATA(oneLedData), BYTE_FOR_ONE_LED(byteForOneLed), ON(onValue), OFF(offValue), SPI_init(-1) {}

    bool initialize() {
        int Init = gpioInitialise();
        if (Init < 0) {
            std::cerr << "JETGPIO initialization failed. Error code: " << Init << std::endl;
            return false;
        } else {
            std::cout << "JETGPIO initialization OK. Return code: " << Init << std::endl;
        }

        SPI_init = spiOpen(0, 2500000, 0, 0, 8, 1, 1);
        if (SPI_init < 0) {
            std::cerr << "SPI port opening failed. Error code: " << SPI_init << std::endl;
            gpioTerminate();
            return false;
        } else {
            std::cout << "SPI port opened OK. Return code: " << SPI_init << std::endl;
        }

        return true;
    }

    void performSPITransfer(LEDController& ledController, char* tx, char* rx, int dataSize) {
      int SPI_stat = spiXfer(ledController.getSpiInit(), tx, rx, dataSize);
      if (SPI_stat >= 0) {
         std::cout << "Transfered data" << std::endl;
      } else {
         std::cerr << "SPI transfer failed" << std::endl;
      }
   }

    void close_spi_port() {
        if (SPI_init >= 0) {
            spiClose(SPI_init);
        }
        gpioTerminate();
    }

    void turnOffLeds(char *data, int size) {
        std::fill(data, data + size, OFF);
        std::cout << "Data OFF: ";
        for (int i = 0; i < size; ++i) {
            std::cout << static_cast<int>(data[i]) << " ";
        }
        std::cout << std::endl;
    }

    void turnOnLed(char *data, int size, int LED_number) {
        if (LED_number >= 0 && LED_number < NUM_OF_LEDS) {
            int start_index = LED_number * ONE_LED_DATA;
            std::fill(data + start_index, data + start_index + ONE_LED_DATA, ON);
            std::cout << "Data ON: ";
            for (int i = 0; i < size; ++i) {
                std::cout << static_cast<int>(data[i]) << " ";
            }
            std::cout << std::endl;
        } else {
            std::cout << "Wrong LED Number Specified. Must be between 0 & " << (NUM_OF_LEDS - 1) << std::endl;
        }
    }


    void turnLEDcolor(char *data, int size, const std::bitset<72>& binaryNumber) {
    if (size < 9) {
        std::cerr << "Data size should be at least 9 bytes for a 72-bit binary number." << std::endl;
        return;
    }
    
    // Convert binary number to char array
    for (int i = 0; i < 72; ++i) {
        data[i / 8] |= (binaryNumber.test(i) ? 1 : 0) << (7 - (i % 8)); // Set the appropriate bit in the char array
    }
    
    std::cout << "Data with LED color: ";
    for (int i = 0; i < size; ++i) {
        std::cout << static_cast<int>(data[i]) << " ";
    }
    std::cout << std::endl;
    }

   std::bitset<8> convertToBinary(int decimalValue) {
      if (decimalValue < 0 || decimalValue > 255) {
         std::cerr << "Decimal value must be between 0 and 255" << std::endl;
         return std::bitset<8>(0); // Return all zeros in case of invalid input
      }
      return std::bitset<8>(decimalValue);
   }

   std::bitset<24> convertToBinary(int decimalValue1, int decimalValue2, int decimalValue3) {
    if (decimalValue1 < 0 || decimalValue1 > 255 || decimalValue2 < 0 || decimalValue2 > 255 || decimalValue3 < 0 || decimalValue3 > 255) {
        std::cerr << "Decimal values must be between 0 and 255" << std::endl;
        return std::bitset<24>(0); // Return all zeros in case of invalid input
    }

    std::bitset<8> binaryValue1(decimalValue1);
    std::bitset<8> binaryValue2(decimalValue2);
    std::bitset<8> binaryValue3(decimalValue3);

    std::bitset<24> result;

    // Concatenate the three 8-bit binary numbers to form a single 24-bit binary number
    for (int i = 0; i < 8; ++i) {
        result[i] = binaryValue1[i];
        result[i + 8] = binaryValue2[i];
        result[i + 16] = binaryValue3[i];
    }

    return result;
    }

    int getSpiInit() const {
        return SPI_init;
    }

    void test_led(char *data, int size, std::bitset<72> binaryData) {
    // Convert binary data to char array
    std::memcpy(data, binaryData.to_string().c_str(), size);
    
    std::cout << "Data OFF: ";
    for (int i = 0; i < size; ++i) {
        std::cout << static_cast<int>(data[i]) << " ";
    }
    std::cout << std::endl;
}


    private:
    bool isValidDecimalValue(int decimalValue) const {
        return (decimalValue >= 0 && decimalValue <= 255);
    }
};

int main() {
    const int NUM_OF_LEDS = 12;  // For this particular project
    const int ONE_LED_DATA = 24;
    const int BYTE_FOR_ONE_LED = ONE_LED_DATA * 3;
    const char ON = 0x06;
    const char OFF = 0x04;

    LEDController ledController(NUM_OF_LEDS, ONE_LED_DATA, BYTE_FOR_ONE_LED, ON, OFF);

    if (!ledController.initialize()) {
        return 1;
    }

 
   char* tx = new char[NUM_OF_LEDS * ONE_LED_DATA];
    char rx[BYTE_FOR_ONE_LED] = {0,};

    // Prompt the user to enter the LED number
        int ledNumber= 0 ;
        std::cout << "Enter the LED number to turn on (0 to 11): ";
        std::cin >> ledNumber;

    while (true)
   {
      ledController.turnOffLeds(tx, NUM_OF_LEDS * ONE_LED_DATA);
      ledController.performSPITransfer(ledController, tx, rx, NUM_OF_LEDS * ONE_LED_DATA);
      sleep(1);

      ledController.turnOnLed(tx, NUM_OF_LEDS * ONE_LED_DATA, ledNumber);
      ledController.performSPITransfer(ledController, tx, rx, NUM_OF_LEDS * ONE_LED_DATA);
      sleep(1);

        //Conversion from Decimal to Binary(255,255,255)
        // std::bitset<24> binaryValue = ledController.convertToBinary(255, 255, 255);
        // std::cout << "Binary representation: " << binaryValue << std::endl;

    //    std::bitset<72> ledColor("111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
    //     ledController.turnLEDcolor(tx, NUM_OF_LEDS * ONE_LED_DATA, ledColor);
    //     ledController.performSPITransfer(ledController, tx, rx, NUM_OF_LEDS * ONE_LED_DATA);
    //     sleep(1);
       
    }//end while(1)

    ledController.close_spi_port();

    return 0;
}














































// /* Usage example of the JETGPIO library
//  * Compile with: g++ -Wall -o led_control_spi led_control_spi.cpp -ljetgpio
//  * Execute with: sudo ./led_control_spi
// */

// #include <iostream>
// #include <unistd.h>
// #include <jetgpio.h>
// #include <algorithm>

// #define NUM_OF_LEDS 12  //For this particular project
// #define ONE_LED_DATA 24
// #define BYTE_FOR_ONE_LED ONE_LED_DATA*3
// #define ON 0x06
// #define OFF 0x04


// // void waitforTransfer(int )

// void turnOffLeds( char *data, int size)
// {
//    std::fill(data, data + size, OFF);
//      std::cout << "Data OFF: ";
//       for (int i = 0; i < size; ++i) {
//          std::cout << static_cast<int>(data[i]) << " "; // Assuming data contains integers representing LED states
//       }
//       std::cout << std::endl;
// }

// void turnOnLeds( char *data, int size)
// {
//    std::fill(data, data + size, ON);
// }

// void turnOn_LED( char *data, int size,int LED_number)  //LED Num starts from 0
// {
//    if (LED_number>=0 && LED_number<12)
//    {
//       int start_index = LED_number*ONE_LED_DATA;
//       std::fill( data+start_index,data+start_index+ONE_LED_DATA, ON);

//       std::cout << "Data ON: ";
//          for (int i = 0; i < size; ++i) {
//             std::cout << static_cast<int>(data[i]) << " "; // Assuming data contains integers representing LED states
//          }
//          std::cout << std::endl;
//    }
//    else
//    {
//       std::cout<<"Wrong LED Number Specified. Must be between 0 & 11"<<std::endl;
//    }
   
// }

// int main(int argc, char *argv[])
// {
//    int Init;
//    int SPI_init;
//    int SPI_stat;
//    int data_size = NUM_OF_LEDS*ONE_LED_DATA;
//    char* tx= new char [data_size];  //24
//    char rx[24] = {0,};

//    Init = gpioInitialise();
//    if (Init < 0)
//    {
//       /* JETGPIO initialization failed */
//       printf("JETGPIO initialization failed. Error code:  %d\n", Init);
//       exit(Init);
//    }
//    else
//    {
//       /* JETGPIO initialized okay*/
//       printf("JETGPIO initialization OK. Return code:  %d\n", Init);
//    }

//    SPI_init = spiOpen(0, 2500000, 0, 0, 8, 1, 1);  
//    if (SPI_init < 0)
//    {
//       /* SPI port opening failed */
//       printf("SPI port opening failed. Error code:  %d\n", SPI_init);
//       exit(Init);
//    }
//    else
//    {
//       /* SPI port opened  okay*/
//       printf("SPI port opened OK. Return code:  %d\n", SPI_init);
//    }


//    while(1)
//    {
//       turnOffLeds(tx,data_size);

//       SPI_stat = spiXfer(SPI_init, tx, rx,data_size);     //200   //72
//          if (SPI_stat >= 0)
//          {
//             printf("Transfered data: ");
//             printf("\n");
//          }
//          else
//          {
//             printf("SPI transfer failed\n");
//          }
//          sleep(1);

//          // turnOnLeds(tx,data_size);
//          turnOn_LED( tx, data_size,5) ;


//       SPI_stat = spiXfer(SPI_init, tx, rx,data_size);     //200   //72
//          if (SPI_stat >= 0)
//          {
//             printf("Transfered data: ");
//             printf("\n");
//          }
//          else
//          {
//             printf("SPI transfer failed\n");
//          }
//          sleep(1);
// }
   
//    // Closing SPI port
//    spiClose(SPI_init);

//    // Terminating library
//    gpioTerminate();

//    exit(0);
// }





















