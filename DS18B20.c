#include <stdio.h>
#include <stdint.h>
#include "DS18B20.h"

typedef struct 
{
	uint8_t  bReadPin_DS;
	uint16_t wRawValue;
	float    fTemp;
}_DSCtrl;

_DSCtrl DSCtrl;

uint8_t InitDS1820(uint8_t ReadPin_DS1820)
{
	uint8_t  bCnt = 10;

 	DSCtrl.bReadPin_DS = ReadPin_DS1820;

 	while(bCnt)
 	{
 		if( ResetDS() )
 			return 1;
 		else
 		{
 			delayMicroseconds(1000);
 			bCnt--;
 		}
 	}

 	return 0;

}

uint8_t ResetDS(void)
{
  uint16_t wTimeOut = 2000;

  //Pull High wait for Voltage Stable
  pinMode(DSCtrl.bReadPin_DS, OUTPUT);

  digitalWrite(DSCtrl.bReadPin_DS, HIGH);
  delayMicroseconds(100);
  digitalWrite(DSCtrl.bReadPin_DS, LOW);
  delayMicroseconds(500);
 
  pinMode(DSCtrl.bReadPin_DS, INPUT);

  delayMicroseconds(60);
  do
  {

  	if( digitalRead(DSCtrl.bReadPin_DS) == 0x00 )
  	{
  		delayMicroseconds(300); //Wait Resistor Pull High
  		return 1;
  	}

  	delayMicroseconds(1);
 
  }while(wTimeOut--);

  return 0x00;
}

void WriteDS(uint8_t bData)
{
	uint8_t i;

	pinMode(DSCtrl.bReadPin_DS, OUTPUT);

	for(i=0;i<8;i++)
	{
		digitalWrite(DSCtrl.bReadPin_DS, HIGH);
		delayMicroseconds(5);
		digitalWrite(DSCtrl.bReadPin_DS, LOW);
		delayMicroseconds(10);

		if( bData&(0x01<<i) )
		{
			digitalWrite(DSCtrl.bReadPin_DS, HIGH);
			delayMicroseconds(50);
		}
		else
		{
			delayMicroseconds(50);
		}
	}
}

uint8_t ReadDS(void)
{
	uint8_t i;
	uint8_t x;
	uint8_t ReadData=0x00;

	for(i=0;i<8;i++)
	{
		pinMode(DSCtrl.bReadPin_DS, OUTPUT);

		digitalWrite(DSCtrl.bReadPin_DS, LOW);
		delayMicroseconds(2);
		digitalWrite(DSCtrl.bReadPin_DS, HIGH);

		pinMode(DSCtrl.bReadPin_DS, INPUT);
		delayMicroseconds(15);

		x = digitalRead(DSCtrl.bReadPin_DS);

		ReadData |= x<<i;

		delayMicroseconds(50);

	}
	return ReadData;
}


uint8_t FetchDS(void)
{
	uint8_t w[8];
	uint8_t i;
	uint8_t crc=0;

	if( !ResetDS() ) return 0;

	WriteDS(0xCC);
	WriteDS(0x44);

	if( !ResetDS() ) return 0;

	WriteDS(0xCC);
	WriteDS(0xBE);	

	for(i=0;i<9;i++)
	{
		w[i] = ReadDS();
		//printf("%02x ",w[i]);
   	}
   	//printf("\n");
  	
  	for(i=0;i<8;i++)
  	{
    	crc=CRC8(crc,w[i]);
  	}
 
	//printf("CRC = %d\n",crc);

	DSCtrl.wRawValue = (uint16_t)(w[1]<<8) | w[0] ;

	if( crc == w[8])
	{
		DSCtrl.fTemp = (float)DSCtrl.wRawValue*0.0625;
		return 1;
	}
	else
		return 0;

}

uint8_t CRC8(uint8_t crc, uint8_t data)
{
    uint8_t j;
    crc = crc ^data;
    for (j = 0; j < 8; j++)
    {
        if ((crc & 0x01) != 0) crc = (crc >> 1) ^ 0x8c;
        else crc = crc >> 1;
    }
    return crc;
}
 
 float GetTemp(void)
 {
 	return DSCtrl.fTemp;
 }