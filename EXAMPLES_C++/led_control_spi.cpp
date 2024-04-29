/* Usage example of the JETGPIO library
 * Compile with: g++ -Wall -o led_control_spi led_control_spi.cpp -ljetgpio
 * Execute with: sudo ./led_control_spi
*/

#include <iostream>
#include <unistd.h>
#include <jetgpio.h>

#define ONE_LED_DATA 24
#define BYTE_FOR_ONE_LED ONE_LED_DATA*3

void turnOffLeds( char (&data)[24])
{
   for (int i=0; i<24; i++)
   {
      data[i] = 0x04;
   }
}


int main(int argc, char *argv[])
{
   int Init;
   int SPI_init;
   int SPI_stat;
   char tx[24] = {0,};  //24
   char red[2] = {0,};
   char blue[24] = {0,};
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

   turnOffLeds(tx);

   //Green
   for (int i=0; i<8; i++)
   {
      tx[i] = 0x06;
   }
   for (int i=8; i<16; i++)
   {
      tx[i] = 0x04;
   }
  for (int i=16; i<24; i++)
   {
      tx[i] = 0x04;
   }

   //Red
   for (int i=0; i<8; i++)
   {
      red[i] = 0x04;
   }
   for (int i=8; i<16; i++)
   {
      red[i] = 0x06;
   }
  for (int i=16; i<24; i++)
   {
      red[i] = 0x04;
   }

//     //BLue
//    for (int i=0; i<8; i++)
//    {
//           blue[i] = 0x04;
//    }
//    for (int i=8; i<16; i++)
//    {
//       blue[i] = 0x04;
//    }
//   for (int i=16; i<24; i++)
//    {
//       blue[i] = 0x06;
//    }


   while(1)
   {
      SPI_stat = spiXfer(SPI_init, tx, rx,72);     //200   //72
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
         
         SPI_stat = spiXfer(SPI_init, red, rx,72);     //200
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

         // SPI_stat = spiXfer(SPI_init, blue, rx,72);     //200
         // if (SPI_stat >= 0)
         // {
         //    printf("Transfered data: ");
         //    printf("\n");
         // }
         // else
         // {
         //    printf("SPI transfer failed\n");
         // }
         // sleep(1);



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
