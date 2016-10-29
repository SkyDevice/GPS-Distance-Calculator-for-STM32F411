#include "stm32f411_dspDevice.h"
#include "stm32f411_dspFonts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

/* global display coursor position */
uint16_t gDCPx 	= 4;
uint16_t gDCPy 	= 4;

/* global display options */
Display_Options_t Diaplay_Opts;

/* global font types */
FontDef_t *font		= &Font_7x10;		// set fonttype
uint32_t font_fg	= COLOR_WHITE;	// set fontcolour foreground
uint32_t font_bg	= COLOR_BLACK;	// set fontcolour background

/* dsp configuration */
void dsp_ClassConfig()
{
	Display_Init();
	Display_Rotate(Landscape_2);
}

/* display setup */
void Display_Init()
{
	GPIO_InitTypeDef GPIO_InitDef;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(Diaplay_WRX_CLK, ENABLE);
	  
	GPIO_InitDef.GPIO_Pin = Diaplay_WRX_PIN;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(Diaplay_WRX_PORT, &GPIO_InitDef);
	
	RCC_AHB1PeriphClockCmd(Diaplay_CS_CLK, ENABLE);
	GPIO_InitDef.GPIO_Pin = Diaplay_CS_PIN;
	GPIO_Init(Diaplay_CS_PORT, &GPIO_InitDef);	
	
	RCC_AHB1PeriphClockCmd(Diaplay_RST_CLK, ENABLE);
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitDef.GPIO_Pin = Diaplay_RST_PIN;
	GPIO_Init(Diaplay_RST_PORT, &GPIO_InitDef);	
	
	Diaplay_CS_SET;
	Display_SPI_Init(Diaplay_SPI, Diaplay_SPI_PINS);
	
	/* display backlight led */
	RCC_AHB1PeriphClockCmd(Diaplay_BL_LED, ENABLE);
  GPIO_InitStructure.GPIO_Pin = Diaplay_BL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(Diaplay_BL_PORT, &GPIO_InitStructure);
	
	/* setzen des pins auf low-level (backlight led off) */
	GPIO_WriteBit(Diaplay_BL_PORT, Diaplay_BL_PIN, Bit_RESET);
	
	Display_InitLCD();
	
	Diaplay_Opts.width = Diaplay_WIDTH;
	Diaplay_Opts.height = Diaplay_HEIGHT;
	Diaplay_Opts.orientation = Display_Landscape; // original display_portrait
	
	Display_Fill(COLOR_BLACK);
	Display_Delay(1000000);
	
	/* display backlight led on */
	GPIO_SetBits(GPIOB, GPIO_Pin_3);
}

/* display init */
void Display_InitLCD(void)
{
	Diaplay_RST_SET;
	Display_SendCommand(Diaplay_RESET);
	Display_Delay(2000000);
	Display_SendCommand(Diaplay_POWERA);
	Display_SendData(0x39);
	Display_SendData(0x2C);
	Display_SendData(0x00);
	Display_SendData(0x34);
	Display_SendData(0x02);
	Display_SendCommand(Diaplay_POWERB);
	Display_SendData(0x00);
	Display_SendData(0xC1);
	Display_SendData(0x30);
	Display_SendCommand(Diaplay_DTCA);
	Display_SendData(0x85);
	Display_SendData(0x00);
	Display_SendData(0x78);
	Display_SendCommand(Diaplay_DTCB);
	Display_SendData(0x00);
	Display_SendData(0x00);
	Display_SendCommand(Diaplay_POWER_SEQ);
	Display_SendData(0x64);
	Display_SendData(0x03);
	Display_SendData(0x12);
	Display_SendData(0x81);
	Display_SendCommand(Diaplay_PRC);
	Display_SendData(0x20);
	Display_SendCommand(Diaplay_POWER1);
	Display_SendData(0x23);
	Display_SendCommand(Diaplay_POWER2);
	Display_SendData(0x10);
	Display_SendCommand(Diaplay_VCOM1);
	Display_SendData(0x3E);
	Display_SendData(0x28);
	Display_SendCommand(Diaplay_VCOM2);
	Display_SendData(0x86);
	Display_SendCommand(Diaplay_MAC);
	Display_SendData(0x48);
	Display_SendCommand(Diaplay_PIXEL_FORMAT);
	Display_SendData(0x55);
	Display_SendCommand(Diaplay_FRC);
	Display_SendData(0x00);
	Display_SendData(0x18);
	Display_SendCommand(Diaplay_DFC);
	Display_SendData(0x08);
	Display_SendData(0x82);
	Display_SendData(0x27);
	Display_SendCommand(Diaplay_3GAMMA_EN);
	Display_SendData(0x00);
	Display_SendCommand(Diaplay_COLUMN_ADDR);
	Display_SendData(0x00);
	Display_SendData(0x00);
	Display_SendData(0x00);
	Display_SendData(0xEF);
	Display_SendCommand(Diaplay_PAGE_ADDR);
	Display_SendData(0x00);
	Display_SendData(0x00);
	Display_SendData(0x01);
	Display_SendData(0x3F);
	Display_SendCommand(Diaplay_GAMMA);
	Display_SendData(0x01);
	Display_SendCommand(Diaplay_PGAMMA);
	Display_SendData(0x0F);
	Display_SendData(0x31);
	Display_SendData(0x2B);
	Display_SendData(0x0C);
	Display_SendData(0x0E);
	Display_SendData(0x08);
	Display_SendData(0x4E);
	Display_SendData(0xF1);
	Display_SendData(0x37);
	Display_SendData(0x07);
	Display_SendData(0x10);
	Display_SendData(0x03);
	Display_SendData(0x0E);
	Display_SendData(0x09);
	Display_SendData(0x00);
	Display_SendCommand(Diaplay_NGAMMA);
	Display_SendData(0x00);
	Display_SendData(0x0E);
	Display_SendData(0x14);
	Display_SendData(0x03);
	Display_SendData(0x11);
	Display_SendData(0x07);
	Display_SendData(0x31);
	Display_SendData(0xC1);
	Display_SendData(0x48);
	Display_SendData(0x08);
	Display_SendData(0x0F);
	Display_SendData(0x0C);
	Display_SendData(0x31);
	Display_SendData(0x36);
	Display_SendData(0x0F);
	
	Display_SendCommand(Diaplay_SLEEP_OUT);
	Display_Delay(1000000);
	
	Display_SendCommand(Diaplay_DISPLAY_ON);
	Display_SendCommand(Diaplay_GRAM);
}

/* display sendcommand */
void Display_SendCommand(uint8_t data)
{
	Diaplay_WRX_RESET;
	Diaplay_CS_RESET;
	Display_SPI_Send(Diaplay_SPI, data);
	Diaplay_CS_SET;
}

/* display senddata */
void Display_SendData(uint8_t data)
{
	Diaplay_WRX_SET;
	Diaplay_CS_RESET;
	Display_SPI_Send(Diaplay_SPI, data);
	Diaplay_CS_SET;
}

/* display delay */
void Display_Delay(volatile unsigned int delay)
{
	for(; delay != 0; delay--); 
}

/* cursor position */
void Display_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	Display_SendCommand(Diaplay_COLUMN_ADDR);
	Display_SendData(x1 >> 8);
	Display_SendData(x1 & 0xFF);
	Display_SendData(x2 >> 8);
	Display_SendData(x2 & 0xFF);

	Display_SendCommand(Diaplay_PAGE_ADDR);
	Display_SendData(y1 >> 8);
	Display_SendData(y1 & 0xFF);
	Display_SendData(y2 >> 8);
	Display_SendData(y2 & 0xFF);
}

/* display orientation */
void Display_Rotate(Display_Orientation_t orientation)
{
	Display_SendCommand(Diaplay_MAC);
	if(orientation == Portrait_1)
	{
		Display_SendData(0x58);
	}
	else if(orientation == Portrait_2)
	{
		Display_SendData(0x88);
	} else if(orientation == Landscape_1)
	{
		Display_SendData(0x28);
	}
	else if(orientation == Landscape_2)
	{
		Display_SendData(0xE8);
	}
	
	if(orientation == Portrait_1 || orientation == Portrait_2)
	{
		Diaplay_Opts.width = Diaplay_WIDTH;
		Diaplay_Opts.height = Diaplay_HEIGHT;
		Diaplay_Opts.orientation = Display_Portrait;
	}
	else
	{
		Diaplay_Opts.width = Diaplay_HEIGHT;
		Diaplay_Opts.height = Diaplay_WIDTH;
		Diaplay_Opts.orientation = Display_Landscape;
	}
}

/* display fill */
void Display_Fill(uint32_t color)
{
	unsigned int n, i, j;
	i = color >> 8;
	j = color & 0xFF;
	Display_SetCursorPosition(0, 0, Diaplay_Opts.width - 1, Diaplay_Opts.height - 1);

	Display_SendCommand(Diaplay_GRAM);

	for(n = 0; n < Diaplay_PIXEL; n++)
	{
		Display_SendData(i);
		Display_SendData(j);
	}
}

/* get string size */
void Display_GetStringSize(char *str, FontDef_t *font, uint16_t *width, uint16_t *height)
{
	uint16_t w = 0;
	*height = font->FontHeight;
	
	while(*str++)
	{
		w += font->FontWidth;
	}
	*width = w;
}

/* draw pixel */
void Display_DrawPixel(uint16_t x, uint16_t y, uint32_t color)
{
	Display_SetCursorPosition(x, y, x, y);
	
	Display_SendCommand(Diaplay_GRAM);
	Display_SendData(color >> 8);
	Display_SendData(color & 0xFF);
}

/* display_drawfilledrectangle */
void Display_DrawFilledRectangle(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint32_t color)
{
	// WhatEverElse...
}

/* itoa (int to char*) */
char* itoa(int i, char b[])
{
	char const digit[] = "0123456789";
	char* p = b;
	int shifter = i;

	if(i<0) { *p++ = '-'; i *= -1; }
	
	do { ++p; shifter = shifter/10; }
	while(shifter); *p = '\0';
	
	do { *--p = digit[i%10]; i = i/10; }
	while(i);
	
	return b;
}

/* display putchar */
void display_putchar(int character)
{
	uint32_t i, b, j;
	
	if((gDCPx + font->FontWidth) > Diaplay_Opts.width)
	{
		// If at the end of a line of display, go to new line and set x to 4 position
		gDCPy = (gDCPy + font->FontHeight) + 4;
		gDCPx = 4;
	}
	
	if((gDCPy + font->FontHeight) > Diaplay_Opts.height)
	{
		gDCPy = 4;
		gDCPx = 4;
	}
	
	for(i=0; i<font->FontHeight; i++)
	{
		b = font->data[(character - 32) * font->FontHeight + i];
		
		for(j=0; j<font->FontWidth; j++)
		{
			if((b<<j) & 0x80)
			{
				Display_DrawPixel(gDCPx + j, (gDCPy + i), font_fg);
			}
			else if((font_bg & TRANSPARENT) == 0)
			{
				Display_DrawPixel(gDCPx + j, (gDCPy + i), font_bg);
			}
		}
	}
	gDCPx = (gDCPx + font->FontWidth) +1;
	
}

/* display putstring */
void display_putstr(const char *str)
{
	while(*str)
	{
		if(*str == '\n')
		{
			newLine();
			str++;
		}
		display_putchar(*str++);
	}
}

/* display newline */
void newLine(void)
{
	gDCPx = 4;
	gDCPy = (gDCPy + font->FontHeight) + 4;
}

/* meine print funktion */
void print(const char *fmt, ...)
{
	va_list argp;
	
	uint32_t i;
	const char *pointer;
	char *string;
	char fmtbuf[256];

	va_start(argp, fmt);

	for(pointer = fmt; *pointer != '\0'; pointer++)
	{
		if(*pointer == '\n')	{ newLine(); continue; }
		if(*pointer != '%')		{ display_putchar(*pointer); continue; }
		
		switch(*++pointer)
		{
			case 'c': // char
				i = va_arg(argp, int);
				display_putchar(i);
			break;

			case 'd': // int als string
				i = va_arg(argp, int);
				string = itoa(i, fmtbuf);
				display_putstr(string);
			break;

			case 's':	// string
				string = va_arg(argp, char *);
				display_putstr(string);
			break;

			case '%':
				display_putchar('%');
			break;
		}
	}
	va_end(argp);
}
