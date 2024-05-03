#include <iostream>
#include <unistd.h>
#include <jetgpio.h>
#include <algorithm>
#include <bitset>
#include <iomanip>
#include <bitset>
#include <cstring>
#include <array>

/**
 * LEDController class controls LED operations using SPI communication.
 */
class LEDController {
private:
    const int NUM_OF_LEDS;     // Number of LEDs
    const int ONE_LED_DATA;    // Size of data for one LED (in bytes)
    const int BYTE_FOR_ONE_LED;// Total bytes for one LED (including color data)
    const char ON;             // Value representing LED ON state
    const char OFF;            // Value representing LED OFF state
    int SPI_init;              // SPI initialization status

public:

    /**
     * Constructor to initialize the LEDController.
     * @param numOfLEDs Number of LEDs in the system.
     * @param oneLedData Size of data for one LED (in bytes).
     * @param byteForOneLed Total bytes for one LED (including color data).
     * @param onValue Value representing LED ON state.
     * @param offValue Value representing LED OFF state.
     */
    LEDController(int numOfLEDs, int oneLedData, int byteForOneLed, char onValue, char offValue) 
        : NUM_OF_LEDS(numOfLEDs), ONE_LED_DATA(oneLedData), BYTE_FOR_ONE_LED(byteForOneLed), ON(onValue), OFF(offValue), SPI_init(-1) {}

    /**
     * Initializes GPIO and SPI communication.
     * @return true if initialization is successful, false otherwise.
     */
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

    /**
     * Performs SPI data transfer.
     * @param ledController Reference to the LEDController object.
     * @param tx Transmit buffer.
     * @param rx Receive buffer.
     * @param dataSize Size of data to transfer.
     */
    void performSPITransfer(LEDController& ledController, char* tx, char* rx, int dataSize) {
      int SPI_stat = spiXfer(ledController.getSpiInit(), tx, rx, dataSize);
      if (SPI_stat >= 0) {
         std::cout << "Transfered data" << std::endl;
      } else {
         std::cerr << "SPI transfer failed" << std::endl;
      }
   }

    /**
     * Closes SPI port and terminates GPIO.
     */
    void close_spi_port() {
        if (SPI_init >= 0) {
            spiClose(SPI_init);
        }
        gpioTerminate();
    }

    /**
     * Turns off all LEDs.
     * @param data Pointer to the LED data array.
     * @param size Size of the LED data array.
     */
    void turnOffLeds(char *data, int size) {
        std::fill(data, data + size, OFF);
        std::cout << "Data OFF: ";
        for (int i = 0; i < size; ++i) {
            std::cout << static_cast<int>(data[i]) << " ";
        }
        std::cout << std::endl;
    }

    /**
     * Turns on all LEDs.
     * @param data Pointer to the LED data array.
     * @param size Size of the LED data array.
     */
    void turnOnLeds(char *data, int size) {
        std::fill(data, data + size, ON);
        std::cout << "Data ON: ";
        for (int i = 0; i < size; ++i) {
            std::cout << static_cast<int>(data[i]) << " ";
        }
        std::cout << std::endl;
    }

    /**
     * Turns on a specific LED.
     * @param data Pointer to the LED data array.
     * @param size Size of the LED data array.
     * @param LED_number LED number to turn on (0 to NUM_OF_LEDS - 1).
     */
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


    /**
     * Turns on LEDs with specified color.
     * @param data Pointer to the LED data array.
     * @param size Size of the LED data array.
     * @param binaryNumber 72-bit binary number representing LED colors.
     */
    void turnLEDcolor(char *data, int size, const std::bitset<72>& binaryNumber) 
    {
        for (int i = 0; i < size; ++i) {
        data[i] = binaryNumber[i];
        }        
        std::cout << "Data COLOR: ";
        for (int i = 0; i < size; ++i) {
            std::cout << static_cast<int>(data[i]) << " ";
        }
        std::cout << std::endl;
    }

    /**
     * Converts a decimal value to an 8-bit binary number.
     * @param decimalValue Decimal value to convert.
     * @return 8-bit binary representation of the decimal value.
     */
   std::bitset<8> convertToBinary(int decimalValue) {
      if (decimalValue < 0 || decimalValue > 255) {
         std::cerr << "Decimal value must be between 0 and 255" << std::endl;
         return std::bitset<8>(0); // Return all zeros in case of invalid input
      }
      return std::bitset<8>(decimalValue);
   }

    /**
     * Converts three decimal values to a 24-bit binary number.
     * @param decimalValue1 First decimal value.
     * @param decimalValue2 Second decimal value.
     * @param decimalValue3 Third decimal value.
     * @return 24-bit binary representation of the three decimal values.
     */
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

    /**
     * Retrieves the SPI initialization status.
     * @return SPI initialization status.
     */
    int getSpiInit() const {
        return SPI_init;
    }

    /**
     * Tests LED operation.
     * 
     * This function fills the LED data array with the LED colors represented by the provided 72-bit binary number.
     * 
     * @param data Pointer to the LED data array.
     * @param size Size of the LED data array.
     * @param binaryData 72-bit binary number representing LED colors.
     */
    std::bitset<72> expandBinary(std::bitset<24> binaryValue) {
    std::bitset<72> expandedBinary;
    int new_index = 0;

    for (int i = 0; i < 24; i++) {
        
        if (binaryValue[i] == 1)
        {
            expandedBinary[new_index] = 0;
            expandedBinary[new_index + 1] =1;
            expandedBinary[new_index + 2] = 1;
        }
        else 
        {
            expandedBinary[new_index] = 0;
            expandedBinary[new_index + 1] =0;
            expandedBinary[new_index + 2] = 1;
        }
        new_index+=3;
        
    }

    return expandedBinary;
    }

    std::array<char, 24> convertToChar(std::bitset<24> binaryValue) {
    std::array<char, 24> charArray;

        for (int i = 0; i < 24; ++i) {
            if (binaryValue[i] == 1) {
                charArray[i] = 0x06;
            } else {
                charArray[i] = 0x04;
            }
        }
        return charArray;
    }

    void turnOnLedSpecificColor(char *data, int size, int LED_number, const std::array<char, 24>& colorArray) {
    if (LED_number >= 0 && LED_number < NUM_OF_LEDS) {
        int start_index = LED_number * ONE_LED_DATA;
        // Copy the color array to the specified LED position in the data array using a loop
        for (int i = 0; i < ONE_LED_DATA; ++i) {
            data[start_index + i] = colorArray[i];
        }
        std::cout << "Data ON with Specific Color: ";
        for (int i = 0; i < size; ++i) {
            std::cout << static_cast<int>(data[i]) << " "; //it by default
        }
        std::cout << std::endl;
    } else {
        std::cout << "Wrong LED Number Specified. Must be between 0 & " << (NUM_OF_LEDS - 1) << std::endl;
    }
}



    private:    
    /**
     * Checks if a decimal value is valid (between 0 and 255).
     * @param decimalValue Decimal value to check.
     * @return true if the decimal value is valid, false otherwise.
     */
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

        int green = 255;
        int red = 255;
        int blue = 255;

        std::cout << "Enter Value of G (Between 0 to 255)";
        std::cin >> green;
        std::cout << "Enter Value of R (Between 0 to 255)";
        std::cin >> red;
        std::cout << "Enter Value of B (Between 0 to 255)";
        std::cin >> blue;

    while (true)
   {
        //LEDs OFF
      ledController.turnOffLeds(tx, NUM_OF_LEDS * ONE_LED_DATA);
      ledController.performSPITransfer(ledController, tx, rx, NUM_OF_LEDS * ONE_LED_DATA);
      sleep(1);

    //     //Specific LED ON
    //   ledController.turnOnLed(tx, NUM_OF_LEDS * ONE_LED_DATA, ledNumber);
    //   ledController.performSPITransfer(ledController, tx, rx, NUM_OF_LEDS * ONE_LED_DATA);
    //   sleep(1);

            //LEDs ALL ON
    //   ledController.turnOnLeds(tx, NUM_OF_LEDS * ONE_LED_DATA);
    //   ledController.performSPITransfer(ledController, tx, rx, NUM_OF_LEDS * ONE_LED_DATA);
    //   sleep(1);

        //Conversion from Decimal to Binary(255,255,255)
        std::bitset<24> binaryValue = ledController.convertToBinary(green , red, blue);   //(G,R,B)
        std::cout << "Binary representation: " << binaryValue << std::endl;

        // std::bitset<72> expandedBinary = ledController.expandBinary(binaryValue);
        // std::cout << "Expanded Binary representation: " << expandedBinary << std::endl;
   
        // ledController.turnLEDcolor(tx, NUM_OF_LEDS * ONE_LED_DATA, expandedBinary) ;
        // sleep(1);

        // Call the function to convert the binary number to a char array
        std::array<char, 24> result = ledController.convertToChar(binaryValue);

        // Print the result to verify
        std::cout << "Converted Char Array: ";
        for (char c : result) {
            std::cout << c << " ";
        }
        std::cout << std::endl;

        ledController.turnOnLedSpecificColor(tx, NUM_OF_LEDS * ONE_LED_DATA, ledNumber, result);
        ledController.performSPITransfer(ledController, tx, rx, NUM_OF_LEDS * ONE_LED_DATA);
        sleep(1);
       
    }//end while(1)

    ledController.close_spi_port();

    return 0;
}


// 110 110 110 110 110 110 110 110    110 110 110 110 110 110 110 110    100 100 100 100 100 100 100 100
// 
//4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4