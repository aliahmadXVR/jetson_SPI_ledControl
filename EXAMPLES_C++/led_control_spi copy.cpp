/* Usage example of the JETGPIO library
 * Compile with: g++ -Wall -o led_control_spi led_control_spi.cpp -ljetgpio
 * Execute with: sudo ./led_control_spi
*/

#include <iostream>
#include <unistd.h>
#include <jetgpio.h>
#include <algorithm>

#define NUM_OF_LEDS 12  //For this particular project
#define ONE_LED_DATA 24
#define BYTE_FOR_ONE_LED ONE_LED_DATA*3
#define ON 0x06
#define OFF 0x04


// void waitforTransfer(int )

void turnOffLeds( char *data, int size)
{
   std::fill(data, data + size, OFF);
     std::cout << "Data OFF: ";
      for (int i = 0; i < size; ++i) {
         std::cout << static_cast<int>(data[i]) << " "; // Assuming data contains integers representing LED states
      }
      std::cout << std::endl;
}

void turnOnLeds( char *data, int size)
{
   std::fill(data, data + size, ON);
}

void turnOn_LED( char *data, int size,int LED_number)  //LED Num starts from 0
{
   if (LED_number>=0 && LED_number<12)
   {
      int start_index = LED_number*ONE_LED_DATA;
      std::fill( data+start_index,data+start_index+ONE_LED_DATA, ON);

      std::cout << "Data ON: ";
         for (int i = 0; i < size; ++i) {
            std::cout << static_cast<int>(data[i]) << " "; // Assuming data contains integers representing LED states
         }
         std::cout << std::endl;
   }
   else
   {
      std::cout<<"Wrong LED Number Specified. Must be between 0 & 11"<<std::endl;
   }
   
}

int main(int argc, char *argv[])
{
   int Init;
   int SPI_init;
   int SPI_stat;
   int data_size = NUM_OF_LEDS*ONE_LED_DATA;
   char* tx= new char [data_size];  //24
   char rx[24] = {0,};

   Init = gpioInitialise();
   if (Init < 0)
   {
      /* JETGPIO initialization failed */
      printf("JETGPIO initialization failed. Error code:  %d\n", Init);
      exit(Init);
   }
   else
   {
      /* JETGPIO initialized okay*/
      printf("JETGPIO initialization OK. Return code:  %d\n", Init);
   }

   SPI_init = spiOpen(0, 2500000, 0, 0, 8, 1, 1);  
   if (SPI_init < 0)
   {
      /* SPI port opening failed */
      printf("SPI port opening failed. Error code:  %d\n", SPI_init);
      exit(Init);
   }
   else
   {
      /* SPI port opened  okay*/
      printf("SPI port opened OK. Return code:  %d\n", SPI_init);
   }


   while(1)
   {
      turnOffLeds(tx,data_size);

      SPI_stat = spiXfer(SPI_init, tx, rx,data_size);     //200   //72
         if (SPI_stat >= 0)
         {
            printf("Transfered data: ");
            printf("\n");
         }
         else
         {
            printf("SPI transfer failed\n");
         }
         sleep(1);

         // turnOnLeds(tx,data_size);
         turnOn_LED( tx, data_size,5) ;


      SPI_stat = spiXfer(SPI_init, tx, rx,data_size);     //200   //72
         if (SPI_stat >= 0)
         {
            printf("Transfered data: ");
            printf("\n");
         }
         else
         {
            printf("SPI transfer failed\n");
         }
         sleep(1);
}
   
   // Closing SPI port
   spiClose(SPI_init);

   // Terminating library
   gpioTerminate();

   exit(0);
}

































//-------------------------------------------------------------//
// #include <iostream>
// #include <unistd.h>
// #include <jetgpio.h>

// int main(int argc, char *argv[])
// {
//    int Init;
//    int SPI_init;
//    int SPI_stat;
//    char tx[7] = {0,};
//    char rx[7] = {0,};

//    Init = gpioInitialise();
//    if (Init < 0)
//    {
//       /* jetgpio initialisation failed */
//       printf("Jetgpio initialisation failed. Error code:  %d\n", Init);
//       exit(Init);
//    }
//    else
//    {
//       /* jetgpio initialised okay*/
//       printf("Jetgpio initialisation OK. Return code:  %d\n", Init);
//    }

//    /* Port SPI2 pins: 37, 22, 13 & 18
//       to perform a simple loop test, pins 22: SPI2_MISO & 37:SPI2_MOSI should be connected
//       with a short jumper cable, when the cable is disconnected the output on screen will show
//       all the rx values as zeros 
//       spiOpen() parameters go as follows: spiOpen(port number, speed in Hz, mode, cs pin delay in us, 
//       bits per word, least significant bit first, cs change)
//    */

//    SPI_init = spiOpen(1, 250000, 0, 0, 8, 1, 1);  //5000000 spiOpen(1, 5000000, 0, 0, 8, 1, 1);
//    if (SPI_init < 0)
//    {
//       /* Port SPI2 opening failed */
//       printf("Port SPI2 opening failed. Error code:  %d\n", SPI_init);
//       exit(Init);
//    }
//    else
//    {
//       /* Port SPI2 opened  okay*/
//       printf("Port SPI2 opened OK. Return code:  %d\n", SPI_init);
//    }

//    tx[0] = 0xFF;
//    tx[1] = 0xFF;
//    tx[2] = 0x00;
//    tx[3] = 0xCC;
//    tx[4] = 0xDD;
//    tx[5] = 0xFF;
//    tx[6] = 0x11;

//    //Transfer data through register numbers for x, y & z axis

//    while(1)
//    {
//       SPI_stat = spiXfer(SPI_init, tx, rx, 7);
//       sleep(1);
//    }
   
//    // Closing spi port
//    spiClose(SPI_init);

//    // Terminating library
//    gpioTerminate();

//    exit(0);
// } //end main 
