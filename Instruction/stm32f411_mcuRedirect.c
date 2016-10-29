#include "main.h"
#include "stm32f4xx.h"
#include <rt_misc.h>
#include <stdio.h>

#pragma import(__use_no_semihosting_swi)

extern int Serial_GetChar(void);
extern int Serial_PutChar(int c);

extern void Display_Fill(uint32_t color);
extern bool usartFlag;

struct __FILE { int handle; };
FILE __stdout;
FILE __stdin;

int fputc(int c, FILE *f)				{ return (Serial_PutChar(c));	}
int fgetc(FILE *f)							{ return (Serial_GetChar());	}
int ferror(FILE *f)							{ return EOF; }
void _ttywrch(int c)						{ Serial_PutChar(c); }
void _sys_exit(int return_code) { label:  goto label; }

/* write character to Serial Port */
int Serial_PutChar(int ch)
{
	/* usart boolean for printf */
	if(usartFlag == true)
	{
		/* true: print to usart terminal */
		while(!(USART2->SR & 0x0080));
		USART2->DR=(ch & 0xFF);
	}
	else
	{
		/* false: print to display */
		if(ch >= 32 && ch <= 126)
		{
			print("%c", ch);
		}
		if(ch == '\n')
		{
			print("\n");
		}
	}
	
	/* reset display coursor position when char is a form feed */
	if(ch == 12)
	{
		gDCPx = 4;
		gDCPy = 4;
	}
	
  return (ch);
}

/* read character from Serial Port */
int Serial_GetChar(void)
{
	if(USART2->SR & 0x0020) { return (USART2->DR); 	}

  return (-1);
}
