#include "main.h"

int main()
{
	/* mcu configuration */
	mcu_ClassConfig();
	
	/* dsp configuration */
	dsp_ClassConfig();
	
	/* gps configuration */
	gps_ClassConfig();
	
	while(0x01)
	{
		// fancy stuff here...
	}
	
}	// main ende
