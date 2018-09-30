#include <mcs51/8051.h>
#include "nRF24L01.h"
#define LED P1_0 //P1_0接LED

void main()
{
//检测nRF24L01是否存在
while(LED=nRF24L01_check());
//初始化nRF24L01
nRF24L01_configuration();

while(1)
	{
	
	}
}
