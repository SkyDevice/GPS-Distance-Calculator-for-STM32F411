#include "stm32f4xx_gpio.h"
#include "stm32f4xx.h"
#include "main.h"

/* mcu configuration */
void mcu_ClassConfig(void)
{
	/* enable the floating point unit */
	mcu_FloatConfig();
	mcu_ClockConfig();
	mcu_AccesConfig();
	mcu_UsartConfig();
}

void mcu_ClockConfig(void)
{
	/* resets the clock configuration */
	RCC_DeInit();
	
	/* enable HSI clock */
	RCC_HSICmd(ENABLE);
	
	/* wait until HSI is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	
	/* configure the PLL for SysClk */
	RCC_PLLConfig(RCC_PLLSource_HSI, 16, 400, 4, 4);
	
	/* enable PLL */
	RCC_PLLCmd(ENABLE);
	
	/* wait until main PLL clock ready */
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	
	/* set AHB divider, 100 MHz */
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	
	/* set APB1 divider, 50 MHz */
	RCC_PCLK1Config(RCC_HCLK_Div2);
	
	/* set APB2 divider, 100 MHz */
	RCC_PCLK2Config(RCC_HCLK_Div1);
	
	/* set system clock source using PLL */
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	
	SystemCoreClockUpdate();
}

void mcu_AccesConfig(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* GPIO(A - LED) Configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* GPIO(C - Button) Configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13); // Button
	
	/* EXTI(Button) Configuration */
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	/* NVIC(Button) Configuration */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void mcu_UsartConfig(void)
{
  GPIO_InitTypeDef 		GPIO_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	/* GPIO(A - USART 2) Configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	/* USART(2) Configuration */
  USART_InitStructure.USART_BaudRate = 19200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
	
  USART_ITConfig(USART2,USART_IT_RXNE, ENABLE);
  USART_Cmd(USART2, ENABLE);
}

// description: volatile
// in c wird durch diesen (typqualifikator) spezifiziert,
// dass sich der wert der variable jederzeit ändern kann
void delay(volatile uint32_t nCount)
{
  while(nCount--){}
}

void delay_led(volatile uint32_t time)
{
	time = time *1000;
	
	GPIOA -> BSRRL = GPIO_Pin_5;	// GPIO port bit set/reset low register
	delay(time);
	GPIOA -> BSRRH = GPIO_Pin_5;	// GPIO port bit set/reset high register
	delay(time);
}
