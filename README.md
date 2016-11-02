# GPS-Distance-Calculator-for-STM32F411

##**Description**  
The task consists in implementing a program, that is able to determine the location of the device, itself,  
by the use of a GPS Module. Furthermore, it is able to ascertain the distance and the direction to the closest  
food chain "Spar". It shall comprise the coordinates of food chain "Spar" as much as possible and compare these.  

The implementation on the STM32F411 shall be extended, such that it can ascertain its own location with the coordinates of all stored branches. Also, it have to be possible to calculate the distance (the coordinates of its own location with the specified) and after that it shows which branch is the closest.  

Additionally, there is a function implemented which displays (in words, not graphically) the cardinal direction to the next food chain (the direction indication) plus the declaration how far it is to go there.

##**Project structure** 
**Main**  
Contains merely the main function which calls all configuration functions of the peripheral entities.  
These include the microcontroller (**mcu**), the display (**dsp**) and the global positioning system (**gps**).

**Source**  
Exposes the device-specific function of the microcontroller (**STM32F411RE**).  
The standard peripheral libraries were integrated into the project folder and can be conducted location-independent  
on the current computer system.

**Instruction**  
Subdivides the executable instructions to the particular peripheral units and structures the function to the microcontroller units (**mcu**), the display (**dsp**) and the global positioning system (**gps**). Starting with the configuration (**ClassConfig**) which declares, initializes and enables the respective components, it follows the chronological constituent part of each function.

##**Software components**   
**uVision:** V5.16a    
**Toolchain:** MDK-Lite Version: 5.16a  
**C Compiler:** Armcc.exe V5.06  
**Assembler:** Armasm.exe V5.06  
**Linker:** ArmLink.exe V5.06  
**Putty:** Release 0.64  

##**Hardware components**  
**Microcontroller:** STM32F411RE  
**TFT Touchscreen\*\:** Adafruit 2.8" (ILI9341, STMP610 not implemented)  
**GPS-Module:** All GPS Modules which outputs location information as NMEA  
**Connection GPS-Module - STM32:** 4 jumper cable female-female  
**Connection GPS-Module - computer:** Mini-USB to USB cable  

\* *optional hardware components*  

##**Pin assignment**  
**Onboard LED**  
GPIO Pin: PA5 (Green LED)

**Control output on the USART Terminal**  
Peripheral: USART2  
GPIO Pin: PA2 (Transmit Data)  
GPIO Pin: PA3 (Receive Data)

**Receive Data for the GPS-Device**  
Peripheral: USART6  
GPIO Pin: PA11 (Transmit Data)  
GPIO Pin: PA12 (Receive Data)

**Textual output on the display**  
Peripheral: SPI1  
GPIO Pin: PB6 (Chip select for SPI)  
GPIO Pin: PC7 (Data and Command)  
GPIO Pin: PB3 (Backlight)  
GPIO Pin: PD12 (Reset LCD)  

##**USART interfaces**  
######*Universal Synchronous Asynchronous Receiver Transmitter*  
For the data provided by the GPS device, the USART6 was used. Since the GPS device sends the data via NMEA protocol  
at a baud rate of 19200, the USART6 has been configured accordingly. The pins used for USART6 are PA11 and PA12.  

For the control output of NMEA data and the closest branch of the company "Spar" (calculated distance and direction)  
the USART2 was configured. By use of USART2 the output is sent, via the ST-Link, to the used/connected computer  
and read via Putty. The used baud rate is 19200.

##**Terminal (control output)**  
The first six lines of the control output show different data of the delivered NMEA protocol, including the Sentence (ID),  
the current time (UTC), latitude and longitude (Latitude and Longitude) in "Degree.Minutes" format, quality of the  
received data, and the number of connected satellites.  

In the next part, the current Austrian time (UTC+2) is shown in format "hours, minutes and seconds" with two digits.  
Furthermore, latitude and longitude will be shown in the format "Signed degrees format (DDDD.dddd)".  

The last section of the control output is devoted to the output of the nearest branch of the supermarket chain "Spar".  
For this purpose, the address of the branch, the distance and the direction is printed. If the distance is less than 1000  
meters, it will be displayed in meters, otherwise in kilometers.  

######*Project image: USART Terminal (control output)*  
![alt text][project image]
[project image]: https://github.com/SkyDevice/GPS-Distance-Calculator-for-STM32F411/blob/master/Documentation/terminal_output_min.png "USART Terminal (control output)"

##**References**  
STMicroelectronics (2016): Product description, [STM32F411]  
National Marine Electronics Association (2016): NMEA data set, [GGA record]  
Adafruit TFT Touchscreen (2016): Product description, [Touch Shield]  

[STM32F411]: http://www.st.com/en/microcontrollers/stm32f411re.html
[GGA record]: http://www.nmea.de/nmea0183datensaetze.html#gga
[Touch Shield]: https://learn.adafruit.com/adafruit-2-8-tft-touch-shield-v2
