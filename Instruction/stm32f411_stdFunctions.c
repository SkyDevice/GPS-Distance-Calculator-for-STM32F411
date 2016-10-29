#include "main.h"

/* mcu functions */
void mcu_ClockReport(void)
{
	RCC_ClocksTypeDef     RCC_Clocks;
	
	RCC_GetClocksFreq(&RCC_Clocks);
	printf("************************************\n\r");
	printf("* Nucleo STM32F411RE Clock Frequency\n\r");
	printf("* SYSCLK Frequency  (CORE): %4d MHz\r\n", (RCC_Clocks.SYSCLK_Frequency / 1000000));
	printf("* AHB    Frequency  (HCLK): %4d MHz\r\n", (RCC_Clocks.HCLK_Frequency   / 1000000));
	printf("* APB1   Frequency (PCLK1): %4d MHz\r\n", (RCC_Clocks.PCLK1_Frequency  / 1000000));
	printf("* APB2   Frequency (PCLK2): %4d MHz\r\n", (RCC_Clocks.PCLK2_Frequency  / 1000000));
	printf("************************************\n\r");
	printf("\n\r");
}

/* dsp functions */
void dsp_ClockReport(void)
{
	RCC_ClocksTypeDef  RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	
	font_fg = COLOR_GRAY;
	print("NUCLEO STM32F411RE\n");
	print("Clock Frequency\n");
	print("SYSCLK   (CORE):  %d.00 MHz\n",  (RCC_Clocks.SYSCLK_Frequency / 1000000));
	print("AHB      (HCLK):  %d.00 MHz\n",  (RCC_Clocks.HCLK_Frequency   / 1000000));
	print("APB1    (PCLK1):   %d.00 MHz\n", (RCC_Clocks.PCLK1_Frequency  / 1000000));
	print("APB2    (PCLK2):  %d.00 MHz\n",  (RCC_Clocks.PCLK2_Frequency  / 1000000));
	print("\n");
}
