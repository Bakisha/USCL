
#include<USCL.h>

// hardware setup:

#define Xsize                      4                       // X size of cube (width)
#define Ysize                      4                       // Y size of cube (depth)
#define Zsize                      4                       // Z size of cube (height)
#define CUBE_MODE                  RGB_CUBE                // color cube
//#define CUBE_MODE                 LED_CUBE               // single color cube
#define pinOE                      PA0                     // Latch Enable pin on 595
#define pinLE                      PA1                     // Output Enable pin on 595
#define FramesPerSecond            60                      // 60 frames per second
#define modulationBitDepth         1                       // 4 = 16 shades per color
//#define SPIspeed                   SPI_speed_16            // Speed of SPI protocol (CPU in MHz )/ 16
#define SPIspeed                   1000000                 // also, it can be set as maximum SPI speed, it will be set to first lower number suported by CPU SPI hardware. Limit speed of 595 chips and lenght of traces/wires


float animtime = 180000;                                    // global duration of animations


USCL cube(Xsize, Ysize, Zsize, CUBE_MODE , pinOE, pinLE,  FramesPerSecond , modulationBitDepth, SPIspeed);   // look for INFO for list of supported microcontrollers


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//      INFO:
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  TESTED HARDWARE
//  595             = BLUEPILL  (ROGER's CORE) / STM32FXXX (STM32DUINO CORE)               = ARDUINO  UNO,NANO,MICRO*        = Leonardo* (atmega32u4)        = MEGA 2560*                    = WEMOS_D1_mini*
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  OE pin          = PA0  // user defined                                                 = D10  // user defined            = A1  // User defined           = 49  // User defined           = D1 - user defined
//  Latch pin       = PA1  // user defined                                                 = D9   // user defined            = A0  // User defined           = 47  // User defined           = D2 - user defined
//  DataIN  (MOSI)  = PA7  // bluepill SPI1 MOSI                                           = D11 (MOSI)                      = 16 MOSI                       = 51 MOSI                       = D7 (MOSI)
//  SRCLK   (SCK)   = PA5  // bluepill SPI1 CLK                                            = D13 (SCK)                       = 15 SCK                        = 52 SCK                        = D5 (CLK)
//
// * - under testing, cube with this CPU still not finished/tested

// Should work on following arduino family MPUs (Not all tested):
//  - Arduino uno
//  - Arduino Diecimila
//  - Arduino Duemilanove with ATmega328
//  - Arduino Duemilanove with ATmega168
//  - Arduino Nano with ATmega328
//  - Arduino Nano with ATmega168
//  - Arduino Mega ADK
//  - Arduino Mega with ATmega2560
//  - Arduino Mega with Atmega1280
//  - Arduino Leonardo
//  - Arduino Mini with ATmega328
//  - Arduino Mini with ATmega168
//  - Arduino LilyPad with ATmega328
// - Arduino LilyPad with ATmega168
//  - Arduino Pro (mini) with ATmega328
//  - Arduino Pro (mini) with ATmega168
//  - Maniacbug arduino with ATmega1284p
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



// wemos D1 mini
// Add this adress to your board manager, search: wemos
// http://arduino.esp8266.com/stable/package_esp8266com_index.json
// @60 fps ( minimum without flicker )
// 4x4x4 RGB - max MODULATION_BIT_DEPTH = 5/7 (recommended = 4/6 ) (80/160MHz)
// 8x8x8 RGB - max MODULATION_BIT_DEPTH = 3/4  (recommended = 2/3 ) (80/160MHz)
// 4x4x4 LED - max MODULATION_BIT_DEPTH = 7/8  (recommended = 5/6 ) (80/160MHz)
// 8x8x8 LED - max MODULATION_BIT_DEPTH = 3/4  (recommended = 2/3 ) (80/160MHz)
// SPI_speed - SPI_speed_2 is max (8MHz). ESP8266 have set clock divider in wierd way. It behave like Arduino UNO, aka, like main CPU clock is 16Mhz, so clock divider 2 is 8Mh SPI clock, 4 is 4Mhz etc... 
// SPI_speed = 20000000 (testing speed) 
// NOTE: HUGE storage space for placing anymations, but low BAM bit depth due to high SPI overhead between sended bytes. Maybe can be combined with Wifi stuff.
// NOTE 2: Still under testing. Not really tried on real cube. I was just looking timing on OE/LE/MOSI/CLK pins, and it looks OK.
// NOTE 3: It have 1.7uS overhead between bytes, so it is more suited for smaller or single color cubes.
// NOTE 4: It is power hungry. Need 1A power supply on USB, or blue led won't have enough voltage to turn on. Need 200 fps to remove flickers, or large capacitor on led voltage rail.
//USCL cube(4, 4, 4, RGB_CUBE , D1, D2,  200 , 3, 5000000); // uncomment this if you are building for Wemos D1 mini


// Arduino (UNO, NANO, MINI)
// @60 fps , SPI_speed_2 ( minimum without flicker )
// 4x4x4 RGB - max MODULATION_BIT_DEPTH = 6 (recommended = 4 )
// 8x8x8 RGB - max MODULATION_BIT_DEPTH = 3 (recommended = 3 or 1 )
// 4x4x4 LED - max MODULATION_BIT_DEPTH = 7 (recommended = 5 )
// 8x8x8 LED - max MODULATION_BIT_DEPTH = 5 (recommended = 4 )
// SPI_speed - SPI_speed_2 is max (8MHz). Lower (SPI_speed_4 or SPI_speed_8) only if board is large with long wires, but MODULATION_BIT_DEPTH must decrease if it doesn't work
// NOTE: LOW flash memory for all animations. Make your pick
// NOTE 2: LOW RAM !MODULATION_BIT_DEPTH is max 3 if any animation use "line" function, 4 if it doesn't use it
// NOTE 3: 8x8x8 RGB, MODULATION_BIT_DEPTH 2 - doesn't work LOL dunno why, maybe low RAM? Tested only on microcontroler, not on cube.
// USCL cube(8, 8, 8, RGB_CUBE , 10, 9,  60 , 4, SPI_speed_2); // uncomment this if you are building for Arduino family
// USCL cube(4, 4, 4, RGB_CUBE , 10, 9,  60 , 4, SPI_speed_2); // uncomment this if you are building for Arduino family

// Arduino Mega2560
// @60 fps , SPI_speed_2 ( minimum without flicker )
// 4x4x4 RGB - max MODULATION_BIT_DEPTH = 5  (recommended = 4 )
// 8x8x8 RGB - max MODULATION_BIT_DEPTH = 4 (recommended = 2 )
// 4x4x4 LED - max MODULATION_BIT_DEPTH = 6 (recommended = 5 )
// 8x8x8 LED - max MODULATION_BIT_DEPTH = 5 (recommended = 3 )
// SPI_speed - SPI_speed - SPI_speed_2 is max (8MHz). Lower (SPI_speed_4 or SPI_speed_8) only if board is large with long wires, but MODULATION_BIT_DEPTH must decrease if it doesn't work
// NOTE: Tested only timing signals on microcontroller, not on actual cube
// USCL cube(8, 8, 8, LED_CUBE , 49, 47,  60 , 3, SPI_speed_2); // Arduino Mega2560

// Arduino Leonardo (atmega32u4)
// @60 fps , SPI_speed_2 ( minimum without flicker )
// 4x4x4 RGB - max MODULATION_BIT_DEPTH =  6 (recommended = 4  )
// 8x8x8 RGB - max MODULATION_BIT_DEPTH =  3 (recommended = 2  )
// 4x4x4 LED - max MODULATION_BIT_DEPTH =  7 (recommended =  4 )
// 8x8x8 LED - max MODULATION_BIT_DEPTH =  4 (recommended = 3  )
// SPI_speed - SPI_speed - SPI_speed_2 is max (8MHz). Lower (SPI_speed_4 or SPI_speed_8) only if board is large with long wires, but MODULATION_BIT_DEPTH must decrease if it doesn't work
// NOTE: LOW flash memory for all animations. Make your pick
// NOTE2: This one was PITA to upload sketch
// NOTE3: Tested only timing signals on microcontroller, not on actual cube
// USCL cube(4, 4, 4, RGB_CUBE , A1, A0,  60 , 4, SPI_speed_2); // atmega32u4


// Just a reminder:
// USB2Serial                   BLUEPILL
// 3.3V                         3.3V
//  TX                          RX ( A10 )
//  RX                          TX ( A9  )
//  GND                         GND
//
//
// Or use STM32duino bootloader for direct upload (once flashed, on my bluepill, USB upload work even with 10k R10 resistor on my motherboard (Win7 x64) ) With 330Ohm resistors for GB and 470Ohm for R, cube can be powered directly from USB
//
// STM32F103 only (Bluepill, Blackpill) ( ROGER's CORE )
// Add this adress to your board manager, search: stm32duino
// http://dan.drown.org/stm32duino/package_STM32duino_index.json
// @60 fps , SPI_speed_8  ( minimum without flicker )
// 4x4x4 RGB - max MODULATION_BIT_DEPTH = 7 (recommended = 5 )
// 8x8x8 RGB - max MODULATION_BIT_DEPTH = 5 (recommended = 4 )
// 4x4x4 LED - max MODULATION_BIT_DEPTH = 8 (recommended = 6 )
// 8x8x8 LED - max MODULATION_BIT_DEPTH = 6 (recommended = 4 )
// SPI_speed - SPI_speed_4 (18MHz). Lowest (SPI_speed_64) (1.2MHz) should be enough even for breadboard connectons
// NOTE: My Favorite :-)
// NOTE 2:
// NOTE 3:
//USCL cube(8, 8, 8, RGB_CUBE , PA0, PA1,  60 , 4, SPI_speed_4); // My DIY prototype - Dec 2018
//USCL cube(4, 4, 4, RGB_CUBE , PA0, PA1,  60 , 7, SPI_speed_2); // My DIY prototype - April 2019
//USCL cube(4, 4, 4, RGB_CUBE , PA0, PA1,  60 , 6, SPI_speed_16); // My SEEED cube V1


// STM32FXXX ( STM32DUINO's CORE )
//  Official ST cores. Support for multiple line of MPU
//  link: https://github.com/stm32duino/Arduino_Core_STM32
//  Set in preferences: https://github.com/stm32duino/BoardManagerFiles/raw/master/STM32/package_stm_index.json and search stm32 in board
// @60 fps , SPI_speed_8  ( minimum without flicker )
// SPI_speed - SPI_speed_4 (18MHz). Lowest (SPI_speed_64) (1.2MHz) should be enough even for breadboard connectons
// NOTE: Microcontroller tested:
//                                STM32F103C8/B
// 4x4x4 RGB - max MODULATION_BIT_DEPTH = 7 (recommended = 5 )
// 8x8x8 RGB - max MODULATION_BIT_DEPTH = 5 (recommended = 4 )
// 4x4x4 LED - max MODULATION_BIT_DEPTH = 8 (recommended = 6 )
// 8x8x8 LED - max MODULATION_BIT_DEPTH = 6 (recommended = 4 )

//                                STM32F401CC
// 4x4x4 RGB - max MODULATION_BIT_DEPTH = 7 (recommended = 5 )
// 8x8x8 RGB - max MODULATION_BIT_DEPTH = 5 (recommended = 4 )
// 4x4x4 LED - max MODULATION_BIT_DEPTH = 8 (recommended = 6 )
// 8x8x8 LED - max MODULATION_BIT_DEPTH = 6 (recommended = 4 )

//                                STM32F411CE
// 4x4x4 RGB - max MODULATION_BIT_DEPTH = 7 (recommended = 5 )
// 8x8x8 RGB - max MODULATION_BIT_DEPTH = 5 (recommended = 4 )
// 4x4x4 LED - max MODULATION_BIT_DEPTH = 8 (recommended = 6 )
// 8x8x8 LED - max MODULATION_BIT_DEPTH = 6 (recommended = 4 )

// NOTE 2: Still under testing. Not really tried on real cube. I was just looking timing on OE/LE/MOSI/CLK pins, and it looks OK.
// NOTE 3:
//USCL cube(8, 8, 8, RGB_CUBE , PA0, PA1,  60 , 4, SPI_speed_4); // My DIY prototype - Dec 2018
//USCL cube(4, 4, 4, RGB_CUBE , PA0, PA1,  60 , 7, SPI_speed_2); // My DIY prototype - April 2019
//USCL cube(4, 4, 4, RGB_CUBE , PA0, PA1,  60 , 6, SPI_speed_16); // My SEEED cube V1




//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//      My way of trying to deal with Arduino IDE
//
//      Animation list is in zz_SetupLoop.ino tab
//
//      Every animations is in it's own tab
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void DoNotDeleteMe () {}
