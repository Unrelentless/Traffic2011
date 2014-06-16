#include "delay.h"

void delay_ms(unsigned int ms)
	{
	int i;
	while(ms>0)
		{
		for(i=0; i<54; i++);
		ms--;
		}
	
	}
