# GPS-Distance-Calculator-for-STM32F411

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
**Microcontroller:** STM32F411 RE  
**GPS-Module:** All GPS Modules which outputs location information as NMEA  
**Connection GPS-Module - STM32:** 4 jumper cable female-female  
**Connection GPS-Module - computer:** Mini-USB to USB cable  

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
