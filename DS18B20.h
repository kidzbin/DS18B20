#include <wiringPi.h>



uint8_t InitDS1820(uint8_t ReadPin_DS1820);
uint8_t ResetDS(void);
uint8_t ReadDS(void);
uint8_t FetchDS(void);

float GetTemp(void);

void WriteDS(uint8_t bData);
uint8_t CRC8(uint8_t crc, uint8_t data);