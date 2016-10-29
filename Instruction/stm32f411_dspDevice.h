#include "stm32f4xx.h"
#include <stdio.h>
#include <stdlib.h>

/* display defines */
#define Diaplay_SPI 						SPI1
#define Diaplay_SPI_PINS				Display_SPI_PinsPack_1
#define Diaplay_CS_CLK					RCC_AHB1Periph_GPIOB
#define Diaplay_CS_PORT					GPIOB
#define Diaplay_CS_PIN					GPIO_Pin_6
#define Diaplay_WRX_CLK					RCC_AHB1Periph_GPIOC
#define Diaplay_WRX_PORT				GPIOC
#define Diaplay_WRX_PIN					GPIO_Pin_7
#define Diaplay_BL_LED					RCC_AHB1Periph_GPIOB
#define Diaplay_BL_PORT					GPIOB
#define Diaplay_BL_PIN					GPIO_Pin_3
#define Diaplay_USE_RST_PIN			0
#define Diaplay_RST_CLK					RCC_AHB1Periph_GPIOD
#define Diaplay_RST_PORT				GPIOD
#define Diaplay_RST_PIN					GPIO_Pin_12

#define Diaplay_RST_SET					GPIO_SetBits(Diaplay_RST_PORT, Diaplay_RST_PIN)
#define Diaplay_RST_RESET				GPIO_ResetBits(Diaplay_RST_PORT, Diaplay_RST_PIN)
#define Diaplay_CS_SET					GPIO_SetBits(Diaplay_CS_PORT, Diaplay_CS_PIN)
#define Diaplay_CS_RESET				GPIO_ResetBits(Diaplay_CS_PORT, Diaplay_CS_PIN)
#define Diaplay_WRX_SET					GPIO_SetBits(Diaplay_WRX_PORT, Diaplay_WRX_PIN)
#define Diaplay_WRX_RESET				GPIO_ResetBits(Diaplay_WRX_PORT, Diaplay_WRX_PIN)

/* display settings */
#define Diaplay_WIDTH 					240
#define Diaplay_HEIGHT					320
#define Diaplay_PIXEL						76800

/* display commands */
#define Diaplay_RESET						0x01
#define Diaplay_SLEEP_OUT				0x11
#define Diaplay_GAMMA						0x26
#define Diaplay_DISPLAY_OFF			0x28
#define Diaplay_DISPLAY_ON			0x29
#define Diaplay_COLUMN_ADDR			0x2A
#define Diaplay_PAGE_ADDR				0x2B
#define Diaplay_GRAM						0x2C
#define Diaplay_MAC							0x36
#define Diaplay_PIXEL_FORMAT		0x3A
#define Diaplay_WDB							0x51
#define Diaplay_WCD							0x53
#define Diaplay_RGB_INTERFACE		0xB0
#define Diaplay_FRC							0xB1
#define Diaplay_BPC							0xB5
#define Diaplay_DFC							0xB6
#define Diaplay_POWER1					0xC0
#define Diaplay_POWER2					0xC1
#define Diaplay_VCOM1						0xC5
#define Diaplay_VCOM2						0xC7
#define Diaplay_POWERA					0xCB
#define Diaplay_POWERB					0xCF
#define Diaplay_PGAMMA					0xE0
#define Diaplay_NGAMMA					0xE1
#define Diaplay_DTCA						0xE8
#define Diaplay_DTCB						0xEA
#define Diaplay_POWER_SEQ				0xED
#define Diaplay_3GAMMA_EN				0xF2
#define Diaplay_INTERFACE				0xF6
#define Diaplay_PRC							0xF7

/* display colors */
#define COLOR_WHITE							0xFFFF
#define COLOR_BLACK							0x0000
#define COLOR_RED								0xF800
#define COLOR_GREEN							0x07E0
#define COLOR_GREEN2						0xB723
#define COLOR_BLUE							0x001F
#define COLOR_BLUE2							0x051D
#define COLOR_YELLOW						0xFFE0
#define COLOR_ORANGE						0xFBE4
#define COLOR_CYAN							0x07FF
#define COLOR_MAGENTA						0xA254
#define COLOR_GRAY							0x7BEF
#define COLOR_BROWN							0xBBCA
#define TRANSPARENT							0x80000000

#define Display_SPI1_PRESCALER		SPI_BaudRatePrescaler_8
#define Display_SPI1_DATASIZE 		SPI_DataSize_8b
#define Display_SPI1_FIRSTBIT 		SPI_FirstBit_MSB
#define Display_SPI1_MASTERSLAVE	SPI_Mode_Master
#define Display_SPI1_MODE					Display_SPI_Mode_0

/* structuren */
typedef enum
{
	Display_SPI_Mode_0,		// clock polarity low, clock phase 1st edge
	Display_SPI_Mode_1,		// clock polarity low, clock phase 2nd edge
	Display_SPI_Mode_2,		// clock polarity high, clock phase 1st edge
	Display_SPI_Mode_3		// clock polarity high, clock phase 2nd edge
} Display_SPI_Mode_t;

typedef enum
{
	Display_SPI_PinsPack_1,
	Display_SPI_PinsPack_2,
	Display_SPI_PinsPack_3
} Display_SPI_PinsPack_t;

typedef enum
{
	Portrait_1,
	Portrait_2,
	Landscape_1,
	Landscape_2
} Display_Orientation_t;

typedef enum
{
	Display_Landscape,
	Display_Portrait
} Display_Orientation;

typedef struct
{
	uint16_t width;
	uint16_t height;
	Display_Orientation orientation; // 1 = portrait; 0 = landscape
} Display_Options_t;

/* display spi prototypes */
void Display_SPI_Init(SPI_TypeDef* SPIx, Display_SPI_PinsPack_t pinspack);
void Display_SPI_InitWithMode(SPI_TypeDef* SPIx, Display_SPI_PinsPack_t pinspack, Display_SPI_Mode_t SPI_Mode);
void Display_SPI_InitFull(SPI_TypeDef* SPIx, Display_SPI_PinsPack_t pinspack, uint16_t SPI_BaudRatePrescaler, Display_SPI_Mode_t SPI_Mode_t, uint16_t SPI_Mode, uint16_t SPI_FirstBit);
void Display_SPI_SendMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint8_t* dataIn, uint16_t count);
void Display_SPI_WriteMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint16_t count);
void Display_SPI_ReadMulti(SPI_TypeDef* SPIx, uint8_t *dataIn, uint8_t dummy, uint16_t count);
void Display_SPI_SendMulti16(SPI_TypeDef* SPIx, uint16_t* dataOut, uint16_t* dataIn, uint16_t count);
void Display_SPI_WriteMulti16(SPI_TypeDef* SPIx, uint16_t* dataOut, uint16_t count);
void Display_SPI_ReadMulti16(SPI_TypeDef* SPIx, uint16_t* dataIn, uint16_t dummy, uint16_t count);
uint16_t	Display_SPI_GetPrescalerFromMaxFrequency(SPI_TypeDef* SPIx, uint32_t MAX_SPI_Frequency);
uint8_t		Display_SPI_Send(SPI_TypeDef* SPIx, uint8_t data);
uint16_t	Display_SPI_Send16(SPI_TypeDef* SPIx, uint16_t data);

/* display basic functions */
void Display_Init(void);
void Display_InitLCD(void);
void Display_SendData(uint8_t data);
void Display_SendCommand(uint8_t data);
void Display_Delay(volatile unsigned int delay);
void Display_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/* display functions */
void Display_Rotate(Display_Orientation_t orientation);
void Display_Fill(uint32_t color);
void Display_DrawPixel(uint16_t x, uint16_t y, uint32_t color);
void Display_DrawFilledRectangle(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint32_t color);

/* private functions */
void display_putchar(int character);
void display_putstr(const char *str);
char *itoa(int i, char b[]);
void newLine(void);
void print(const char *fmt, ...);
