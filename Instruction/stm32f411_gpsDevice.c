#include "main.h"
#include "math.h"

#define PI 3.141592623846f
#define HOLDTIME 4000000

/* extern waypoint struct */
extern const PT_T waypoints[];

/* struct for nmea time data */
Timedata time;

/* strcut for nmea protocol data */
NMEAdata nmea;

/* boolean for printf */
/* true: print to usart terminal */
/* false: print to display */
bool usartFlag = true;


/* nmea gps variables */
char usartReceive;
char strNMEA[100] = "";

/* gps configuration */
void gps_ClassConfig(void)
{
	gps_UsartConfig();
}

/* gps usart and nvic setup */
void gps_UsartConfig(void)
{
  GPIO_InitTypeDef 		GPIO_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef		NVIC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	
	/* GPIO(A - USART 6) Configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_USART6);
	
	/* USART(6) Configuration */
  USART_InitStructure.USART_BaudRate = 19200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART6, &USART_InitStructure);
	
	USART_ITConfig(USART6,USART_IT_RXNE, ENABLE);
	USART_Cmd(USART6, ENABLE);
	
	/* NVIC Configuration */
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

// ************************************************************ (required ?)
/* print string to usart */
void USART_SendString(char *strData)
{
	while(*strData)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		USART_SendData(USART2, *strData++);
	}
}

/* boolean for empty data fields */
bool flag = false;

/* periodical usart irq handler */
/* trigger: receiving data on usart 6 */
void USART6_IRQHandler(void)
{
	/* usart receive char */
	usartReceive = USART_ReceiveData(USART6);
	
	/* begin of the nmea protocol */
	if(USART_ReceiveData(USART6) == '$')
	{
		memset(strNMEA, '\0', 100);
	}
	
	/* replace empty data fields with the ? sign */
	if(USART_ReceiveData(USART6) == ',' && flag == true) { strcat(strNMEA, "?"); flag = false; }
	if(USART_ReceiveData(USART6) == ',') { flag = true; }
	else { flag = false; }
	
	/* attach the usart receive data to string */
	strcat(strNMEA, &usartReceive);
	
	/* the end of the nmea protocol */
	if(USART_ReceiveData(USART6) == 10)
	{
		if(!strncmp(strNMEA, "$GPGGA", 6))
		{
			/* the nmea protocol fractionize */
			NMEA_tokString(strNMEA); /* USART_SendString(strNMEA); */
		}
	}
	
	/* wait until receive not empty flag is reset */
	while(USART_GetFlagStatus(USART6, USART_FLAG_RXNE) == RESET);
}

/* fractionize the nmea protocol */
void NMEA_tokString(char *nmeaData)
{
	char delimiter[] = ",";
	char *partPtr = NULL;
	unsigned short int token = 0;
	char check[] = "?";
	
	/* initialize the first section */
	partPtr = strtok(nmeaData, delimiter);
	
	if(token == 0) { strncpy(nmea.prcNMEA, &partPtr[1], 5); }
	
	while(partPtr != NULL)
	{
		token++;
		
		/* initialize the next sections */
		partPtr = strtok(NULL, delimiter);
		
		if(token == 1) { strcpy(nmea.utcNMEA, partPtr); }
		if(token == 2) { strcpy(nmea.latNMEA, partPtr); }
		if(token == 3) { nmea.lapNMEA = *partPtr; }	
		if(token == 4) { strcpy(nmea.lonNMEA, partPtr); }
		if(token == 5) { nmea.lopNMEA = *partPtr; }
		if(token == 6) { nmea.gpsNMEA = atoi(partPtr); }
		if(token == 7) { nmea.satNMEA = atoi(partPtr); }
		
		delay(8000000);
		//delay(2000000);
	}
	
	/* disable usart6 */
	USART_Cmd(USART6, DISABLE);
	
	/* clear the usart terminal */
	putchar(12);
	
	/* only output valid data, no empty output */
	if((strcmp(check, nmea.latNMEA) != 0) || (strcmp(check, nmea.lonNMEA) != 0))
	{
		NMEA_prtString();
		delay(HOLDTIME);
	}
	else
	{
		/* print signal message to usart terminal */
		printf("Wait for valid signal!\n\r");
		
		/* print signal message to display */
		font_fg = COLOR_RED;
		Display_Fill(0x0000);
		
		usartFlag = false;
		printf("* Wait for valid signal!\n");
		usartFlag = true;
	}
	
	/* enable usart6 */
	USART_Cmd(USART6, ENABLE);
}

/* print the nmea protocol */
void NMEA_prtString(void)
{
	float latitude  = 0.0f;
	float longitude = 0.0f;
	char *end;
	
	/* print the nmea protocol variables */
	printf("Sentence ID (GPS): %c%c%c%c%c\n\r", nmea.prcNMEA[0], nmea.prcNMEA[1], nmea.prcNMEA[2], nmea.prcNMEA[3], nmea.prcNMEA[4]);
	printf("Time        (UTC): %s\n\r", nmea.utcNMEA);
	printf("Latitude    (DEG): %s %c\n\r", nmea.latNMEA, nmea.lapNMEA);
	printf("Longitude   (DEG): %s %c\n\r", nmea.lonNMEA, nmea.lopNMEA);
	printf("Quality     (SAT): %02i\n\r", nmea.gpsNMEA);
	printf("Satellites (VIEW): %02i\n\r", nmea.satNMEA);
	printf("\n\r");
	
	/* get and print the nmea time (UTC+2) */
	NMEA_timString(nmea.utcNMEA);
	printf("LocalTime (UTC+2): %02i:%02i:%02i\n\r", time.timeHH, time.timeMM, time.timeSS);
	
	/* cast string to float and convert coordinates from degree minutes to degree decimal */
	latitude  = NMEA_conCoordinates(strtof(nmea.latNMEA, &end));
	longitude = NMEA_conCoordinates(strtof(nmea.lonNMEA, &end));
	
	/* print latitude and longitude as float */
	printf("Latitude  (float): %f %c\n\r", latitude, nmea.lapNMEA);
	printf("Longitude (float): %f %c\n\r", longitude, nmea.lopNMEA);
	printf("\n\r");
	
	/* print the nmea protocol variables to display */
	NMEA_dspString(latitude, longitude);
	
	/* find and print spar branch */
	NMEA_spaBranch(latitude, longitude);
}

/* convert coordinates from degree minutes to degree decimal */
float NMEA_conCoordinates(float coordinates)
{
	int b;
	float c;
	
	b = coordinates/100.0f;									// 51 degrees
	c = (coordinates/100.0f - b)*100.0f ;		//(51.536605 - 51)* 100 = 53.6605
	c /= 60.0f;															// 53.6605 / 60 = 0.8943417
	c += b;																	// 0.8943417 + 51 = 51.8943417
	
	return c;
}

/* find the nearest spar branch */
void NMEA_spaBranch(float latitude, float longitude)
{
	unsigned short int i = 0;
	char Address[45];
	float distance = 0.00f;
	float d = 1000000.00f;
	float sparlat = 0.00f;
	float sparlon = 0.00f;
	
	while(strcmp(waypoints[i].Address, "END") != 0)
	{
		distance = NMEA_calDistance(latitude, longitude, &(waypoints[i]));
		
		if(distance < d)
		{
			d = distance;
			strcpy(Address, waypoints[i].Address);
			sparlat = waypoints[i].Lat;
			sparlon = waypoints[i].Lon;
		}
		i++;
	}
	
	/* print address of nearest spar branch */
	printf("Nearest Spar Branch\n\r");
	printf("*********************************************\n\r");
	printf("* %s\n\r", Address);
	
	/* print address of nearest spar branch to display */
	usartFlag = false;
	printf("* Nearest Spar Branch\n");
	printf("* %s\n", Address);
	usartFlag = true;
	
	/* print distance and direction of nearest spar branch */
	NMEA_prtDistance(d, sparlat, sparlon, latitude, longitude);
	printf("*********************************************\n\r");
}

/* calculate distance between local and spar branch coordinates (point2) */
float NMEA_calDistance(float latitude, float longitude, const PT_T * p2)
{	
  return acos(sin(latitude*PI/180)*sin(p2->Lat*PI/180) + 
					cos(latitude*PI/180)*cos(p2->Lat*PI/180)*
							cos(p2->Lon*PI/180 - longitude*PI/180)) * 6371*1000;
}

/* display distance in Kilometers if distance is higher than 1000 Meters */
/* else: display distance in Meters */
/* + print direction */
void NMEA_prtDistance(float d, float sparlat, float sparlon, float latitude, float longitude)
{
	int duration;
	
	/* print distance and direction first to usart terminal, second to display */
	for(duration = 0; duration <= 1; duration++)
	{
		if(usartFlag == true) { printf("* Distance :   "); }
		else { printf("* Distance : "); }
		
		if(d > 1000.0f) { printf("%.2f km", d/1000); }
		else { printf("%.2f m", d); }
		
		printf("\n\r");
		
		if(usartFlag == true) { printf("* Direction:   "); }
		else { printf("* Direction: "); }
		
		if(sparlat < latitude) { printf("South - "); }
		else { printf("North - "); }
		
		if(sparlon < longitude) { printf("West"); }
		else { printf("East"); }
		printf("\n\r");
		
		usartFlag = false;
	}
	usartFlag = true;
}

/* fractionize the nmea time and set the local time (utc+2) */
void NMEA_timString(char *timeData)
{
	char tmpTime[6];
	
	/* hour */
	strncpy(tmpTime, &timeData[0], 2);
	time.timeHH = (atoi(tmpTime));
	
	/* set hour to utc + 2 */
	if(time.timeHH == 22)			{ time.timeHH = 0; }
	else if(time.timeHH == 23) { time.timeHH = 1; }
	else { time.timeHH = time.timeHH + 2; }
	
	/* minute */
	strncpy(tmpTime, &timeData[2], 2);
	time.timeMM = (atoi(tmpTime));
	
	/* second */
	strncpy(tmpTime, &timeData[4], 2);
	time.timeSS = (atoi(tmpTime));
}

/* print the nmea protocol to display */
void NMEA_dspString(float latitude, float longitude)
{
	font_fg = COLOR_GRAY;
	
	/* print the nmea protocol variables to display */
	usartFlag = false;
	printf("Sentence ID (GPS): %c%c%c%c%c\n\r", nmea.prcNMEA[0], nmea.prcNMEA[1], nmea.prcNMEA[2], nmea.prcNMEA[3], nmea.prcNMEA[4]);
	printf("LocalTime (UTC+2): %02i:%02i:%02i\n", time.timeHH, time.timeMM, time.timeSS);
	printf("Latitude  (float): %f %c\n", latitude, nmea.lapNMEA);
	printf("Longitude (float): %f %c\n", longitude, nmea.lopNMEA);;
	printf("Quality     (SAT): %02i\n", nmea.gpsNMEA);
	printf("Satellites (VIEW): %02i\n", nmea.satNMEA);
	printf("\n");
	usartFlag = true;
	
	font_fg = COLOR_WHITE;
}
