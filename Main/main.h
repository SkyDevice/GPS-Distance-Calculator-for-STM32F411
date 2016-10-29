#include "stm32f4xx.h"
#include "stm32f411_dspDevice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* global display variables */
extern uint16_t gDCPx; 			// Display Coursor Position X
extern uint16_t gDCPy; 			// Display Coursor Position Y
extern uint32_t font_fg;		// Schriftart VordergrundFarbe
extern uint32_t font_bg;		// Schriftart HintergrundFarbe

/* extern printf function */
extern int Serial_GetChar(void);
extern int Serial_PutChar(int c);

/* mcu configuration */
void mcu_ClassConfig(void);
void mcu_FloatConfig(void);
void mcu_ClockConfig(void);
void mcu_AccesConfig(void);
void mcu_UsartConfig(void);

/* mcu functions */
void mcu_ClockReport(void);

/* dsp configuration */
void dsp_ClassConfig(void);

/* dsp functions */
void dsp_ClockReport(void);

/* gps struct for coordinates */
typedef struct
{
	float Lat;
	float Lon;
	char Address[45];
} PT_T;

/* struct for nmea time variables */
typedef struct
{
int timeHH;
int timeMM;
int timeSS;
} Timedata;

/* struct for nmea protocol variables */
typedef struct
{
char prcNMEA[21];
char utcNMEA[21];
char latNMEA[21];
char lapNMEA;
char lonNMEA[21];
char lopNMEA;
int  gpsNMEA;
char satNMEA;
} NMEAdata;

/* gps configuration */
void gps_ClassConfig(void);
void gps_UsartConfig(void);

/* gps functions */
void NMEA_tokString(char *nmeaData);
void NMEA_prtString(void);
void NMEA_timString(char *timeData);
void NMEA_spaBranch(float latitude, float longitude);

/* gps coordinates and distance functions */
float NMEA_conCoordinates(float coordinates);
float NMEA_calDistance(float latitude, float longitude, const PT_T * p2);
void	NMEA_prtDistance(float d, float sparlat, float sparlon, float latitude, float longitude);

/* private usart function */
void USART_SendString(char *strData);

void NMEA_dspString(float latitude, float longitude);

/* delay */
void delay(volatile uint32_t nCount);
void delay_led(volatile uint32_t time);
