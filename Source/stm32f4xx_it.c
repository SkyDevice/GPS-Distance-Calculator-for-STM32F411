#include "stm32f4xx_it.h"
#include "stm32f4xx.h"
#include "main.h"

void bugReport(void)
{
	int i;
	for(i=0; i<=2; i++)
	{
		delay_led(2000);
	}
	delay(1000);
}

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
		bugReport();
  }
}

void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
}

/* IRQ Handler(Button) */
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
		int i;
		for(i=0; i<5; i++)
		{
			delay_led(2000);
		}
		
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
}
