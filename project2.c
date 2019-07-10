/** SPI C Transfer Example, Written by Derek Molloy (www.derekmolloy.ie)
*    for the book Exploring BeagleBone. Based on the spidev_test.c code
*    example at www.kernel.org
*
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<stdio.h>
#include<fcntl.h>
#include <stddef.h>
#include <time.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<stdint.h>
#include<linux/spi/spidev.h>
#include <sys/time.h>

#define SPI_PATH "/dev/spidev1.0"

int transfer(int fd, unsigned char send[], unsigned char receive[], int length){
   struct spi_ioc_transfer transfer;           //the transfer structure
   transfer.tx_buf = (unsigned long) send;     //the buffer for sending data
   transfer.rx_buf = (unsigned long) receive;  //the buffer for receiving data
   transfer.len = length;                      //the length of buffer
   transfer.speed_hz = 1000000;                //the speed in Hz
   transfer.bits_per_word = 8;                 //bits per word
   transfer.delay_usecs = 0;                   //delay in us

   // send the SPI message (all of the above fields, inc. buffers)
	int status = ioctl(fd, SPI_IOC_MESSAGE(1), &transfer);
   if (status < 0) {
      perror("SPI: SPI_IOC_MESSAGE Failed");
      return -1;
   }
   return status;
}

void delay_us(int);
void displayNumber(int, int);
void resetpwm(void);

FILE *per;	// period of P8_13
FILE *per1;	// period of P9_22
FILE *dut;	// duty of P8_13
FILE *dut1;	// duty of P9_22

FILE *run ;	//run value for P8_13
FILE *run1 ;	// run value for P9_22

FILE * row[4];	// array for rows
FILE * column[4];// array for colums
int pwm[4]={1435000,1299000,1174000,1063000};	// values of P8_13 period
int pwm1[4]={827100,748500,677000,612400};	// values of P9_22 period
int inputval;

unsigned int fd, i=0,j=0;                   //file handle and loop counter
   unsigned char value, null=0x00;         //sending only a single char
   uint8_t bits = 8, mode = 3;             //8-bits per word, SPI mode 3
   uint32_t speed = 1000000;               //Speed is 1 MHz
   
int main(){
   

   /*
   const unsigned char SevenSegmentDisplay[10] = {
	   0b00111111, 0b00000110, 0b01011011, 0b01001111 , 0b01100110 ,
	   0b01101101 , 0b01111101 , 0b00000111 , 0b01111111 , 0b01101111
   };*/



   const unsigned char LedMatrixColumns[8]= {
	   0b10000000, 0b010000000, 0b00100000, 0b00010000,
	   0b00001000 , 0b00000100, 0b00000010 , 0b00000001};

    const unsigned char CNumber0[8]={
	    0b00000000,
	    0b00111100,
	    0b01100110,
	    0b01101110,
	    0b01110110,
	    0b01100110,
	    0b01100110,
	    0b00111100};

   const unsigned char CNumber1[8]={
	   0b00000000,
	   0b00011000,
	   0b00111000,
	   0b00111000,
	   0b00011000,
	   0b00011000,
	   0b01111110,
	   0b01111110}; 

  const unsigned char CNumber2[8]={
	  0b00000000,
	  0b00111100,
	  0b01100110,
	  0b00000110,
	  0b00001100,
	  0b00110000,
	  0b01100000,
	  0b01111110};

  const unsigned char CNumber3[8]={
	 0b00000000,
	 0b00111100,
	 0b01100110,
	 0b00000110,
	 0b00011100,
	 0b00000110,
	 0b01100110,
	 0b00111100};

  const unsigned char CNumber4[8]={
	0b00000000,
	0b00001100,
	0b00011100,
	0b00101100,
	0b01001100,
	0b01111110,
	0b00001100,
	0b00001100};

   const unsigned char CNumber5[8]={
	0b00000000,
	0b01111110,
	0b01100000,
	0b01111100,
	0b00000110,
	0b00000110,
	0b01100110,
	0b00111100};

const unsigned char CNumber6[8]={
	0b00000000,
	0b00111100,
	0b01100110,
	0b01100000,
	0b01111100,
	0b01100110,
	0b01100110,
	0b00111100};

const unsigned char CNumber7[8]={
	0b00000000,
	0b01111110,
	0b01100110,
	0b00001100,
	0b00001100,
	0b00011000,
	0b00011000,
	0b00011000};

const unsigned char CNumber8[8]={
	0b00000000,
	0b00111100,
	0b01100110,
	0b01100110,
	0b00111100,
	0b01100110,
	0b01100110,
	0b00111100};

   const unsigned char LedMatrixRows[8]={
	   0b01111111, 0b10111111, 0b11011111, 0b11101111,
	   0b11110111, 0b11111011, 0b11111101, 0b11111110};
	   
	   
	   
	   // setting row and column pins
row[0]=fopen("/sys/class/gpio/gpio47/value","w");
row[1]=fopen("/sys/class/gpio/gpio46/value","w");
row[2]=fopen("/sys/class/gpio/gpio27/value","w");
row[3]=fopen("/sys/class/gpio/gpio65/value","w");


// setting periods
per=fopen("/sys/devices/ocp.2/pwm_test_P8_13.14/period","w");
per1=fopen("/sys/devices/ocp.2/pwm_test_P9_22.15/period","w");
dut=fopen("/sys/devices/ocp.2/pwm_test_P8_13.14/duty","w");
dut1=fopen("/sys/devices/ocp.2/pwm_test_P9_22.15/duty","w");

// setting run values of pwm pins to zero
run=fopen("/sys/devices/ocp.2/pwm_test_P8_13.14/run","w");
run1=fopen("/sys/devices/ocp.2/pwm_test_P9_22.15/run","w");

fprintf(run,"%d\n",0);
fprintf(run1,"%d\n",0);



  
   while(1){
   // Check that the properties have been set
   printf("SPI Mode is: %d\n", mode);
   printf("SPI Bits is: %d\n", bits);
   printf("SPI Speed is: %d\n", speed);
   printf("Counting through all of the LEDs:\n");
   
   int key_pressed = 0;			// Initially, assume that no key is pressed
	for (int i = 0; i <4 ; i++) {	
		fprintf(row[i],"%d\n",1);	// Send a high value to the GPIO for row i
		fseek(row[i],SEEK_SET,0);
		delay_us(500);			// Delay 0.5 ms
	
		
	for (int j =0 ; j < 4 ; j++) {

column[0]=fopen("/sys/class/gpio/gpio66/value","r");
column[1]=fopen("/sys/class/gpio/gpio67/value","r");
column[2]=fopen("/sys/class/gpio/gpio69/value","r");
column[3]=fopen("/sys/class/gpio/gpio68/value","r");

	fscanf(column[j],"%d",&inputval);	// read value of column j

fclose(column[0]);
fclose(column[1]);
fclose(column[2]);
fclose(column[3]);

			if (inputval==1) { 	// Check the GPIO value for column j
				printf("i=%d  j=%d\n",i,j);
				key_pressed = 1;	// We know that a key has been pressed
      		 	displayNumber(i,j);		// Output the 2 PWM frequencies
			}
		}
		delay_us(500);		// Delay 0.5 ms
		fprintf(row[i],"%d\n",0);   // Send a low value to the GPIO for row i
		fseek(row[i],SEEK_SET,0);
		delay_us(1000);		// Delay 1 ms
	}
	if (key_pressed == 0) {
		resetpwm();
	}
}



fclose(per);
fclose(per1);
fclose(dut);
fclose(dut1);
fclose(run);
fclose(run1);
fclose(row[0]);
fclose(row[1]);
fclose(row[2]);
fclose(row[3]);
fclose(column[0]);
fclose(column[1]);
fclose(column[2]);
fclose(column[3]);
   
   
   
 
  };
   
   return 0;
 }
 
 
 
 
 
 void displayNumber(int row, int col){
 // The following calls set up the SPI bus properties
   if ((fd = open(SPI_PATH, O_RDWR))<0){
      perror("SPI Error: Can't open device.");
      return -1;
   }
   if (ioctl(fd, SPI_IOC_WR_MODE, &mode)==-1){
      perror("SPI: Can't set SPI mode.");
      return -1;
   }
   if (ioctl(fd, SPI_IOC_RD_MODE, &mode)==-1){
      perror("SPI: Can't get SPI mode.");
      return -1;
   }
   if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits)==-1){
      perror("SPI: Can't set bits per word.");
      return -1;
   }
   if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits)==-1){
      perror("SPI: Can't get bits per word.");
      return -1;
   }
   if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed)==-1){
      perror("SPI: Can't set max speed HZ");
      return -1;
   }
   if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed)==-1){
      perror("SPI: Can't get max speed HZ.");
      return -1;
   }

   
 if(row==0 && col== 0){
	  for (i = 0; i < 8; i++) { 

	   
 for(j=0; j<8; j++){	 
	if (transfer(fd, (unsigned char*) (LedMatrixRows+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}
     if (transfer(fd, (unsigned char*) (CNumber1+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}	
        usleep(1500);       //sleep for 1s each loop
      }
  }
 };
 
 
 if(row==0 && col==1){
	 for (i = 0; i < 8; i++) { 

	   
 for(j=0; j<8; j++){	 
	if (transfer(fd, (unsigned char*) (LedMatrixRows+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}
     if (transfer(fd, (unsigned char*) (CNumber2+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}	
        usleep(1500);       //sleep for 1s each loop
      }
  }
 };
 
 
 if(row==0 && col==2){
	 for (i = 0; i < 8; i++) { 

	   
 for(j=0; j<8; j++){	 
	if (transfer(fd, (unsigned char*) (LedMatrixRows+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}
     if (transfer(fd, (unsigned char*) (CNumber3+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}	
        usleep(1500);       //sleep for 1s each loop
      }
  }
 };
 
 
 if(row==1 && col== 0){
	 for (i = 0; i < 8; i++) { 

	   
 for(j=0; j<8; j++){	 
	if (transfer(fd, (unsigned char*) (LedMatrixRows+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}
     if (transfer(fd, (unsigned char*) (CNumber4+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}	
        usleep(1500);       //sleep for 1s each loop
      }
  }
	 
 };
 
 if(row==1 && col==1){
	 for (i = 0; i < 8; i++) { 

	   
 for(j=0; j<8; j++){	 
	if (transfer(fd, (unsigned char*) (LedMatrixRows+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}
     if (transfer(fd, (unsigned char*) (CNumber5+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}	
        usleep(1500);       //sleep for 1s each loop
      }
  }
 };
 
 if(row==1 && col==2){
	 for (i = 0; i < 8; i++) { 

	   
 for(j=0; j<8; j++){	 
	if (transfer(fd, (unsigned char*) (LedMatrixRows+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}
     if (transfer(fd, (unsigned char*) (CNumber6+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}	
        usleep(1500);       //sleep for 1s each loop
      }
  }
 };
 
 if(row==2 && col==0){
	 for (i = 0; i < 8; i++) { 

	   
 for(j=0; j<8; j++){	 
	if (transfer(fd, (unsigned char*) (LedMatrixRows+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}
     if (transfer(fd, (unsigned char*) (CNumber7+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}	
        usleep(1500);       //sleep for 1s each loop
      }
  }
 };
 
 if(row==2 && col== 1){
	 for (i = 0; i < 8; i++) { 

	   
 for(j=0; j<8; j++){	 
	if (transfer(fd, (unsigned char*) (LedMatrixRows+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}
     if (transfer(fd, (unsigned char*) (CNumber8+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}	
        usleep(1500);       //sleep for 1s each loop
      }
  }
 };
 
 if(row==2 && col==2){
	 for (i = 0; i < 8; i++) { 

	   
 for(j=0; j<8; j++){	 
	if (transfer(fd, (unsigned char*) (LedMatrixRows+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}
     if (transfer(fd, (unsigned char*) (CNumber1+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}	
        usleep(1500);       //sleep for 1s each loop
      }
 }
 };
 
 if(row==3 && col==1){
	 for (i = 0; i < 8; i++) { 

	   
 for(j=0; j<8; j++){	 
	if (transfer(fd, (unsigned char*) (LedMatrixRows+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}
     if (transfer(fd, (unsigned char*) (CNumber0+j), &null, 1)==-1){
	   perror("Failed to update the display");
	   return -1;
	}	
        usleep(1500);       //sleep for 1s each loop
      }
  }
 };
 
 close(fd);               //close the file

	return;
}
			
		
void resetpwm(void){
	// FILL IN CODE to set the "run" values of both PWMs to 0.

	return;
}
	
void delay_us(int desired_delay_us){
// Function obtained from pp. 228-229 of:
// Barrett and Kridner, "Bad to the Bone", Morgan&Claypool Publishers, 2013

	struct timeval tv_start;	// start time hack
	struct timeval tv_now;		//current time hack
	int elapsed_time_us;

	gettimeofday(&tv_start,NULL);
	elapsed_time_us = 0;

	while(elapsed_time_us < desired_delay_us) {
		gettimeofday(&tv_now,NULL);
		if(tv_now.tv_usec >= tv_start.tv_usec)
			elapsed_time_us = tv_now.tv_usec - tv_start.tv_usec;
		else
			elapsed_time_us = (1000000 - tv_start.tv_usec) + tv_now.tv_usec;
	}


	return;


}	

