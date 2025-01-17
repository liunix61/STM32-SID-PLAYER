// Choose your board and upload method from menu

// it's strongly recommended to set optimatization on FASTEST -O3 (from tool menu of Arduino IDE).

//                                                  STM32-SID-Player:     https://github.com/Bakisha/STM32-SID-PLAYER
//                                                  HVSC database:        https://www.hvsc.c64.org/downloads (download and unpack to SD Card)


/////////////////////////////////////////////////////////////////////////////////////////////
//
//           Player configuration:
//
//////////////////////////////////////////////////////////////////////////////////////////////

#define TUNE_PLAY_TIME 215                               // Can't implement songlenghts, manual values are needed (in seconds)//  TODO: try to determine silence in output, and skip to next tune

const char * const HVSC = "C64Music" ;                   // ---> IMPORTANT! <---  name of HVSC SID Collection folder on your SD Card ("HVSC", "C64Music", "/" or "" for root, etc"

//#include "01_HVSC.h"                                   // uncomment this line to load whole HVSC folder list from "01_HVSC.h". If disabled, it will load folder list below (disabled for BluePill, can be enabled, but must compile with O0 optimatization)

#ifndef NUMBER_OF_FOLDERS
#define NUMBER_OF_FOLDERS 38                             // set number of folder for playlist. Must have at least 1.
const char * const FOLDER_PLAYLIST                       //  set favorite directories paths (relative to main HVSC folder) with sid files in it
[NUMBER_OF_FOLDERS ] =
{
  "/",
  "CUSTOM/",
  "FAVORITES/",
  "MUSICIANS/G/Gregfeel/",
  "MUSICIANS/B/Bayliss_Richard/",
  "MUSICIANS/G/Gas_On/",
  "FAVORITES/",  // custom folder
  "MUSICIANS/R/Rowlands_Steve/",
  "DEMOS/A-F/",
  "MUSICIANS/D/DRAX/",
  "MUSICIANS/B/Blues_Muz/Gallefoss_Glenn/",
  "MUSICIANS/L/Lft/",
  "MUSICIANS/L/LMan/",
  "MUSICIANS/L/Linus/",
  "MUSICIANS/P/Page_Jason/", // lot of multi-speed tunes (CIA speed test)
  "MUSICIANS/D/Dunn_David/",
  "MUSICIANS/T/Tel_Jeroen/",
  "MUSICIANS/V/Vincenzo/",
  "MUSICIANS/D/Daglish_Ben/",
  "MUSICIANS/G/Gray_Matt/",
  "MUSICIANS/B/Blues_Muz/" ,
  "MUSICIANS/S/Sequencer/",
  "DEMOS/G-L/",
  "MUSICIANS/G/Gray_Matt/",
  "MUSICIANS/F/Fanta/",
  "MUSICIANS/T/The_Syndrom/",
  "MUSICIANS/L/Laxity/",
  "MUSICIANS/K/KB/",
  "MUSICIANS/H/Hesford_Paul/",
  "MUSICIANS/G/Goto80/",
  "MUSICIANS/M/Mahoney/",
  "MUSICIANS/H/Hubbard_Rob/",
  "MUSICIANS/B/Blues_Muz/",
  "MUSICIANS/P/PVCF/",
  "MUSICIANS/J/JCH/",
  "MUSICIANS/R/Rowlands_Steve/",
  "MUSICIANS/H/Huelsbeck_Chris/",
  "MUSICIANS/G/Galway_Martin/"

};
#endif


////////////////////////////////////////////////////////////////////////////////////////////
//
//          hardware settings
//
//////////////////////////////////////////////////////////////////////////////////////////////

#define AUDIO_OUT       PA8                 // can't be changed, this is just reminder 

#define USE_SERIAL                          // for debugging info on Serial (usually USB Serial), uncomment if it's needed
//#define USE_SERIAL1                       // for debugging info on Serial1 (usually on PA9/PA10), uncomment if it's needed
#define SERIAL_SPEED    9600                // Speed of serial connection

#define CS_SDCARD       PA1                 // can be changed
#define SD_SPEED        20                  // Maximum SD Card SPI speed in MHz 
///                                         ! Speed matters ! Loading between folders is around 1second per 50 files in a folder on 20MHz, and around 3 seconds per 50 files in a folder for 1 MHz

//#define SD_CLK          PA5               // it must be hardware SPI1 port pins (look at your microcontroller pinout)
//#define SD_MISO         PA6
//#define SD_MOSI         PA7

#define irPIN           PB14                          // infraRED pin from sensor. Can be any pin

// Acquire codes by running a sketch from https://github.com/Bakisha/irNEC_STM32_TIM2_Callback
// raw values (in HEX) from "irData" variable
#define irPLAY          0x22
#define irPREV          0x02
#define irNEXT          0xc2
#define irLOOP          0x98
#define irSCAN          0xb0
#define irZERO          0x68
#define irONE           0x30
#define irTWO           0x18
#define irTHREE         0x7a
#define irFOUR          0x10
#define irFIVE          0x38
#define irSIX           0x5a
#define irSEVEN         0x42
#define irEIGHT         0x4a
#define irNINE          0x52



////////////////////////////////////////////////////////////////////////////////////////////
//
//          optional emulator settings
//
//////////////////////////////////////////////////////////////////////////////////////////////

#define USE_FILTERS                        // uncomment for testing, irq is  faster in calculations (so multiplier will be smaller, and samplerate will be higher)

#define FILTER_SENSITIVITY  4              // Lower is better. Best is 1.  It will consume irq time (smaller samplerate) , but FILTER_FREQUENCY can be set much higher
#define FILTER_FREQUENCY 12500              // You'll know it if it's very high (it will totaly ruin sound)

//                                         Play with these two values to determine what is "SID-like" sound
//                                         NOTE: Filters are linear in all frequencies range. Don't ask for true filters emulation, i'm suprised i managed to emulate it at all :-)
//                                         NOTE2: lower FILTER_FREQUENCY is similar to 6581 SID chip, higher value (12500 and above) is more like 8581 SID chip.
//                                         .--------------------------------------------.
//                                         |                      |  maximum            |
//                                         |  FILTER_SENSITIVITY  |  FILTER_FREQUENCY   |
//                                         ----------------------------------------------
//                                         |            1         |      18000          |
//                                         ----------------------------------------------
//                                         |            2         |      14000          |
//                                         ----------------------------------------------
//                                         |            4         |      12500          |
//                                         ----------------------------------------------
//                                         |            8         |       8000          |
//                                         ----------------------------------------------
//                                         |            16        |       4000          |
//                                         .--------------------------------------------.


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Don't change stuff bellow
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(USE_HAL_DRIVER)
#define USE_STM32duino_CORE
//                                        Official ST cores. Support for multiple line of MPU
//                                        link: https://github.com/stm32duino/Arduino_Core_STM32
//                                        Set in preferences: https://github.com/stm32duino/BoardManagerFiles/raw/master/package_stmicroelectronics_index.json and search stm32 in board manager.
//                                        Choose stm32 cores by ST Microelectronics. Select your CPU from tools menu


#elif defined(__STM32F1__)
#define USE_ROGER_CORE
//                                        Most of stuff for blue pill is made for this core
//                                        link: https://github.com/rogerclarkmelbourne/Arduino_STM32
//                                        Set in preferences: http://dan.drown.org/stm32duino/package_STM32duino_index.json and search stm32F1 in board manager.
//                                        Choose STM32F1xx core (NOTE: Only STM32F1 works)
#elif defined(AVR)
//                                        Must test this some day with Arduino MEGA
//
#error "Unsupported core - will try someday when i learn to setup interrupts and pwm on pins, without need to memorize TTROA, DDROB or DDWhateva"
#else
#error "Unknown or unsupported core. Maybe even both"
#endif

#include <SPI.h>
#include "SDCONFIG.h"                                    // local settings for SdFat library (same as file "SdFatConfig.h" copied from SdFat library folder)
#include <SdFat.h>                                       // install from library manager or from:  https://github.com/greiman/SdFat
// ---> IMPORTANT! <--- Try 1.1.4 version of library if you are having trouble compiling.


#include "xx_RAM.h"
#include "xx_SID.h"
#include "xx_6502.h"


/*


  SID chip (6581 and 8580) and 6502 CPU emulator, with audio output on pin PA8, for STM32 line of microcontrollers, compiled with Arduino IDE, uploaded with ST-LINK V2.

  My personal project that i'm still having fun with . Mainly done for STM32F103C8 (Blue Pill). Should work with other STM32 microcontrollers that Arduino IDE support. Tested also at STM32F401CCU6, STM32F411CEU6 and STM32F407VET6 dev boards (and as i see, they all have same pin (PA8) for Timer1,channel1) and same pins for SPI.

  -Only PSID V2 sids, no digis, emulator is not fast enough.

  -Maximum size of sid is limited by microcontrollers RAM

  -Sid load address must be at $0400 or greater.

  -Filter emulation is far from real SID chip, but, for me, it sound enough "SID-ish".

  -Plays sid from hardcoded folders from SD CARD. Use buttons or infrared remote control (NEC protocol) to switch to next tune/file/folder.

  -SID tune database can be found at  https://www.hvsc.c64.org/

  I'm not good at C, so code is maybe a mess, but it works :-) Code used for emulator is found at http://rubbermallet.org/fake6502.c

  reSID can be found at https://en.wikipedia.org/wiki/ReSID. This is not reSID port, but i did used some parts of reSID code.


  SCHEMATICS (not to scale) :

  STM32F103C8/B - STM32F401CC - STM32F411CE :

  .------------------------------------------------------.                                              +----------+
  |                                                      |--------< 3.3V  >-----------------------------| infrared |
  | STM32FxxxXXxx                                        |--------< irPIN >-----------------------------| receiver |
  .-----------------------|------|------|----|-----------.--------< GND   >-----------------------------|          |
  |G P   P P P                              P|                                                          +----------+
  |N A   A A A                              A|
  |D 1   5 6 7                              8.------|R1|---+---|C2|----------|
  |  |   | | -- SD_MOSI                                    |                 --
  |  |   | ---- SD_MISO                                    C                 || P1
  |  |   ------ SD_CLK                                     1                 ||<----------------| OUDIO OUT
  |  ---------- CS_SDARD                                   |                 --
  .--------------------------------------------------------+-----------------|------------------| GND
                                       GND


  STM32F407VET6 black board   :

  .------------------------------------------------------.                                              +----------+
  |                                                      |--------< 3.3V  >-----------------------------| infrared |
  | STM32F407VE                                          |--------< irPIN >-----------------------------| receiver |
  .-----------------------|------|------|----|-----------.--------< GND   >-----------------------------|          |
  |G P   P P P                              P|                                                          +----------+
  |N B   B B B                              A|
  |D 7   3 4 5                              8.------|R1|---+---|C2|----------|
  |  |   | | -- SD_MOSI                                    |                 --
  |  |   | ---- SD_MISO                                    C                 || P1
  |  |   ------ SD_CLK                                     1                 ||<----------------| OUDIO OUT
  |  ---------- CS_SDARD                                   |                 --
  .--------------------------------------------------------+-----------------|------------------| GND
  R1 = 100-500 Ohm
  C1 = 10-100 nF
  C2 = 10 uF
  P1 = 10KOhm potentiometer




  HAVE FUN :-)

*/
