# USCL

                                  UNIVERSAL SERIAL CUBE LIBRARY - USCL
                                  
  For  Single color and RGB (Common Anode) colored LED cube, from 1x1x1 to 128x128x128 CUBE sizes
  
   - currently supporting arduino and bluepill boards, other boards are in development -
  
NOTE: This is not about bulding LED RGB/single color CUBE, this library is software side of a hardware used to build cubes.



ABOUT:
I love LED cubes. There are lot of guides how to build them on internet.
But every each one of them comes with software that i can't use. 
Then i found library for single color LED cube that use only 595 chip and few transistors. 
The "Universal SR driven 4x4x4 to 8x8x8 LED cube library" that can be found at

https://forum.arduino.cc/index.php?topic=336469.0

What i liked about this library is it's universality. Code maded for 8x8x8 can be used for 4x4x4 cube.
Then my skill got better and i decided to build RGB cube. I started with similar plan:
Use 74hc595 for transfering all data, ULN2803 for driving common anode RGB LEDs and P-MOSFETS (or UDN2981 for smaller cubes) for driving layers.

First was arduino based 4x4x4. But then i hit a wall. Again i needed a software, now for RGB cube.
With noob skils in programing, i started changing LedCube library.

In a meantime, i finished 8x8x8 RGB cube, but this time i used STM32 microcontroller, the cheap BLUEPILL from ALIEXPRESS.
And, since it's same in design, i rewrited some animation software from a youtuber Kevin Darrah ( https://www.kevindarrah.com/ ), and changed interrupt code from some bluepill tutorial.

Well, after few years, and builded various led/rgb cubes, i finally had enough of reinventing hot water, so i manage to finish a library that can be used in various cube sizes, between 1x1x1 up to 128x128x128, and with different x,y,z sizes. It that can be compiled for both arduino and bluepill (Wemos D1 mini is still under test). Not that all sizes can work any cpu, it's just that library support it. 

Animations are written in single sketch in Arduino IDE, and only 1 line of code is to select how cube is built (see examples). To select cpu, just select proper board in Arduino IDE options. 

HARDWARE:
As for hardware, i was never able to draw schematics, but it should be simple to explain:

MCU sends data via SPI hardware pins, and two other (selectable) pins is used for Output Enable and Latch pin for 74hc595.
 First ones in line 595 is used to control layers, and in combination with ULN2803 is pulling P-mosfet's gate to ground (or pnp array transistors: UDN2981 for smaller cubes). Number of 74hc595/UDN2981 is 1 per 8 layers. If cube is bigger, then first one control layer 1-8, second one control 9-16, etc. 
 Number of other 74hc595 depends on cube size.
 
 Generaly, it need 'Y size' pins , multiplied by 'X size', divided by 8 (rounded up), and multipled by 3 (if using RGB cube)
 
 
Example: 5x13x18 RGB

1st 595 control layers 1-8, 2nd controls layer 9-16, 3rd control layers 17-18

it have 13 'Y size" so 13 pins is used. Totaly, 5x13=50 , divided by 8 and rounded up is 7 595 chips , per color. .
So, in total it would have total of 24 74hc595, of wich 3 ULN2803 for driving 18 P-channel mossfets (with pull-up resistors or resistor array) and 21 ULN2803 for driving led's cathodes. 


Ofcorse, with cube this big, it is wise to put some in-between buffers ( like 74HC245) for faster SPI comunication and not to stress outputs on cpu pins. I generaly put it after every 8 74HC595 chips.

Example 2:
For 4x4x4 RGB cube, it's total of 7 74hc595 (1 for layer + 2 per color ). 6 ULN2803 and 1 UDN2981 (as long as current per layer is less then 500mA, UDN2981 can be used)
For 8x8x8 RGB cube it's total of 25 74hc595, 25 ULN2803, and 8 P-type logic level mosfets (i used IRF9530 for my 8x8x8 RGB Cube with bluepill, and i added 74hc245 buffer between PCB - i build  150x100 single sided PCB for each color and 1 for layer driver and MPU, and stack it all on top of each other. ). 
But, there is so much flexibility, instead of cheap 74hc595/ULN2803 combo, other dedicated LED driver chips can be used . I preffered THT , so i stayed with 595. Only  chip for driving layers should be 595 (or rather, 8bit serial to parallel) chip. 

Important note: On 5x5x5, 6x6x6 and 7x7x7 cube , there is unused bits.
For example 5x5x5 rgb cube, since it have 25 leds per color, it needed 4 74hc595, but in 4th 595, only 1 output is used (as 25th), other outputs are not connected. For next color, next 595 is used (not next avaible pin). So, in total it need 4*3+1, 13 chips

So, for 6x6x6 rgb cube it need 5 (36/8 = 5) 74hc595 per color + 1 for layer. In total 17 chips. 

For 7x7x7 rgb cube it's 7 chips per color (49/8). So 22 in total.

Compared to 25 chips in total for 8x8x8 RGB cube, i preffered that one, even cube support others.
For single color led cubes, LEDs can be driven from 74hc595, and 1 ULN2803 can drive cathodes of layers.

NOTE1: Bluepill use 3.3V and so 74HC494 work also on 3.3V. I didn't bother putting logic levels converters, SPI speed is good enough.

NOTE2: Library us "positive" logic. Meaning, digital 1 on output, means LED (or layer) is ON.

I tried to pass as much variables to library, so it can be as universal as much.
Here is pinouts of boards :

HARDWARE
  595             = BLUEPILL                              = ARDUINO  UNO,NANO,MICRO*        = Leonardo* (atmega32u4)        = MEGA 2560*                    = WEMOS_D1_mini*
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  OE pin          = PA0  // user defined                  = D10  // user defined            = A1  // User defined           = 49  // User defined           = D0  IO - OR D4  IO, 10k Pull-up, BUILTIN_LED - user defined
  Latch           = PA1  // user defined                  = D9   // user defined            = A0  // User defined           = 47  // User defined           = D8  IO, 10k Pull-down, SS    - user defined
  DataIN  (MOSI)  = PA7  // bluepill SPI1 MOSI            = D11 (MOSI)                      = 16 MOSI                       = 51 MOSI                       = D7 (MOSI)
  SRCLK   (SCK)   = PA5  // bluepill SPI1 CLK             = D13 (SCK)                       = 15 SCK                        = 52 SCK                        = D5 (CLK)


 * - under testing, cube with this CPU still not finished/tested



HARDWARE AND SOFTWARE EXPLANED:

Basicly, using interrupts, MPU send data in "B-G-R-layer" format. Every time it sends data, it set OE_pin to HIGH when sending. This is to prevent flickering and keep timing of PWM-ing leds. Then latch new data, then set OE_pin to LOW. It is done at fixed rate:
Lenght in microseconds of interruput = 1000000 / frames per seconds *  pow(2 , MODULATION_BIT_DEPTH) * cubesize. For example, 60 fps, 4bit color depth, 8x8x8 cube is 1000000 / 60*8*16 = 130 uS . MPU need some time to actually send data, and it depends on  MPU and SPI speed. Any time MPU not spending in interrupt routine, is time spending calculating animations. The goal is to find sweet spot so animation can calculate new frame of animation by lowering framerate, lowering bit bam modulation and trying to achive as much SPI speed as possible until number of colors (or shades) is enough for animation to , not just look good, but fast enough too. Unfortunatelly, library is not optimized, as my coding skill is just one level above "hello world" example sketches. Sorry about that. 
All i wanted to focus on creating animation with compatible solution for my cubes (so i don't need to write animation-per-cube code).

HOW TO USE:

I tried to pass as much variables to library, so it can be as universal as much.


      !!!IMPORTANT: !!!!
      
      
     Library use double-buffering!!!       
      
      
It means , library will keep showing front frame buffer, while user draw in back buffer, then use command to transfer back buffer to front buffer.
This gives very nice non-flickering experience. It can be disabled for those who want it.

Since it have so much combination, here is some examples (with measured recommendation, but it all depends on the hardware of your cube):

-STM32F1 (Bluepill, Blackpill)
 Add this adress to your board manager, search: stm32duino
 http://dan.drown.org/stm32duino/package_STM32duino_index.json
 @60 fps , SPI_speed_8  ( minimum without flicker )
 4,4,4,RGB - max MODULATION_BIT_DEPTH = 7 (recommended = 5 )
 8,8,8,RGB - max MODULATION_BIT_DEPTH = 5 (recommended = 4 )
 4,4,4,LED - max MODULATION_BIT_DEPTH = 8 (recommended = 6 )
 8x8x8,LED - max MODULATION_BIT_DEPTH = 6 (recommended = 4 )
 SPI_speed - SPI_speed_4 (18MHz) . Lower (SPI_speed_128) should be enough even for breadboard connectons

EXPLANATION OF VARIABLES AND SYNTAX:

  USCL cube_name( x cubesize, y cubesize, z cubesize, CUBE mode,  OE_pin, LE_pin, FPS, MODULATION_BIT_DEPTH, SPI speed )
  
  -cube name
            can be any name. For example"MyCube", "c", "experiment". Since it will be unique for every sketch, i recommend to pick one and stick with it. You can use "search and replace" function to rename from deafult.
  
  -x cubesize:
             any number between 1 and 128. This the "depth" of a cube.

  -y cubesize:
             any number between 1 and 128. This is the "width" of the cube.             

  -z cubesize:
             any number between 1 and 128. This the layer size of cube.
             
  - cube mode:
            RGB_CUBE  - for rgb cube
            LED_CUBE  - for single color led cube
            
  - OE_pin
            OE pin connected to 74hc595. For Bluepill, original pin names can be used (PA0, PB6 etc).
            
  - LE_pin
            Latch pin connected to 74hc595
            
  - FPS
            Number of frames per second. Minimal is 60, so cube won't flicker. It can go higher,  but keep in mind, there is a limit (because timing is based on interrupts. MPU can crash, or animation can slow down if you go to very high with framerate. Timing in interrupt is based on this value. Generally, if you double the fps, to keep same timings, lower MODULATION_BIT_DEPTH by 1.
            
  - MODULATION_BIT_DEPTH
            A number between 1 and 8.
            This is number as power of 2. Meaning: number 4 means 16 combination of 1 color (but one color is not color, LED is OFF). But that means 16*16*16=4096 unique colors (including black).  Number 2 means 3 shades per color (in total 64 unique colors, including black). Start with 1 or 2, and go higher untill cube start to flicker (and possibly cpu crash).
            
  - SPI speed  
            SPI_speed_2     - MPU speed / 2
            SPI_speed_4	    - MPU speed / 4
            SPI_speed_8	    - MPU speed / 8
            SPI_speed_16    - MPU speed / 16	
            SPI_speed_32	  - MPU speed / 32
            SPI_speed_64    - MPU speed / 64
            SPI_speed_128   - MPU speed / 128
            
  Use this to set SPI speed for your cube. It CPU MHz / number. Meaning,  for Bluepill, maximum speed of 74HC595 at 3.3V is experimental. Since it's 76MHz MPU, experiment with low values first (SPI_speed_64= 76/64), around 1.18MHZ should be enough even for cube with chips on breadboard. I use 19MHz for my 4x4x4 RGB cube, but for 8x8x8 RGB cude fastest speed i manage to do is 9.5Mhz. You can tweak values in USCL.cpp and add delays in microseconds when latching data for maximum perfomance. You'll know it's too fast SPI speed when leds connected to last 74hc595 are flickering or not working at all). At vey high speed , only red color will work.
            
ANIMATIONS:

These two line must be included in code:
#include<SPI.h>
#include<USCL.h>


Then followed by:

 USCL cube_name(x cubesize, y cubesize, z cubesize, CUBE mode,  OE_pin, LE_pin, FPS, MODULATION_BIT_DEPTH, SPI speed);

You can write animations in main loop, but i recommend you create functions and pass paramaters. So it can be nice list of animation names (or write extra code to random select animation). I didn't bother to write animations as library. Honestly, i don't know how to write animations as library :-) Deal with it.

Here is syntax when passing a function:

void name_of_animation(USCL & cube_name, any paramater 1, any paramater 2,any paramater 3)
{
code here
}
 Must be called once (in setup):

  cube_name.begin();         
        Use this in your setup code. 
        name "cube_name" can be any but it must match when library is initiated   
        
 
Use in the main loop:
 
 void loop() {
 name_of_animation(cube_name,any paramater 1, any paramater 2,any paramater 3);
}

Don't bother writing setup and loop voids before animations voids. It doesn't work. I tried.


variables that can be read from library:

  int cubesize = cube.getCubeSizeX(); - integers to get size of cube. Use it to scale it, so animations can be shared with other cube-sizes. Value will between 1 and 128
  int cubesize = cube.getCubeSizeY();  
  int cubesize = cube.getCubeSizeZ();  
  
  int max_brightness = cube.getMaxBrightness(); - maximum brightness of one color (zero-indexed), and it is variable number, depending on MODULATION_BIT_DEPTH. It is power of two of MODULATION_BIT_DEPTH.  For example: it's 1 for 1bit BAM, 3 for 2bit BAM, 7 for 3bit BAM, 15 for 4bit BAM, 31 for 5bit BAM, etc...
  
  int FPS = cube.getFPS();  - read frames per seconds. Use it for timing animation for cubes with different fps. 
  
  

  
Commands for control of cube animations (inside a animation subroutine):

-cube_name.setAutoClearBackBuffer(false/true); 

 every time next frame is draw, back buffer will be cleared (true). You must re-draw every frame. Or drawed LEDs will remain the same (false). You must draw it again with brighness 0 to turn it off

-cube.copyCurrentFrame();

copy frame that is showing currently in front buffer to back buffer so stuff can be added/deleted 
              
              
List of commands for drawing (note: it not imiadelly shown, it is drawn in back buffer)

-    cube_name.HSV(z, x, y,  hue, S,  V);
      Draw at z,x,y in HSV format
       integer hue - value between 0 and 359 (color wheel pallete of colors)
       float S - saturation (white) of a color. 0.0 is pure white, 1.0 without saturation
       float V - "value" of HSV color space. Decrease to black (0.0) or full brighness (1.0) of a color
       
-    cube_name.RGB(z, x, y,  red,  green ,  blue);
      Draw at z,x,y in RGB format
        red, green, blue - number for brightness (depending of max_brightness variable)
        
-    cube_name.LED(z, x, y,  brightness); 
        Set all colors at same brightness (white) (this is more for single-color cubes, since single color cubes averages intesity of brightness when using colored animations)
        
-    cube_name.RED(z, x, y,  brightness); 
      Set only red leds , brightness is determined by color bit depth (depending of max_brightness variable). It can be used for single color leds, since it is only "color" that is send when using single-color cubes
-    cube_name.GREEN(z, x, y,  brightness);
      Set only green leds , brightness is determined by color bit depth (depending of max_brightness variable)
-    cube.BLUE(z, x, y,  brightness);   
      Set only blue leds , brightness is determined by color bit depth (depending of max_brightness variable)
      
   
      
Only few complex functions included:

Draw functions:

    cube_name.HSV_line(z0, x0, y0, z1, x1, y1,  hue,  S , V);
    cube_name.RGB_line(z0, x0, y0, z1, x1, y1,  red,  green ,  blue);
    cube_name.LED_line(z0, x0, y0, z1, x1, y1,  brightness);
    cube_name.GREEN_line(z0, x0, y0, z1, x1, y1,  brightness);
    cube_name.BLUE_line(z0, x0, y0, z1, x1, y1,  brightness);
    cube_name.RED_line(z0, x0, y0, z1, x1, y1,  brightness);    
    
    
  And the command to switch buffers :
  
  cube.drawVoxels(true/false);   
   
    - true: it will use vsync (just like in you PC monitor) .  Wait until previous frame is fully drawn, then show next frame.
    - false: do not use vsync. Don't wait until previous frame is fully drawn, start drawing next frame immidiatelly. Can cauze flickering at high  MODULATION_BIT_DEPTH.
    Also, can cause cube-tearing (best way to described is like this: Slow animation is drawing, and you see one frame in lower half of layers, then in upper half next frame that is  calculated and start to show.
    
     It's a matter of taste.  I preffer (true) , or just () . it's default.
    
    
    
HINTS:

-Use milis function for per-animation time
-I'll include animation that i had done so far. Learn from them.
-If using arduino uno, avoid animations with line functions. Not 100% tested, and i beleave that it doesn't have enough RAM for it.
- Video of 8x8x8 and 4x4x4 RGB cube can be found at: https://www.youtube.com/watch?v=fdaupiZpBj4



    
    
    

    Bakisha
    
    June 2019
      
      
        
        
        
            
            
            
            
