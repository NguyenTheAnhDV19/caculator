#include <timer.h>

void delay(unsigned int millisecond)
{
	unsigned int i;
	for(i=0;i<=millisecond*100;++i);
}