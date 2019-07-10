#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

void delay_us(int);
void setpwm(int, int);
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


int main(void){
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



while (true) {
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
      		 	setpwm(i,j);		// Output the 2 PWM frequencies
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

return 0;
}
void setpwm(int row, int col){

fseek(per,SEEK_SET,0);
fseek(per1,SEEK_SET,0);
fseek(dut,SEEK_SET,0);
fseek(dut1,SEEK_SET,0);
fseek(run,SEEK_SET,0);
fseek(run1,SEEK_SET,0);





fprintf(per,"%d\n",pwm[row]);	// The inputs row and col are the row and columns of the key that's pressed.
fprintf(per1,"%d\n",pwm1[col]);	// The inputs row and col are the row and columns of the key that's pressed.


fprintf(dut,"%d\n",pwm[row]/2);	
fprintf(dut1,"%d\n",pwm1[col]/2);	// FILL IN CODE to set the appropriate PWM periods and duties based on
						// the row and col inputs.
fprintf(run,"%d\n",1);
fprintf(run1,"%d\n",1);
	return;
}
			
		
void resetpwm(void){
	// FILL IN CODE to set the "run" values of both PWMs to 0.

fseek(run,SEEK_SET,0);
fseek(run1,SEEK_SET,0);

fprintf(run,"%d\n",0);
fprintf(run1,"%d\n",0);
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
