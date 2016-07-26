#include <stdio.h>
#include <stdint.h>
#include "DS18B20.h"

#define  DATA_PIN    5

int main(void)
{
  	if(wiringPiSetup() == -1)
    	return -1;

	if( InitDS1820(DATA_PIN) == 0 )
		return -1;

	do
	{
		if(FetchDS())
		{
			printf("Temp = %.2f\n",GetTemp());
			delay(1000);
			//break;
		}	
		else
		{
			//printf(" Failed\n");
			delayMicroseconds(10000);
		}

	}while(1);

  	return 0x00;

}