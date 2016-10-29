#include "stm32f411_dspDevice.h"

static void Display_SPI1_Init(Display_SPI_PinsPack_t pinspack, Display_SPI_Mode_t SPI_Mode, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_MasterSlave, uint16_t SPI_FirstBit);

void Display_SPI_Init(SPI_TypeDef* SPIx, Display_SPI_PinsPack_t pinspack)
{
	Display_SPI1_Init(pinspack, Display_SPI1_MODE, Display_SPI1_PRESCALER, Display_SPI1_MASTERSLAVE, Display_SPI1_FIRSTBIT);
}

void Display_SPI_InitWithMode(SPI_TypeDef* SPIx, Display_SPI_PinsPack_t pinspack, Display_SPI_Mode_t SPI_Mode)
{
	Display_SPI1_Init(pinspack, SPI_Mode, Display_SPI1_PRESCALER, Display_SPI1_MASTERSLAVE, Display_SPI1_FIRSTBIT);
}

void Display_SPI_InitFull( SPI_TypeDef* SPIx, Display_SPI_PinsPack_t pinspack, uint16_t SPI_BaudRatePrescaler, Display_SPI_Mode_t SPI_Mode_t, uint16_t SPI_Mode, uint16_t SPI_FirstBit)
{
	Display_SPI1_Init(pinspack, SPI_Mode_t, SPI_BaudRatePrescaler, SPI_Mode, SPI_FirstBit);
}

uint16_t Display_SPI_GetPrescalerFromMaxFrequency(SPI_TypeDef* SPIx, uint32_t MAX_SPI_Frequency)
{
	RCC_ClocksTypeDef RCC_Clocks;
	uint32_t APB_Frequency;
	uint8_t i;
	
	if(MAX_SPI_Frequency == 0) { return SPI_BaudRatePrescaler_256; }
	
	RCC_GetClocksFreq(&RCC_Clocks);
	
	if(SPIx == SPI1 || SPIx == SPI2)
	{
		APB_Frequency = RCC_Clocks.PCLK2_Frequency;
	}
	else
	{
		APB_Frequency = RCC_Clocks.PCLK1_Frequency;
	}
	
	for(i = 0; i < 8; i++)
	{
		if(APB_Frequency / (1 << (i + 1)) <= MAX_SPI_Frequency)
		{
			return (i << 3);
		}
	}
	return SPI_BaudRatePrescaler_256;
}

uint8_t Display_SPI_Send(SPI_TypeDef* SPIx, uint8_t data)
{
	SPIx->DR = data;
	while((SPIx->SR & SPI_I2S_FLAG_TXE) == 0);
	while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == 0);
	while (SPIx->SR & SPI_I2S_FLAG_BSY);
	
	return SPIx->DR;
}

void Display_SPI_SendMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint8_t* dataIn, uint16_t count)
{
	uint16_t i;
	for(i = 0; i < count; i++) { dataIn[i] = Display_SPI_Send(SPIx, dataOut[i]); }
}

void Display_SPI_WriteMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint16_t count)
{
	uint16_t i;
	for(i = 0; i < count; i++) { Display_SPI_Send(SPIx, dataOut[i]); }
}

void Display_SPI_ReadMulti(SPI_TypeDef* SPIx, uint8_t* dataIn, uint8_t dummy, uint16_t count)
{
	uint16_t i;
	for(i = 0; i < count; i++) { dataIn[i] = Display_SPI_Send(SPIx, dummy); }
}

uint16_t Display_SPI_Send16(SPI_TypeDef* SPIx, uint16_t data)
{
	SPIx->DR = data;
	while ((SPIx->SR & SPI_I2S_FLAG_TXE) == 0);
	while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == 0);
	while (SPIx->SR & SPI_I2S_FLAG_BSY);
	
	return SPIx->DR;
}

void Display_SPI_SendMulti16(SPI_TypeDef* SPIx, uint16_t* dataOut, uint16_t* dataIn, uint16_t count)
{
	uint16_t i;
	for(i = 0; i < count; i++) { dataIn[i] = Display_SPI_Send16(SPIx, dataOut[i]); }
}

void Display_SPI_WriteMulti16(SPI_TypeDef* SPIx, uint16_t* dataOut, uint16_t count)
{
	uint16_t i;
	for(i = 0; i < count; i++) { Display_SPI_Send16(SPIx, dataOut[i]); }
}

void Display_SPI_ReadMulti16(SPI_TypeDef* SPIx, uint16_t* dataIn, uint16_t dummy, uint16_t count)
{
	uint16_t i;
	for(i = 0; i < count; i++) { dataIn[i] = Display_SPI_Send16(SPIx, dummy); }
}

static void Display_SPI1_Init(Display_SPI_PinsPack_t pinspack, Display_SPI_Mode_t SPI_Mode, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_MasterSlave, uint16_t SPI_FirstBit)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;

	if(pinspack == Display_SPI_PinsPack_1)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	}
	else if(pinspack == Display_SPI_PinsPack_2)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	}
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
	SPI_InitStruct.SPI_DataSize = Display_SPI1_DATASIZE;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit;
	SPI_InitStruct.SPI_Mode = SPI_MasterSlave;
	
	if(SPI_Mode == Display_SPI_Mode_0)
	{
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	}
	else if(SPI_Mode == Display_SPI_Mode_1)
	{
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	}
	else if(SPI_Mode == Display_SPI_Mode_2)
	{
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	}
	else if(SPI_Mode == Display_SPI_Mode_3)
	{
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	}
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Cmd(SPI1, DISABLE);
	SPI_DeInit(SPI1);
	
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);
}
