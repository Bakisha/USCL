#include<SPI.h>
#include<USCL.h>

float animtime = 60000; // duration of animations


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  TESTED HARDWARE
//  595             = BLUEPILL                              = ARDUINO  UNO,NANO,MICRO*        = Leonardo* (atmega32u4)        = MEGA 2560*                    = WEMOS_D1_mini*
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  OE pin          = PA0  // user defined                  = D10  // user defined            = A1  // User defined           = 49  // User defined           = D0  IO - OR D4  IO, 10k Pull-up, BUILTIN_LED - user defined
//  Latch           = PA1  // user defined                  = D9   // user defined            = A0  // User defined           = 47  // User defined           = D8  IO, 10k Pull-down, SS    - user defined
//  DataIN  (MOSI)  = PA7  // bluepill SPI1 MOSI            = D11 (MOSI)                      = 16 MOSI                       = 51 MOSI                       = D7 (MOSI)
//  SRCLK   (SCK)   = PA5  // bluepill SPI1 CLK             = D13 (SCK)                       = 15 SCK                        = 52 SCK                        = D5 (CLK)
//
// * - under testing, cube with this CPU still not finished/tested
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


// uncomment to match your cube



// wemos D1 mini
// Add this adress to your board manager, search: wemos
// http://arduino.esp8266.com/stable/package_esp8266com_index.json
// @60 fps ( minimum without flicker )
// 4x4x4 RGB - max MODULATION_BIT_DEPTH = 4 (recommended = 3 )
// 8x8x8 RGB - max MODULATION_BIT_DEPTH = 2 (recommended = 2 )
// 4x4x4 LED - max MODULATION_BIT_DEPTH = 5 (recommended = 4 )
// 8x8x8 LED - max MODULATION_BIT_DEPTH = 4 (recommended = 3 )
// SPI_speed - any, it is always same speed --> SLOW
// NOTE: Best for smaller ones, HUGE storage space for placing anymations, but low BAM bit depth. Maybe can be combined with Wifi stuff. 
// NOTE 2: Still under testing, Wemos D1 mini reset itself every few seconds. Not really tried on real cube. I was just looking timing on OE pin, and it looks OK.
//USCL cube(4, 4, 4, RGB_CUBE , D3, D2,  60 , 3, SPI_speed_64); // uncomment this if you are building for Wemos D1 mini


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



// STM32F1 (Bluepill, Blackpill)
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
USCL cube(4, 4, 4, RGB_CUBE , PA0, PA1,  60 , 6, SPI_speed_16); // My SEEED cube V1 
//
/*




*/
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE
//*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE
//*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE
//*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE*+*+*+*+*+*+*+*+*+*+*+*+PUT ANIMATIONS DOWN HERE
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




void plasma6(float time, USCL & cube, uint16_t shiftDelay)
{
  /* plasma1 parameters   ***   ***
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'USCL' class that you made in your main program.
    - shiftDelay :        Define the delay (in ms) . Normally a value of  0 - 100 ms will suffice.
  */
  // Declare and initialize other variables needed for the animation
  //  float omega = (2.0 * PI) / (2 * cube.getCubeSize()); // Calculate the radial frequency omega
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  int FPS = cube.getFPS() ;
  int delay_FPS = 1000 / FPS;
  float S = 1.0;
  float V = 1.0;
  int hue = 0;

  // Other variables
  float step_offset = 240.0;
  float xoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizex))); //19//24
  float yoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizey))); //37//37
  float zoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizez))); //24//19

  float xx;
  float yy;
  float zz;

  float step_offsetV = 180.0;
  float xoffsetV = float(PI / (((step_offsetV) + random(step_offsetV)) / float(cubesizex))); //19//24
  float yoffsetV = float(PI / (((step_offsetV) + random(step_offsetV)) / float(cubesizey))); //37//37
  float zoffsetV = float(PI / (((step_offsetV) + random(step_offsetV)) / float(cubesizez))); //24//19

  float xxV;
  float yyV;
  float zzV;

  float plasma;

  float xmap[cubesizex];
  float ymap[cubesizey];
  float zmap[cubesizez];
  for ( int cc = 0 ; cc < cubesizex; cc++)
  {
    xmap[cc] = map(cc, 0, cubesizex - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizex))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  for ( int cc = 0 ; cc < cubesizey; cc++)
  {
    ymap[cc] = map(cc, 0, cubesizey - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizey))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  for ( int cc = 0 ; cc < cubesizez; cc++)
  {
    zmap[cc] = map(cc, 0, cubesizez - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizez))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  float fx;
  float fy;
  float fz;

  float fxV;
  float fyV;
  float fzV;



  float  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    for ( int x = 0; x < cubesizex ; x++)
    {
      //fx = float(sin(xx + xmap[x])) ;
      fx = sin(xx + (xmap[x] / 1000.0));
      fxV = sin(xxV + (xmap[x] / 1000.0));
      for (int   z = 0; z < cubesizez; z++)
      {
        fz = sin(zz + (zmap[z]) / 1000.0) ;
        fz = sin(zzV  + (zmap[z]) / 1000.0) ;
        for ( int  y = 0 ; y < cubesizey ; y++ )
        {
          fy = sin(yy + (ymap[y] / 1000.0));
          fyV = sin(yyV + (ymap[y] / 1000.0));
          plasma =  (180.0 + 60.0 * (fx + fz + fy) ); // *max=3

          hue = int(plasma ) ;
          V =   0.5 + 0.5 * sin(2.0 * PI * ((fxV + fzV +  fyV)/3) ) ;
          cube.HSV( z, x, y, hue   , 1.0, V);

        }//  y
      }  //  z
    }    //  x


    xx = xx + xoffset;
    if (xx > 2.0 * PI)
    {
      xx = 0;


    }
    zz = zz + zoffset;
    if (zz > 2.0 * PI)
    {
      zz = 0;
    }
    yy = yy + yoffset;
    if (yy > 2.0 * PI)
    {
      yy = 0;

    }






    zzV = zzV + zoffsetV;
    if (zzV > 2.0 * PI)
    {
      zzV = 0;

    }
    xxV = xxV + xoffsetV;
    if (xxV > 2.0 * PI)
    {
      xxV = 0;

    }

    yyV = yyV + yoffsetV;
    if (yyV > 2.0 * PI)
    {
      yyV = 0;

    }

    cube.drawVoxels();
    if (shiftDelay > 0) {
      delay(shiftDelay);
    }


    // hue
  } // while



}//void plasma6


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void rotating_circles(float time, USCL & cube, unsigned int drawDelay)
{
  /* rotating circles parameters
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'LedCube' class that you made in your main  loop.
    - shiftDelay (optional):    Define the delay (in ms) . Normally a value of
                    50 - 100 ms will suffice.
  */

  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int max_brightness = cube.getMaxBrightness();
  //  int maxVoxelValue = cubesize - 1;
  int FPS = cube.getFPS() ;
  int delay_FPS = 1000 / FPS;
  float S = 1.0;
  float V = 1.0;
  int hue = 0;


  // Declare and initialize other variables needed for the animation

  // 3d matrix for points to draw
  ///////////////////////////////////////////// numbers to play with :-) ///////////////////////////////////////////////
  const int number_of_points = 9; //  total number of points to draw
  int  number_of_circles = 2; //(cubesizex / 4) + 1; // number of rotating circles
  float angle_speed_x = 1.0 * 2.0 * PI / (2.0 * FPS) ; // speed of x,y,z rotation // variable number is amount of seconds that axis rotate in 1 second
  float angle_speed_y = 1.0 * 2.0 * PI / (90.0 * FPS); // experiment with these values for best effect
  float angle_speed_z = 1.0 * 2.0 * PI / (30.0 * FPS);
  float zoom = 1.0 ; // variable for zooming in/out // for now, it is not per x,y,z, but one for all
  float zoom_speed = 0.001;
  int zoom_direction = -1;
  float zoom_min = 0.5;
  float zoom_max = 1.2; // try to keep points inside cubesize value
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  float half_cubesize = float(cubesizez / 2.0);
  // matrix for initial points to be drawn, in this case, it's a   ----   circle  -----
  float point [number_of_points] [3] =

    // a float matrix, must be same number as integer number_of_points
  {
    {  sin(0.00 * PI / 8.00), 0, cos(0.00 * PI / 8.00)},
    {  sin(2.00 * PI / 8.00), 0, cos(2.00 * PI / 8.00)},
    {  sin(4.00 * PI / 8.00), 0, cos(4.00 * PI / 8.00)},
    {  sin(6.00 * PI / 8.00), 0, cos(6.00 * PI / 8.00)},
    {  sin(8.00 * PI / 8.00), 0, cos(8.00 * PI / 8.00)},
    {  sin(10.00 * PI / 8.00), 0, cos(10.00 * PI / 8.00)},
    {  sin(12.00 * PI / 8.00), 0, cos(12.00 * PI / 8.00)},
    {  sin(14.00 * PI / 8.00), 0, cos(14.00 * PI / 8.00)},
    {  sin(0.00 * PI / 8.00), 0, cos(0.00 * PI / 8.00)}
  };  // -1, +1 boundary
  float rotated_points [number_of_points] [3]; // matrix to store calculated 3d points // value = number of current point, x , y ,z
  int xx[number_of_points], yy[number_of_points], zz[number_of_points]; // array for actuate coordinates in led cube for number of points
  float angle_x ; // variables for rotations around x,y,z axis
  float angle_y ;
  float angle_z ;


  // Voxel coordinates

  float x, x1, x2, x3;
  float y, y1, y2, y3;
  float z, z1, z2, z3;
  //  int xx, yy, zz;

  int hue_fps = 0 ; // color
  int i, j; // temps and counters
  float temp1, temp2, temp3;



  float startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    // color of animation
    hue_fps = hue_fps + 1;
    if (hue_fps >  FPS)
    {
      hue_fps = 0;
      hue = hue +  1;
      if (hue > 360 / number_of_circles )
      {
        hue = 0;
      }
    } // change hue once in 1 second


    for (j = 0; j < number_of_circles; j++)
    {

      temp1 = angle_x;
      temp2 = angle_y;
      temp3 = angle_z;
      angle_x = angle_x + float(j * 2.0  * PI / ( 2.0 * number_of_circles));
      angle_y = angle_y + float(j * 2.0  * PI / ( 2.0 * number_of_circles));
      angle_z = angle_z + float(j * 2.0  * PI / ( 2.0 * number_of_circles));

      for (i = 0; i < number_of_points ; i++)
      {
        x = point [i] [0]; // x from point matrix
        y = point [i] [1]; // y from point matrix
        z = point [i] [2]; // z from point matrix



        // rotating x axis
        x1 = x;
        y1 = cos(angle_x) * y - sin(angle_x) * z;
        z1 = sin(angle_x) * y + cos(angle_x) * z;
        // rotating y axis
        x2 = cos(angle_y) * x1 + sin(angle_y) * z1;
        y2 = y1;
        z2 = -sin(angle_y) * x1 + cos(angle_y) * z1;
        // rotating z axis
        x3 = cos(angle_z) * x2 - sin(angle_z) * y2;
        y3 = sin(angle_z) * x2 + cos(angle_z) * y2;
        z3 = z2;
        rotated_points [i] [0] = x3; // rotated x point
        rotated_points [i] [1] = y3; // rotated y point
        rotated_points [i] [2] = z3; // rotated z point
      } // number of points


      // calculate x,y,z coordinates for led cube
      for (i = 0; i < number_of_points; i++)
      {
        xx[i] = zoom * ((rotated_points [i] [0]) * half_cubesize) + half_cubesize;
        yy[i] = zoom * ((rotated_points [i] [1]) * half_cubesize) + half_cubesize;
        zz[i] = zoom * ((rotated_points [i] [2]) * half_cubesize) + half_cubesize;
      } //

      // connect dots - connect (i,(i+1) ),last-0
      for (i = 0; i < number_of_points - 1 ; i++) // for connecting 4 sides of cube
      {
        x1 = xx [i] ;
        x2 = xx [i + 1 ] ;
        y1 = yy [i];
        y2 = yy [i + 1] ;
        z1 = zz [i] ;
        z2 = zz [i + 1] ;
        // now draw a line from point to point
        cube.HSV_line(z1, x1, y1, z2, x2, y2, hue + j * 360 / number_of_circles, S, V);

      } // i

      angle_x  = temp1 ;
      angle_y = temp2 ;
      angle_z = temp3;

    } // j // do it -number_of_circles- times


    ////////////////////////////////////////// draw leds
    cube.drawVoxels(); // Draw the frame to the cube explicitly using vsync
    if (drawDelay > 0) {
      delay(drawDelay);
    }
    /////////////////////////////////////////////////

    angle_x = angle_x + angle_speed_x;
    if (angle_x > 2 * PI) {
      angle_x = 0;
    }
    angle_y = angle_y + angle_speed_y;
    if (angle_y > 2 * PI) {
      angle_y = 0;
    }
    angle_z = angle_z + angle_speed_z;
    if (angle_z > 2 * PI) {
      angle_z = 0;
    }

    zoom = zoom + zoom_direction * zoom_speed;
    if (zoom < zoom_min || zoom > zoom_max)
    {
      zoom_direction = -zoom_direction;
    }



  }//while
} //void
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void rotating_star(float time, USCL & cube, unsigned int drawDelay)
{
  /* rotating star parameters
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'LedCube' class that you made in your main  loop.
    - shiftDelay (optional):    Define the delay (in ms) . Normally a value of
                    50 - 100 ms will suffice.
  */

  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int max_brightness = cube.getMaxBrightness();
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  // int maxVoxelValue = cubesize - 1;
  float S = 1;
  float V = 1;
  int hue = 0;

  // Declare and initialize other variables needed for the animation

  // 3d matrix for points to draw
  float half_cubesize = float(cubesizez / 2.0);
  const int number_of_points_star = 11; //  total number of points to draw
  float inner_r = 0.5; // iner circle of a star
  float outer_r = 1.0; // outer circle of a star
  // star coordinates taken from https://www.codeproject.com/Articles/18149/Draw-a-US-Flag-using-C-and-GDI

  // matrix for initial points to be drawn, in this case, it's a   ----   STAR  -----
  float point [number_of_points_star] [3] = { // a float matrix, must be same number as integer number_of_points

    { 0, -outer_r, 0},
    {  inner_r * sin(2 * PI / 10), -inner_r * cos(2 * PI / 10), 0},
    {  outer_r * sin(2 * PI / 5), -outer_r * cos(2 * PI / 5), 0},
    { inner_r * sin(2 * PI / 5), inner_r * cos(2 * PI / 5), 0},
    { outer_r * sin(2 * PI / 10), outer_r * cos(2 * PI / 10), 0},
    {  0, inner_r, 0},
    {  -outer_r * sin(2 * PI / 10), outer_r * cos(2 * PI / 10), 0},
    { -inner_r * sin(2 * PI / 5), inner_r * cos(2 * PI / 5), 0},
    { -outer_r * sin(2 * PI / 5),   -outer_r * cos(2 * PI / 5), 0},
    { -inner_r * sin(2 * PI / 10),  -inner_r * cos(2 * PI / 10), 0},
    { 0, -outer_r, 0}
  };  // star is in -1, +1 boundary


  float rotated_points [number_of_points_star] [3]; // matrix to store calculated 3d points // value = number of current point, x , y ,z
  int xx[number_of_points_star], yy[number_of_points_star], zz[number_of_points_star]; // array for actuate coordinates in led cube for number of points
  float angle_x = 0; // variables for rotations around x,y,z axis
  float angle_y = 0;
  float angle_z = 0;


  float angle_speed_x =  2 * PI / (2 * FPS) ; //  speed of x,y,z rotation // variable number is amount of seconds that axis rotate in 1 second
  float angle_speed_y =  2 * PI / (90 * FPS); // experiment with these values for best effect
  float angle_speed_z =  2 * PI / (180 * FPS); // try to keep points inside cubesize value
  float zoom = 1.0 ; // variable for zooming in/out
  float zoom_speed = 0.0001;
  int zoom_direction = -1;


  // Voxel coordinates

  float x, x1, x2, x3;
  float y, y1, y2, y3;
  float z, z1, z2, z3;
  //  int xx, yy, zz;

  int hue_fps = 0; // color
  int i, j; // temps and counters


  long startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    for (i = 0; i < number_of_points_star; i++) {

      x = point [i] [0]; // x from point matrix
      y = point [i] [1]; // y from point matrix
      z = point [i] [2]; // z from point matrix
      // rotating x axis
      x1 = x;
      y1 = cos(angle_x) * y - sin(angle_x) * z;
      z1 = sin(angle_x) * y + cos(angle_x) * z;
      // rotating y axis
      x2 = cos(angle_y) * x1 + sin(angle_y) * z1;
      y2 = y1;
      z2 = -sin(angle_y) * x1 + cos(angle_y) * z1;
      // rotating z axis
      x3 = cos(angle_z) * x2 - sin(angle_z) * y2;
      y3 = sin(angle_z) * x2 + cos(angle_z) * y2;
      z3 = z2;
      rotated_points [i] [0] = x3; // rotated x point
      rotated_points [i] [1] = y3; // rotated y point
      rotated_points [i] [2] = z3; // rotated z point
    } // number of points
    // color of animation
    hue_fps = hue_fps + 1;
    if (hue_fps >  FPS)
    {
      hue_fps = 0;
      hue = hue +  1; // max_brightness / 4;
      if (hue > 360 ) {
        hue = 0;
      }
    } // change hue once in 1 second

    // calculate x,y,z coordinates for led cube
    for (i = 0; i < number_of_points_star; i++)
    {
      xx[i] = zoom * ((rotated_points [i] [0]) * half_cubesize) + half_cubesize;
      yy[i] = zoom * ((rotated_points [i] [1]) * half_cubesize) + half_cubesize;
      zz[i] = zoom * ((rotated_points [i] [2]) * half_cubesize) + half_cubesize;
    } //

    // connect dots - connect (i,(i+1) ),last-0
    for (i = 0; i < number_of_points_star - 1; i++) // for connecting 4 sides of cube
    {
      x1 = xx [i] ;
      x2 = xx [i + 1 ] ;
      y1 = yy [i];
      y2 = yy [i + 1] ;
      z1 = zz [i] ;
      z2 = zz [i + 1] ;
      // now draw a line from point to point
      cube.HSV_line(z1, x1, y1, z2, x2, y2, hue , S, V);
    } // i


    angle_x = angle_x + angle_speed_x;
    if (angle_x > 2 * PI) {
      angle_x = 0;
    }
    angle_y = angle_y + angle_speed_y;
    if (angle_y > 2 * PI) {
      angle_y = 0;
    }
    angle_z = angle_z + angle_speed_z;
    if (angle_z > 2 * PI) {
      angle_z = 0;
    }

    zoom = zoom + zoom_direction * zoom_speed;
    if (zoom < 0.6 || zoom > 1.1)
    {
      zoom_direction = -zoom_direction;
    }


    ////////////////////////////////////////// draw leds
    cube.drawVoxels(); // Draw the frame to the cube explicitly using vsync
    if (drawDelay > 0) {
      delay(drawDelay);
    }
    /////////////////////////////////////////////////


  }//while
} //void
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void rotating_cube(float time, USCL & cube, unsigned int drawDelay)
{
  /* rotating_cube parameters
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'LedCube' class that you made in your main  loop.
    - shiftDelay (optional):    Define the delay (in ms) . Normally a value of
                    50 - 100 ms will suffice.
  */

  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  //  int maxVoxelValue = cubesize - 1;
  float S = 1;
  float V = 1;
  int hue = 0;

  // Declare and initialize other variables needed for the animation

  // 3d matrix for points to draw
  float half_cubesize = cubesizez / 2.0;
  int number_of_points = 8; //  total number of points to draw

  // matrix for initial points to be drawn, in this case, it's a   ----   CUBE  -----
  float point [8] [3] = { // a float matrix, must be same number as integer number_of_points
    { -1, -1, -1},
    {  1, -1, -1},
    {  1, 1, -1},
    { -1, 1, -1},
    { -1, -1, 1},
    {  1, -1, 1},
    {  1, 1, 1},
    { -1, 1, 1}
  };  // cube is in -1, +1 boundary


  float rotated_points [8] [3]; // matrix to store calculated 3d points // value = number of current point, x , y ,z
  int xx[8], yy[8], zz[8]; // array for actuate coordinates in led cube for number of points
  float angle_x = 0; // variables for rotations around x,y,z axis
  float angle_y = 0;
  float angle_z = 0;


  float angle_speed_x =  2 * PI / (3 * FPS) ; // 120.00; // speed of x,y,z rotation // variable number is amount of seconds that axis rotate in 1 second
  float angle_speed_y =  2 * PI / (60 * FPS); // experiment with these values for best effect
  float angle_speed_z =  3 * PI / (180 * FPS); // try to keep points inside cubesize value
  float zoom = 0.7 ; // variable for zooming in/out


  // Voxel coordinates

  float x, x1, x2, x3;
  float y, y1, y2, y3;
  float z, z1, z2, z3;
  //  int xx, yy, zz;

  int hue_fps = 0 ; // color
  int i, j; // temps and counters



  long startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////
    for (i = 0; i < number_of_points; i++) {

      x = point [i] [0]; // x from point matrix
      y = point [i] [1]; // y from point matrix
      z = point [i] [2]; // z from point matrix


      // rotating x axis
      x1 = x;
      y1 = cos(angle_x) * y - sin(angle_x) * z;
      z1 = sin(angle_x) * y + cos(angle_x) * z;

      // rotating y axis
      x2 = cos(angle_y) * x1 + sin(angle_y) * z1;
      y2 = y1;
      z2 = -sin(angle_y) * x1 + cos(angle_y) * z1;

      // rotating z axis

      x3 = cos(angle_z) * x2 - sin(angle_z) * y2;
      y3 = sin(angle_z) * x2 + cos(angle_z) * y2;
      z3 = z2;

      rotated_points [i] [0] = x3; // rotated x point
      rotated_points [i] [1] = y3; // rotated y point
      rotated_points [i] [2] = z3; // rotated z point

    } // number of points


    // calculate x,y,z coordinates for led cube
    for (i = 0; i < number_of_points; i++)
    {
      xx[i] = zoom * ((rotated_points [i] [0]) * half_cubesize) + half_cubesize;
      yy[i] = zoom * ((rotated_points [i] [1]) * half_cubesize) + half_cubesize;
      zz[i] = zoom * ((rotated_points [i] [2]) * half_cubesize) + half_cubesize;
    } //
    // color of animation
    hue_fps = hue_fps + 1;
    if (hue_fps >  FPS)
    {
      hue_fps = 0;
      hue = hue +  cubesizez / 2;
      if (hue > 360 / 2) {
        hue = 0;
      }
    } // change hue once in 1 second

    // connect dots (square sides of a cube)
    // 0-1
    // 1-2
    // 2-3
    // 3-0
    //
    // 4-5
    // 5-6
    // 6-7
    // 7-4
    //
    // 0-4
    // 1-5
    // 2-6
    // 3-7
    //
    // 1-connect (i,(i+1)%4 )
    // 2-connect (i+4,((i+1)%4)+4 )
    // 3-connect(i,i+4)
    // inspiration from youtube : Coding challenge #112 24:50


    // 1-connect (i,(i+1)%4 )
    for (i = 0; i < 4; i++)  // for connecting 4 sides of cube
    {
      x1 = xx [i] ;
      x2 = xx [(i + 1) % 4] ;
      y1 = yy [i];
      y2 = yy [(i + 1) % 4] ;
      z1 = zz [i] ;
      z2 = zz [(i + 1) % 4] ;
      // now draw a line from point to point

      cube.HSV_line(z1, x1, y1, z2, x2, y2, hue , S, V);
      /*
        int8_t zDirectionVector = z2 - z1;
        int8_t xDirectionVector = x2 - x1;
        int8_t yDirectionVector = y2 - y1;
        for (j = 0; j < cubesize; j++)
        {
        z = round((j / (float)maxVoxelValue) * zDirectionVector + z1);
        x = round((j / (float)maxVoxelValue) * xDirectionVector + x1);
        y = round((j / (float)maxVoxelValue) * yDirectionVector + y1);
        cube.HSV(z, x, y, (hue) % 360, S, V);
        } // j
      */
    } // i
    // 2-connect (i+4,((i+1)%4)+4 )
    for (i = 0; i < 4; i++)  // for connecting 4 sides of cube
    {
      x1 = xx [i + 4] ;
      x2 = xx [((i + 1) % 4) + 4 ] ;
      y1 = yy [i + 4];
      y2 = yy [((i + 1) % 4) + 4 ] ;
      z1 = zz [i + 4] ;
      z2 = zz [((i + 1) % 4) + 4 ] ;
      // now draw a line from point to point
      cube.HSV_line(z1, x1, y1, z2, x2, y2, hue + 180, S, V);
      /*
        int8_t zDirectionVector = z2 - z1;
        int8_t xDirectionVector = x2 - x1;
        int8_t yDirectionVector = y2 - y1;
        for (j = 0; j < cubesize; j++)
        {
        z = round((j / (float)maxVoxelValue) * zDirectionVector + z1);
        x = round((j / (float)maxVoxelValue) * xDirectionVector + x1);
        y = round((j / (float)maxVoxelValue) * yDirectionVector + y1);
        cube.HSV(z, x, y, (hue + 120) % 360, S, V);
        } // j
      */
    } // i
    /*
        //  3-connect(i,i+4)
        for (i = 0; i < 4; i++)  // for connecting 4 sides of cube
        {
          x1 = xx [i] ;
          x2 = xx [i + 4] ;
          y1 = yy [i];
          y2 = yy [i + 4] ;
          z1 = zz [i] ;
          z2 = zz [i + 4] ;
          // now draw a line from point to point
          cube.HSV_line(z1, x1, y1, z2, x2, y2, (hue + 240) % 360, S, V);
          /*

            int zDirectionVector = z2 - z1;
            int xDirectionVector = x2 - x1;
            int yDirectionVector = y2 - y1;
            for (j = 0; j < cubesize; j++)
            {
            z = ((j / (float)maxVoxelValue) * zDirectionVector + z1);
            x = ((j / (float)maxVoxelValue) * xDirectionVector + x1);
            y = ((j / (float)maxVoxelValue) * yDirectionVector + y1);

            cube.HSV(z, x, y, (hue + 240) % 360, S, V);
            } // j
    */
    /*
       } // i

    */

    angle_x = angle_x + angle_speed_x;
    if (angle_x > 2 * PI) {
      angle_x = 0;
    }
    angle_y = angle_y + angle_speed_y;
    if (angle_y > 2 * PI) {
      angle_y = 0;
    }
    angle_z = angle_z + angle_speed_z;
    if (angle_z > 2 * PI) {
      angle_z = 0;
    }


    ////////////////////////////////////////// draw leds
    cube.drawVoxels(); // Draw the frame to the cube explicitly using vsync
    delay(drawDelay);
    /////////////////////////////////////////////////

  }//while
} //void
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void plasma_sinx2y2(float time, USCL & cube, uint16_t shiftDelay /* = 75 */)
{
  /* plasma sinx2y2 cube parameters
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'LedCube' class that you made in your main program.
    - shiftDelay (optional):    Define the delay (in ms) . Normally a value of
                    50 - 100 ms will suffice.
  */

  // usual  variables
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  // byte maxVoxelValue = cubesize - 1;
  float S = 1;
  float V = 1;
  int hue = 180;
  // Declare and initialize other variables needed for the animation
  float omega = (2.0 * PI) / (2 * cubesizez); // Calculate the radial frequency omega


  // Other variables
  float step_offset = 180.00;
  float xoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizex))); //19//24
  float yoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizey))); //37//37
  float zoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizez))); //24//19

  float xmap[cubesizex];
  float ymap[cubesizey];
  float zmap[cubesizez];
  for ( int cc = 0 ; cc < cubesizex; cc++)
  {
    xmap[cc] = map(cc, 0, cubesizex - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizex))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  for ( int cc = 0 ; cc < cubesizey; cc++)
  {
    ymap[cc] = map(cc, 0, cubesizey - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizey))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  for ( int cc = 0 ; cc < cubesizez; cc++)
  {
    zmap[cc] = map(cc, 0, cubesizez - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizez))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }

  float fx;
  float fy;
  float fz;

  float plasma;

  float xx;
  float yy;
  float zz;


  long  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////


    for ( float offset = 0; offset < 4 * PI ; offset = offset +  0.18 ) {

      //  hue = 180 + 180 * (sin(PI * sin(offset / 4.0)));

      for ( byte x = 0; x < cubesizex; x++) {
        for ( byte y = 0; y < cubesizey; y++)
        {
          float      ipsilon = 10000 *  sin(offset + sqrt(pow(map(x, 0, cubesizez - 1, -PI, PI), 2) + pow(map(y, 0, cubesizez - 1, -PI, PI), 2)));
          // y=sin(z * omega + offset ) * cos( x * omega + offset  );
          float     z   = map(ipsilon, -10000, 10000, 0, cubesizez);

          float      br = 10000 * sin(x * omega + offset / 0.55  );
          fx = sin(xx + (xmap[int(x)] / 1000.0));
          fz = sin(zz + (zmap[int(z)]) / 1000.0) ;
          fy = cos(yy + (ymap[int(y)] / 1000.0));

          br = map(br, -10000, 10000, 1, 10000 );

          plasma = 180 + 60 * (fx + fz + fy) ;
          cube.HSV(z, x, y, plasma, 1, float(br) / 100.00);
        }
      }
      cube.drawVoxels();
      if (shiftDelay > 0) {
        delay(shiftDelay);
      }


      xx = xx + xoffset;
      if (xx > 2.0 * PI)
      {
        xx = 0;

      }
      yy = yy + yoffset;
      if (yy > 2.0 * PI)
      {
        yy = 0;

      }
      zz = zz + zoffset;
      if (zz > 2.0 * PI)
      {
        zz = 0;
      }

    } // offset

    ///////////////////////
  } // while
}// void


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



void plasma_sineWave(float time, USCL & cube, uint16_t shiftDelay /* = 75 */, uint8_t period /* = 16 */)
{
  /* Plasma colored Sine wave parameters
    - time:                 Define the length of time that the animation should be executed.
    - cubeRef:            Define the instance of the 'LedCube' class that you made in your main program.
    - shiftDelay (optional):    Define the delay (in ms) between each iteration of shifting the sine wave to a new position. Normally a value of
                  50 - 100 ms will suffice.
    - period (optional):    Period time of the sine wave. Normally a value of 2 times the cube size will suffice.
  */
  // usual  variables
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  float S = 1;
  float V = 1;
  int hue;
  // Voxel coordinates
  uint8_t z;
  uint8_t x;
  uint8_t y;

  // For loop iterators
  uint16_t i;
  uint8_t j;

  // Other variables
  uint8_t timeCount = 0;
  float sineOffset = maxVoxelValue / 2.0;
  float omega = (2.0 * PI) / (float)period;
  float omegaZ = (2.0 * PI) / (float)maxVoxelValue;

  float step_offset = 180.00;
  float xoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizex))); //19//24
  float yoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizey))); //37//37
  float zoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizez))); //24//19


  float xmap[cubesizex];
  float ymap[cubesizey];
  float zmap[cubesizez];
  for ( int cc = 0 ; cc < cubesizex; cc++)
  {
    xmap[cc] = map(cc, 0, cubesizex - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizex))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  for ( int cc = 0 ; cc < cubesizey; cc++)
  {
    ymap[cc] = map(cc, 0, cubesizey - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizey))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  for ( int cc = 0 ; cc < cubesizez; cc++)
  {
    zmap[cc] = map(cc, 0, cubesizez - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizez))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }



  float fx;
  float fy;
  float fz;

  float plasma;

  float xx;
  float yy;
  float zz;

  long  startTime = millis();
  while (millis() - startTime < time)
  {





    for (i = timeCount; i < (timeCount + cubesizez); i++)
    {
      z = round(sineOffset * sin(i * omega) + sineOffset); // Calculate z position
      x = i - timeCount;
      if (x < 0) x = 0;
      if (x > cubesizex - 1) x = cubesizex - 1;
      fx = sin(xx + (xmap[x] / 1000.0));
      fz = sin(zz + (zmap[z]) / 1000.0) ;
      for (j = 0; j < cubesizey; j++)
      {
        y = j;



        fy = cos(yy + (ymap[y] / 1000.0));

        plasma = 180 + 60 * (fx + fz + fy) ;

        if (plasma >= 0.0) {
          if (plasma <= 360.0 ) {
            cube.HSV(z, x, y, plasma, 1.00, 1.00);
          }
        }


      } // j
    } // i
    cube.drawVoxels();
    delay(shiftDelay);
    timeCount++;
    if (timeCount >= period)
      timeCount = 0;


    xx = xx + xoffset;
    if (xx > 2.0 * PI)
    {
      xx = 0;

    }
    yy = yy + yoffset;
    if (yy > 2.0 * PI)
    {
      yy = 0;

    }
    zz = zz + zoffset;
    if (zz > 2.0 * PI)
    {
      zz = 0;
    }


  }//while
}//void

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




void plasma5(float time, USCL & cube, uint16_t shiftDelay)
{
  /* plasma1 parameters   ***   ***
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'USCL' class that you made in your main program.
    - shiftDelay :        Define the delay (in ms) . Normally a value of  0 - 100 ms will suffice.
  */
  // Declare and initialize other variables needed for the animation
  //  float omega = (2.0 * PI) / (2 * cube.getCubeSize()); // Calculate the radial frequency omega
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();

  // Other variables
  float step_offset = 180.00;
  float xoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizex))); //19//24
  float yoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizey))); //37//37
  float zoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizez))); //24//19

  float xx;
  float yy;
  float zz;


  float plasma;
  float hue;
  int int_hue;


  float xmap[cubesizex];
  float ymap[cubesizey];
  float zmap[cubesizez];
  for ( int cc = 0 ; cc < cubesizex; cc++)
  {
    xmap[cc] = map(cc, 0, cubesizex - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizex))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  for ( int cc = 0 ; cc < cubesizey; cc++)
  {
    ymap[cc] = map(cc, 0, cubesizey - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizey))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  for ( int cc = 0 ; cc < cubesizez; cc++)
  {
    zmap[cc] = map(cc, 0, cubesizez - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizez))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }

  float fx;
  float fy;
  float fz;


  float  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    for ( int x = 0; x < cubesizex ; x++)
    {
      //fx = float(sin(xx + xmap[x])) ;
      fx = sin(xx + (xmap[x] / 1000.0));
      for (int   z = 0; z < cubesizez; z++)
      {
        fz = sin(zz + (zmap[z]) / 1000.0) ;
        for ( int  y = 0 ; y < cubesizey ; y++ )
        {
          fy = cos(yy + (ymap[y] / 1000.0));
          plasma = 180 + 70 * (fx + fz + fy) ; // *max=3
          if (plasma >= 0.0) {
            if (plasma <= 360.0 ) {
              cube.HSV( z, x, y, plasma   , 1.0, float( 1.1 / max_brightness) );
            }
          }
        }//  y
      }  //  z
    }    //  x
    xx = xx + xoffset;
    if (xx > 2.0 * PI)
    {
      xx = 0;
      yy = yy + yoffset;
      if (yy > 2.0 * PI)
      {
        yy = 0;

      }
    }

    zz = zz + zoffset;
    if (zz > 2.0 * PI)
    {
      zz = 0;
    }





    cube.drawVoxels();
    if (shiftDelay > 0) {
      delay(shiftDelay);
    }


    // hue
  } // while

  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


}//void plasma5

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



void plasma4(float time, USCL & cube, uint16_t shiftDelay)
{
  /* plasma1 parameters   ***   ***
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'USCL' class that you made in your main program.
    - shiftDelay :        Define the delay (in ms) . Normally a value of  0 - 100 ms will suffice.
  */
  // Declare and initialize other variables needed for the animation
  //  float omega = (2.0 * PI) / (2 * cube.getCubeSize()); // Calculate the radial frequency omega
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();

  // Other variables
  float step_offset = 180.00;
  float xoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizex))); //19//24
  float yoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizey))); //37//37
  float zoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizez))); //24//19

  float xx;
  float yy;
  float zz;


  float plasma;
  float hue;
  int int_hue;

  float xmap[cubesizex];
  float ymap[cubesizey];
  float zmap[cubesizez];
  for ( int cc = 0 ; cc < cubesizex; cc++)
  {
    xmap[cc] = map(cc, 0, cubesizex - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizex))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  for ( int cc = 0 ; cc < cubesizey; cc++)
  {
    ymap[cc] = map(cc, 0, cubesizey - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizey))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  for ( int cc = 0 ; cc < cubesizez; cc++)
  {
    zmap[cc] = map(cc, 0, cubesizez - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizez))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }


  float fx;
  float fy;
  float fz;


  float  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    for ( int x = 0; x < cubesizex ; x++)
    {
      //fx = float(sin(xx + xmap[x])) ;
      fx = sin(xx + (xmap[x] / 1000.0));
      for (int   z = 0; z < cubesizez; z++)
      {
        fz = sin(zz + (zmap[z]) / 1000.0) ;
        for ( int  y = 0 ; y < cubesizey ; y++ )
        {
          fy = cos(yy + (ymap[y] / 1000.0));
          plasma = 180 + 80 * (fx + fz + fy) ;
          if (plasma >= 0.0) {
            if (plasma <= 360.0 ) {
              cube.HSV( z, x, y, plasma   , 1.0, 1.0);
            }
          }
        }//  y
      }  //  z
    }    //  x
    xx = xx + xoffset;
    zz = zz + zoffset;
    yy = yy + yoffset;
    if (xx > 2.0 * PI) {
      xx = 0;
    }
    if (yy > 2.0 * PI) {
      yy = 0;
    }
    if (zz > 2.0 * PI) {
      zz = 0;
    }


    cube.drawVoxels();
    if (shiftDelay > 0) {
      delay(shiftDelay);
    }


    // hue
  } // while

  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


}//void plasma1


void RandomLedFade(float time, USCL & cube, unsigned char numberOfVoxels, unsigned int drawDelay)
{
  /* RandomLedAnimator parameters
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'LedCube' class that you made in your main program.
    - numberOfVoxels:  Define the number of voxels that fade.
    - shiftDelay (optional):    Define the delay (in ms) . Normally a value of
                    50 - 100 ms will suffice.
  */

  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  float S = 1;
  float V = 1;
  int hue;

  // Declare and initialize other variables needed for the animation
  // Voxel coordinates
  uint8_t z;
  uint8_t x;
  uint8_t y;
  uint8_t br;

  // Always set the timestamp to 0
  long  timestamp = 0;
  int ledZ[numberOfVoxels];
  int ledX[numberOfVoxels];
  int ledY[numberOfVoxels];
  int ledHUE[numberOfVoxels];
  int fade_speed[numberOfVoxels];
  float ledV[numberOfVoxels];
  float ledS[numberOfVoxels];
  for (int i = 0; i < numberOfVoxels; i++)
  {

    ledZ[i] = random(cubesizez);
    ledX[i] = random(cubesizex);
    ledY[i] = random(cubesizey);
    ledHUE[i] =  random(360 );
    fade_speed[i] = 1 + random(max_brightness);
    ledV[i] = 100.00;
    ledS[i] = 0.0;
  }



  long startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    // draw leds
    for (int i = 0; i < numberOfVoxels; i++)
    {
      cube.HSV(ledZ[i], ledX[i], ledY[i], ledHUE[i], ledS[i] / 100.0, ledV[i] / 100.0); // Set a voxel brightness in the current frame with the random generated z, x, y coordinates
    }
    cube.drawVoxels(); // Draw the frame to the cube explicitly using vsync
    delay(drawDelay);

    for (int i = 0; i < numberOfVoxels; i++)
    {
      if (ledS[i] <= 100.0)
      {
        ledS[i] = ledS[i] + fade_speed[i];
      }
      else
      {
        ledV[i] = ledV[i] - fade_speed[i];
        if (ledV[i] < 0) // led has faded, calculate new led
        {
          ledZ[i] = random(cubesizez);
          ledX[i] = random(cubesizex);
          ledY[i] = random(cubesizey);
          ledHUE[i] = random(360 );
          fade_speed[i] = 1 + random(max_brightness);
          ledV[i] = 100.00;
          ledS[i] = 0.0;
        }
      }
    }




  }//while
} //void
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void color_wheelTWO(float time, USCL & cube, uint16_t shiftDelay /* = 75 */)
{
  /*
    color_wheelTWO cube parameters
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'LedCube' class that you made in your main program.
    - shiftDelay (optional):    Define the delay (in ms) . Normally a value of
                    50 - 100 ms will suffice.
  */

  // usual  variables
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  float S = 1;
  float V = 1;
  int hue;


  //*****colorWheelTwo*****colorWheelTwo*****colorWheelTwo*****colorWheelTwo*****colorWheelTwo
  int xx, yy, zz, ww, rr = 1, gg = 1, bb = 1, ranx, rany , ranz, select, swiper;


  long  startTime = millis();
  while (millis() - startTime < time)
  {

    swiper = random(6);

    hue =  random(360 );




    if (swiper == 0) {
      for (yy = 0; yy < cubesizey; yy++) { //left to right
        for (xx = 0; xx < cubesizex; xx++) {
          for (zz = 0; zz < cubesizez; zz++) {
            cube.HSV(zz, yy, xx, hue, 1, 1);
          }
        }
        cube.drawVoxels();
        delay(shiftDelay);
      }
    }
    if (swiper == 1) { //bot to top
      for (xx = 0; xx < cubesizex; xx++) {
        for (yy = 0; yy < cubesizey; yy++) {
          for (zz = 0; zz < cubesizez; zz++) {
            cube.HSV(zz, yy, xx, hue, 1, 1);
          }
        }
        cube.drawVoxels();
        delay(shiftDelay);
      }
    }
    if (swiper == 2) { //back to front
      for (zz = 0; zz < cubesizez; zz++) {
        for (xx = 0; xx < cubesizex; xx++) {
          for (yy = 0; yy < cubesizey; yy++) {
            cube.HSV(zz, yy, xx, hue, 1, 1);
          }
        }
        cube.drawVoxels();
        delay(shiftDelay);
      }
    }
    if (swiper == 3) {
      for (yy = cubesizey - 1; yy >= 0; yy--) { //right to left
        for (xx = 0; xx < cubesizex; xx++) {
          for (zz = 0; zz < cubesizez; zz++) {
            cube.HSV(zz, yy, xx, hue, 1, 1);
          }
        }
        cube.drawVoxels();
        delay(shiftDelay);
      }
    }
    if (swiper == 4) { //top to bot
      for (xx = cubesizex - 1; xx >= 0; xx--) {
        for (yy = 0; yy < cubesizey; yy++) {
          for (zz = 0; zz < cubesizez; zz++) {
            cube.HSV(zz, yy, xx, hue, 1, 1);
          }
        }
        cube.drawVoxels();
        delay(shiftDelay);
      }
    }
    if (swiper == 5) { //front to back
      for (zz = cubesizez - 1; zz >= 0; zz--) {
        for (xx = 0; xx < cubesizex; xx++) {
          for (yy = 0; yy < cubesizey; yy++) {
            cube.HSV(zz, yy, xx, hue, 1, 1);
          }
        }
        cube.drawVoxels();
        delay(shiftDelay);
      }
    }




  }//while

}//color wheel

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void sineWaveThree(float time, USCL & cube, uint16_t shiftDelay /* = 75 */, uint8_t period /* = 16 */,  uint8_t baseWavePeriodicFactor /* = 4 */)
{
  /* Sine wave three parameters
    - time:                           Define the length of time that the animation should be executed.
    - cubeRef:                      Define the instance of the 'LedCube' class that you made in your main program.
    - shiftDelay (optional):              Define the delay (in ms) between each iteration of shifting the sine wave to a new position. Normally a value of
                              50 - 100 ms will suffice.
    - period (optional):              Period time of the main sine wave . Normally a value of 2 times the cube size will suffice.
    - baseWavePeriodicFactor (optional):  A factor that represents how many times larger the period of the base (carrying wave) has to be compared to the main wave its period.
  */
  // usual  variables
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  float S = 1;
  float V = 1;
  int hue;

  // Voxel coordinates
  uint8_t z;
  uint8_t x;
  uint8_t y;

  // For loop iterators
  uint16_t i;
  uint8_t j;

  // Other variables
  float timeCount = 0;
  float sineOffset = float((maxVoxelValue) / 4.0);
  float omegaAlpha = (2.0 * PI) / (float)period;
  float omegaBeta = (2.0 * PI) / ((float)period * baseWavePeriodicFactor);
  float omegaZ = (2.0 * PI) / (float)maxVoxelValue;



  long  startTime = millis();
  while (millis() - startTime < time)
  {
    for (hue = 0; hue < 360; hue++)
    {
      for (i = timeCount; i < (timeCount + cubesizez); i++)
      {
        x = i - timeCount;
        z = round(sineOffset * (sin(i * omegaAlpha) - sin(i * omegaBeta + PI)) + 2 * sineOffset); // Calculate z position
        for (j = 0; j < cubesizey; j++)
        {
          y = j;
          V = 0.5 +   sin( (2.0 * PI) / (z + 1) );
          cube.HSV(z, x, y, hue , 1, V);
        }
      }

      cube.drawVoxels();
      delay(shiftDelay);

      timeCount++;
      if (timeCount >= period * baseWavePeriodicFactor)
        timeCount = 0;

    }//hue
  }//while
}//void

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



void sineWave(float time, USCL & cube, uint16_t shiftDelay /* = 75 */, uint8_t period /* = 16 */)
{
  /* Sine wave parameters
    - time:                 Define the length of time that the animation should be executed.
    - cubeRef:            Define the instance of the 'LedCube' class that you made in your main program.
    - shiftDelay (optional):    Define the delay (in ms) between each iteration of shifting the sine wave to a new position. Normally a value of
                  50 - 100 ms will suffice.
    - period (optional):    Period time of the sine wave. Normally a value of 2 times the cube size will suffice.
  */
  // usual  variables
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  float S = 1;
  float V = 1;
  int hue;
  // Voxel coordinates
  uint8_t z;
  uint8_t x;
  uint8_t y;

  // For loop iterators
  uint16_t i;
  uint8_t j;

  // Other variables
  uint8_t timeCount = 0;
  float sineOffset = maxVoxelValue / 2.0;
  float omega = (2.0 * PI) / (float)period;
  float omegaZ = (2.0 * PI) / (float)maxVoxelValue;



  long  startTime = millis();
  while (millis() - startTime < time)
  {
    for (hue = 0; hue < 360 ; hue++)
    {

      for (i = timeCount; i < (timeCount + cubesizez); i++)
      {
        z = round(sineOffset * sin(i * omega) + sineOffset); // Calculate z position
        x = i - timeCount;
        for (j = 0; j < cubesizey; j++)
        {
          y = j;
          V = 0.5 +   sin( (2.0 * PI) / (z + 1) );
          cube.HSV(z, x, y, hue , 1, V);
        }
      }
      cube.drawVoxels();
      delay(shiftDelay);
      timeCount++;
      if (timeCount >= period)
        timeCount = 0;
    }//hue
  }//while
}//void

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




void sineWaveTwo(float time, USCL & cube, uint16_t shiftDelay /* = 75 */, uint8_t periodX /* = 16 */,  uint8_t periodY /* = 16 */)
{
  /* Sine wave two parameters
    - time:                 Define the length of time that the animation should be executed.
    - cubeRef:            Define the instance of the 'LedCube' class that you made in your main program.
    - shiftDelay (optional):    Define the delay (in ms) between each iteration of shifting the sine wave to a new position. Normally a value of
                    50 - 100 ms will suffice.
    - periodX (optional):     Period time of the x-axis sine wave . Normally a value of 2 times the cube size will suffice.
    - periodY (optional):     Period time of the y-axis sine wave . Normally a value of 2 times the cube size will suffice.
  */

  // usual  variables
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  float S = 1;
  float V = 1;
  int hue;

  // Voxel coordinates
  uint8_t z;
  uint8_t x;
  uint8_t y;

  // For loop iterators
  uint16_t i;
  uint16_t j;

  // Other variables
  uint8_t timeCountX = 0;
  uint8_t timeCountY = 0;
  float sineOffset = (maxVoxelValue) / 4.0;
  float omegaX = (2.0 * PI) / (float)periodX;
  float omegaY = (2.0 * PI) / (float)periodY;
  float omegaZ = (2.0 * PI) / (float)maxVoxelValue;

  long  startTime = millis();
  while (millis() - startTime < time)
  {

    for (hue = 0; hue < 360 ; hue++)
    {
      for (i = timeCountX; i < (timeCountX + cubesizex); i++)
      {
        x = i - timeCountX;
        for (j = timeCountY; j < (timeCountY + cubesizey); j++)
        {
          y = j - timeCountY;
          z = round(sineOffset * (sin(i * omegaX)  - sin(j * omegaY + PI)) + 2 * sineOffset); // Calculate z position
          // hue = round (180 + max_brightness * sin(sineOffset * (sin( (i * omegaX - j * omegaY)))));

          V = 0.5 +   sin( (2.0 * PI) / (z + 1) );
          cube.HSV(z, x, y, hue , 1, V);
        }
      }


      cube.drawVoxels();
      delay(shiftDelay);


      timeCountX++;
      timeCountY++;
      if (timeCountX >= periodX)
        timeCountX = 0;
      if (timeCountY >= periodY)
        timeCountY = 0;
    }//hue
  }// while
}//void

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




void snake(float time, USCL & cube, int shiftDelay, uint16_t iterations,  unsigned char mode)
{

  /*   snake animation ****NOTE ****

    - time:               Define the length of time that the animation should be executed.
    - cube:               Define the instance of the 'LedCube' class that you made in your main program.
    - shiftDelay :        Define the delay (in ms) . Normally a value of 50 - 100 ms will suffice.
    - iterations:         Define the number of iterations
    - mode :              Define mode (1 - enable ; 0 - disable): bit1 - crash action  ; bit2 - avoid corners
    - drawmode :          Define drawmode : 0 - pixel snake ; 1 - draw snake
  */


  // usual  variables
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  float S = 1;
  float V = 1;


  //  fill(0x00);   // Blank the cube

  int x, y, z;    // Current coordinates for the point
  int dx, dy, dz; // Direction of movement
  int lol, i;   // lol?
  unsigned char crash_x, crash_y, crash_z;

  y = random(cubesizey) % (cubesizey - 1);
  x = random(cubesizex) % (cubesizex - 1);
  z = random(cubesizez) % (cubesizez - 1);

  // Coordinate array for the snake.
  int snake[iterations][3];
  for (i = 0; i < iterations; ++i)
  {
    snake[i][0] = x;
    snake[i][1] = y;
    snake[i][2] = z;
  }
  dx = 1;
  dy = 1;
  dz = 1;

  int hue [iterations];
  int start_hue = random(180);
  int end_hue = 180 + random(180);
  int step_hue = (end_hue - start_hue) / iterations;


  for (i = 0; i < iterations ; i++)
  {
    hue [i] = start_hue + (i * step_hue);
  }


  long  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////




    crash_x = 0;
    crash_y = 0;
    crash_z = 0;


    // Let's mix things up a little:
    if (random(cubesizez) % 3 == 0)
    {
      // Pick a random axis, and set the speed to a random number.
      lol = random(cubesizez) % 3;
      if (lol == 0)
        dx = random(cubesizex) % 3 - 1;

      if (lol == 1)
        dy = random(cubesizey) % 3 - 1;

      if (lol == 2)
        dz = random(cubesizez) % 3 - 1;
    }

    // The point has reached 0 on the x-axis and is trying to go to -1
    // aka a crash
    if (dx == -1 && x == 0)
    {
      crash_x = 0x01;
      if (random(cubesizex) % 3 == 1)
      {
        dx = 1;
      } else
      {
        dx = 0;
      }
    }

    // y axis 0 crash
    if (dy == -1 && y == 0)
    {
      crash_y = 0x01;
      if (random(cubesizey) % 3 == 1)
      {
        dy = 1;
      } else
      {
        dy = 0;
      }
    }

    // z axis 0 crash
    if (dz == -1 && z == 0)
    {
      crash_z = 0x01;
      if (random(cubesizez) % 3 == 1)
      {
        dz = 1;
      } else
      {
        dz = 0;
      }
    }

    // x axis 7 crash
    if (dx == 1 && x == (cubesizex - 1))
    {
      crash_x = 0x01;
      if (random(cubesizex) % 3 == 1)
      {
        dx = -1;
      } else
      {
        dx = 0;
      }
    }

    // y axis 7 crash
    if (dy == 1 && y == (cubesizey - 1))
    {
      crash_y = 0x01;
      if (random(cubesizey) % 3 == 1)
      {
        dy = -1;
      } else
      {
        dy = 0;
      }
    }

    // z azis 7 crash
    if (dz == 1 && z == (cubesizez - 1))
    {
      crash_z = 0x01;
      if (random(cubesizez) % 3 == 1)
      {
        dz = -1;
      } else
      {
        dz = 0;
      }
    }

    // mode bit 0 sets crash action enable
    if (mode | 0x01)
    {
      if (crash_x)
      {
        if (dy == 0)
        {
          if (y == (cubesizey - 1))
          {
            dy = -1;
          } else if (y == 0)
          {
            dy = +1;
          } else
          {
            if (random(cubesizey) % 2 == 0)
            {
              dy = -1;
            } else
            {
              dy = 1;
            }
          }
        }
        if (dz == 0)
        {
          if (z == (cubesizez - 1))
          {
            dz = -1;
          } else if (z == 0)
          {
            dz = 1;
          } else
          {
            if (random(cubesizez) % 2 == 0)
            {
              dz = -1;
            } else
            {
              dz = 1;
            }
          }
        }
      }

      if (crash_y)
      {
        if (dx == 0)
        {
          if (x == (cubesizex - 1))
          {
            dx = -1;
          } else if (x == 0)
          {
            dx = 1;
          } else
          {
            if (random(cubesizex) % 2 == 0)
            {
              dx = -1;
            } else
            {
              dx = 1;
            }
          }
        }
        if (dz == 0)
        {
          if (z == 3)
          {
            dz = -1;
          } else if (z == 0)
          {
            dz = 1;
          } else
          {
            if (random(cubesizez) % 2 == 0)
            {
              dz = -1;
            } else
            {
              dz = 1;
            }
          }
        }
      }

      if (crash_z)
      {
        if (dy == 0)
        {
          if (y == (cubesizey - 1))
          {
            dy = -1;
          } else if (y == 0)
          {
            dy = 1;
          } else
          {
            if (random(cubesizey) % 2 == 0)
            {
              dy = -1;
            } else
            {
              dy = 1;
            }
          }
        }
        if (dx == 0)
        {
          if (x == (cubesizex - 1))
          {
            dx = -1;
          } else if (x == 0)
          {
            dx = 1;
          } else
          {
            if (random(cubesizex) % 2 == 0)
            {
              dx = -1;
            } else
            {
              dx = 1;
            }
          }
        }
      }
    }

    // mode bit 1 sets corner avoid enable
    if (mode | 0x02)
    {
      if (  // We are in one of 8 corner positions
        (x == 0 && y == 0 && z == 0) ||
        (x == 0 && y == 0 && z == (cubesizez - 1)) ||
        (x == 0 && y == (cubesizey - 1) && z == 0) ||
        (x == 0 && y == (cubesizey - 1) && z == (cubesizez - 1)) ||
        (x == (cubesizex - 1) && y == 0 && z == 0) ||
        (x == (cubesizex - 1) && y == 0 && z == (cubesizez - 1)) ||
        (x == (cubesizex - 1) && y == (cubesizey - 1) && z == 0) ||
        (x == (cubesizex - 1) && y == (cubesizey - 1) && z == (cubesizez - 1))
      )
      {
        // At this point, the voxel would bounce
        // back and forth between this corner,
        // and the exact opposite corner
        // We don't want that!

        // So we alter the trajectory a bit,
        // to avoid corner stickyness
        lol = random(cubesizez) % 3;
        if (lol == 0)
          dx = 0;

        if (lol == 1)
          dy = 0;

        if (lol == 2)
          dz = 0;
      }
    }

    // one last sanity check
    if (x == 0 && dx == -1)
      dx = 1;

    if (y == 0 && dy == -1)
      dy = 1;

    if (z == 0 && dz == -1)
      dz = 1;

    if (x == (cubesizex - 1) && dx == 1)
      dx = -1;

    if (y == (cubesizey - 1) && dy == 1)
      dy = -1;

    if (z == (cubesizez - 1) && dz == 1)
      dz = -1;


    // Finally, move the voxel.
    x = x + dx;
    y = y + dy;
    z = z + dz;


    for (i = iterations - 1; i >= 0; --i)
    {
      snake[i][0] = snake[i - 1][0];
      snake[i][1] = snake[i - 1][1];
      snake[i][2] = snake[i - 1][2];
    }
    snake[0][0] = x;
    snake[0][1] = y;
    snake[0][2] = z;

    for (i = 0; i < iterations; ++i)
    {
      cube.HSV(snake[i][2],  snake[i][1], snake[i][0],  hue[i], 1, 1); //(snake[i][0], snake[i][1], snake[i][2]);
    }


    cube.drawVoxels();

    delay(shiftDelay);







  }//while
} // void



// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void plasma3(float time, USCL & cube, int shiftDelay)
{
  /* plasma2 parameters   ***   ***
    - time:                 Define the length of time that the animation should be executed.
    - cubeRef:            Define the instance of the 'LedCube' class that you made in your main program.
      - shiftDelay:     Define the delay (in ms) . Normally a value of 25 - 100 ms will suffice.
  */

  // usual  variables
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  float S = 1;
  float V = 1;

  // Other variables

  float xoffset = 0.1925;//19
  float yoffset = 0.2925;//37
  float zoffset = 0.3925; //24

  float x_max = 120.0 * PI;
  float y_max = 120.0 * PI;
  float z_max = 120.0 * PI;

  float xx_time = 0;
  float yy_time = 0;
  float zz_time = 0;


  float plasma;
  float hue;
  int int_hue;

  float xmap[cubesizex];
  float ymap[cubesizey];
  float zmap[cubesizez];
  for ( int cc = 0 ; cc < cubesizex; cc++)
  {
    xmap[cc] = map(cc, 0, cubesizex - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizex))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  for ( int cc = 0 ; cc < cubesizey; cc++)
  {
    ymap[cc] = map(cc, 0, cubesizey - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizey))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  for ( int cc = 0 ; cc < cubesizez; cc++)
  {
    zmap[cc] = map(cc, 0, cubesizez - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizez))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }


  float fx;
  float fy;
  float fz;
  int color;

  float  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    for ( int x = 0; x < cubesizex ; x++)
    {
      for ( int y = 0 ; y < cubesizey ; y++ )
      {
        for ( int z = 0; z < cubesizez; z++)
        {



          fx =  xmap[x] / 1000.0;
          fx = fx + (xx_time);

          fy = ymap[y] / 1000.0;
          fy = fy + (yy_time);

          fz =  zmap[z] / 1000.0;
          fz =  fz + (zz_time );
          plasma = 0.5 +  0.5 * sin((fy + fz  + fx ) / 3.0 );


          fx =  xmap[x] / 1000.0;
          fx = fx + (xx_time);

          fy = ymap[y] / 1000.0;
          fy = fy + (yy_time);

          fz =  zmap[z] / 1000.0;
          fz =  fz + (zz_time );
          color = int
                  (
                    180.0 + (180.0 * sin((fy - fz  + fx ) / 3.0 ))
                  ) ;


          if (plasma > 0) {
            cube.HSV( z, x, y, color   , 1.0, plasma);//  cube.HSV(z, x, y, 1, plasma * max_brightness);
          }


        }//  z
      }  //  y
    }    //  x


    xx_time = xx_time + xoffset;
    zz_time = zz_time + zoffset;
    yy_time = yy_time + yoffset;

    cube.drawVoxels();
    if (shiftDelay > 0) {
      delay(shiftDelay);
    }

    // hue
    ///////////////////////
  } // while



}// void




// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------





void    wipeout(float time, USCL & cube, unsigned int drawDelay)
{
  /* wipeout parameters
    - time:                     Define the length of time that the animation should be executed.
    - cubeRef:                Define the instance of the 'LedCube' class that you made in your main program.
    - timeDelay (optional):     Define the delay (in ms) between each iteration of the rain drops falling down. Normally a value of
                      50 - 100 ms will suffice.
  */

  // usual  variables
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  float S = 1;
  float V = 1;
  int hue;


  int xxx = 0, yyy = 0, zzz = 0;
  int fx = random(cubesizex), fy = random(cubesizey), fz = random(cubesizez), direct, fxm = 1, fym = 1, fzm = 1, fxo = 0, fyo = 0, fzo = 0;
  int  ftx = random(cubesizex), fty = random(cubesizey), ftz = random(cubesizez), ftxm = 1, ftym = 1, ftzm = 1, ftxo = 0, ftyo = 0, ftzo = 0;
  int select, rr, gg, bb, rrt, ggt, bbt;

  rr = random(0, max_brightness);
  delay(1);
  gg = random(0, max_brightness);
  delay(1);
  bb = random(0, max_brightness);
  delay(1);

  rrt = random(0, max_brightness);
  delay(1);
  ggt = random(0, max_brightness);
  delay(1);
  bbt = random(0, max_brightness);
  delay(1);

  long  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////


    cube.RGB(ftx, fty, ftz,  rr, gg, bb);
    cube.RGB(ftx, fty, ftz + 1, rr, gg, bb);
    cube.RGB(ftx, fty, ftz - 1,  rr, gg, bb);
    cube.RGB(ftx + 1, fty, ftz, rr, gg, bb);
    cube.RGB(ftx - 1, fty, ftz, rr, gg, bb);
    cube.RGB(ftx, fty + 1, ftz, rr, gg, bb);
    cube.RGB(ftx, fty - 1, ftz, rr, gg, bb);

    cube.RGB(fx, fy, fz,  rrt, ggt, bbt);
    cube.RGB(fx, fy, fz + 1,  rrt, ggt, bbt);
    cube.RGB(fx, fy, fz - 1,  rrt, ggt, bbt);
    cube.RGB(fx + 1, fy, fz,  rrt, ggt, bbt);
    cube.RGB(fx - 1, fy, fz,  rrt, ggt, bbt);
    cube.RGB(fx, fy + 1, fz, rrt, ggt, bbt);
    cube.RGB(fx, fy - 1, fz, rrt, ggt, bbt);


    cube.drawVoxels(); // Draw the frame to the cube explicitly using vsync
    delay(drawDelay);

    fxo = fx;
    fyo = fy;
    fzo = fz;

    ftxo = ftx;
    ftyo = fty;
    ftzo = ftz;

    direct = random(3);
    if (direct == 0)
      fx = fx + fxm;
    if (direct == 1)
      fy = fy + fym;
    if (direct == 2)
      fz = fz + fzm;
    if (fx < 0) {
      fx = 0; fxm = 1;
    }
    if (fx > cubesizex - 1) {
      fx = cubesizex - 1; fxm = -1;
    }
    if (fy < 0) {
      fy = 0; fym = 1;
    }
    if (fy > cubesizey - 1) {
      fy = cubesizey - 1; fym = -1;
    }
    if (fz < 0) {
      fz = 0; fzm = 1;
    }
    if (fz > cubesizez - 1) {
      fz = cubesizez - 1; fzm = -1;
    }

    direct = random(3);
    if (direct == 0)
      ftx = ftx + ftxm;
    if (direct == 1)
      fty = fty + ftym;
    if (direct == 2)
      ftz = ftz + ftzm;
    if (ftx < 0) {
      ftx = 0; ftxm = 1;
    }
    if (ftx > cubesizex - 1) {
      ftx = cubesizex - 1; ftxm = -1;
    }
    if (fty < 0) {
      fty = 0; ftym = 1;
    }
    if (fty > cubesizey - 1) {
      fty = cubesizey - 1; ftym = -1;
    }
    if (ftz < 0) {
      ftz = 0; ftzm = 1;
    }
    if (ftz > cubesizez - 1) {
      ftz = cubesizez - 1; ftzm = -1;
    }






  }//while

}//wipeout







// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void MY_4D_function2(float time, USCL & cube, int shiftDelay)
{
  /* MY_4D_function1 parameters   ***   ***
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'LedCube' class that you made in your main program.
      - shiftDelay:     Define the delay (in ms) . Normally a value of 25 - 100 ms will suffice.
  */

  // usual  variables
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  float S = 1;
  float V = 1;
  int hue;
  float plasma;
  float ipsilon ;

  // Other variables




  float  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////
    for ( float offset = 0; offset < 4 * PI ; offset = offset +  0.1 ) {

      //   hue = 180 + 180 * (sin(offset / 4.0));

      for ( byte x = 0; x < cubesizex; x++)
      {
        for ( byte y = 0; y < cubesizey; y++)
        {
          for ( byte z = 0; z < cubesizez; z++)
          {
            //ipsilon = 10000 *  sin((offset *sin(offset+ sqrt(sqrt(x*x + y*z*x + z*z))/(4*cubesize)   ))/(1*cubesize) );
            ipsilon = 10000 *  sin((offset + sin((x * y - z) / (2 * PI))));
            plasma   = map(ipsilon, -10000, 10000, 0, 10000);
            ipsilon = 10000 *  sin(offset + sin(offset * sin((x * y - z) / (2 * PI)   ) ));
            hue =  map(ipsilon, -10000, 10000, 0, 360);
            if (plasma > 0) {
              cube.HSV(z, x, y, hue, 1, float(plasma / 10000.00));
            }
          }
        }
      }
      cube.drawVoxels();
      if (shiftDelay > 0) {
        delay(shiftDelay);
      }

    } // offset
    ///////////////////////
  } // while



}// void




// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void MY_4D_function1 (float time, USCL & cube, uint16_t shiftDelay)
{
  //
  /* MY_4D_function1 cube parameters
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'LedCube' class that you made in your main program.
    - shiftDelay (optional):    Define the delay (in ms) . Normally a value of 50 - 100 ms will suffice.
    -function:
  */
  // usual  variables
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  float S = 1;
  float V = 1;
  float hue = 180;
  int int_hue;

  float  hue_min = 0;
  float hue_max = 343.47;





  float  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    for (   float a = -4 * cubesizez; a < 4 * 2 * cubesizez; a = a + 0.25)
    {
      for ( int x = 0; x <  cubesizex ; x++)
      {

        for ( int y = 0; y < cubesizey; y++)
        {

          for ( int z = 0 ; z <  cubesizez ; z++ )
          {

            // hue = (x + a) * 2.0 - ( (y + a) / 2.0 ) + (((z + a) * 3.) / 2.0);
            hue = (x + a) * (x + a) + (y + a) * (y + a) + (z + a) * (z + a); // min : 0  ; max : 343.47

            int_hue =     map(int(hue), int(hue_min), int(hue_max), 0, 360);
            //        int_hue = int(hue);
            //            Serial.println(int_hue);
            cube.HSV(z, x, y, int_hue , 1, 1 );

          }//  z
        }  //  y
      }    //  x


      cube.drawVoxels();

      if (shiftDelay > 0) {
        delay(shiftDelay);
      }
    } // a
    ///////////////////////




  } // While
}// void

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void sinx2y2(float time, USCL & cube, uint16_t shiftDelay /* = 75 */)
{
  /* sinx2y2 cube parameters
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'LedCube' class that you made in your main program.
    - shiftDelay (optional):    Define the delay (in ms) . Normally a value of
                    50 - 100 ms will suffice.
  */

  // usual  variables
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  byte maxVoxelValue = cubesizez - 1;
  float S = 1;
  float V = 1;
  int hue = 180;
  // Declare and initialize other variables needed for the animation
  float omega = (2.0 * PI) / (2 * cubesizez); // Calculate the radial frequency omega


  // Other variables


  long  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////


    for ( float offset = 0; offset < 4 * PI ; offset = offset +  0.18 ) {

      hue = 180 + 180 * (sin(PI * sin(offset / 4.0)));

      for ( byte x = 0; x < cubesizex; x++) {
        for ( byte y = 0; y < cubesizey; y++)
        {
          float      ipsilon = 10000 *  sin(offset + sqrt(pow(map(x, 0, cubesizex - 1, -PI, PI), 2) + pow(map(y, 0, cubesizey - 1, -PI, PI), 2)));
          // y=sin(z * omega + offset ) * cos( x * omega + offset  );
          float     z   = map(ipsilon, -10000, 10000, 0, cubesizez);

          float      br = 0.5 + 0.5 * sin(x * omega + offset / 0.55  );



          cube.HSV(z, x, y, hue, 1, br);
        }
      }
      cube.drawVoxels();
      if (shiftDelay > 0) {
        delay(shiftDelay);
      }
    } // offset

    ///////////////////////
  } // while
}// void


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



void slantingLines(float time, USCL & cube, uint16_t shiftDelay /* = 75 */, bool invertDirection /* = false */)
{
  /* Slanting lines parameters
    - time:                 Define the length of time that the animation should be executed.
    - cube:                Define the instance of the 'LedCube' class that you made in your main program.
    - shiftDelay (optional):        Define the delay (in ms) between each iteration of shifting the lines to a new position. Normally a value of
                      50 - 100 ms will suffice.
    - invertDirection (optional): With this value you can invert the direction which way the slanting lines are rotating.
  */

  // usual  variables
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  uint8_t maxVoxelValue = cube.getCubeSizeZ() - 1;
  float S = 1;
  float V = 1;
  int hue = 180;
  // Voxel arrays
  uint8_t xVoxel[cubesizez];
  uint8_t yVoxel[cubesizez];

  // Voxel coordinates
  uint8_t z;
  uint8_t x;
  uint8_t y;

  // For loop iterators
  uint8_t i;

  // Other variables


  // Initialize arrays
  for (i = 0; i < cubesizez; i++)
  {
    xVoxel[i] = i;
    yVoxel[i] = 0;
  }

  long  startTime = millis();
  while (millis() - startTime < time)
  {


    for (hue = 0; hue < 360; hue++)
    {
      // Display voxel data
      for (i = 0; i < cubesizez; i++)
      {
        // Normal line
        z = i;
        x = xVoxel[i];
        y = yVoxel[i];
        if (invertDirection)
          cube.HSV(z, y, x, hue, 1, 1);
        else
          cube.HSV(z, x, y, hue, 1, 1);

        // Opposite line
        z = maxVoxelValue - i;
        x = maxVoxelValue - xVoxel[maxVoxelValue - i];
        y = maxVoxelValue - yVoxel[maxVoxelValue - i];
        if (invertDirection)
          cube.HSV(z, y, x, hue, 1, 1);
        else
          cube.HSV(z, x, y, hue, 1, 1);
      }
      cube.drawVoxels();

      // Calculate new voxels
      for (i = 0; i < cubesizez; i++)
      {
        if (xVoxel[i] < maxVoxelValue && yVoxel[i] == maxVoxelValue)
          xVoxel[i] += 1;
        else if (xVoxel[i] == maxVoxelValue && yVoxel[i] > 0)
          yVoxel[i] -= 1;
        else if (xVoxel[i] > 0 && yVoxel[i] == 0)
          xVoxel[i] -= 1;
        else if (xVoxel[i] == 0 && yVoxel[i] < maxVoxelValue)
          yVoxel[i] += 1;
      }
      delay(shiftDelay);

    } // hue
  } //  while

} // void

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



void fireworks(float time, USCL & cube,  int n, int shiftDelay)
{
  /*   fireworks animation

    - time:               Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'LedCube' class that you made in your main program.
    - iterations:         Define the number of iterations
    - n         :         Define number of particles
    - shiftDelay :        Define the delay (in ms) . Normally a value of 50 - 100 ms will suffice.
  */
  // usual  variables
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  byte maxVoxelValue = cubesizez - 1;
  float S = 1;
  float V = 1;
  int hue = 180;

  int i, f, e;

  int z, x, y;

  float origin_x = cubesizex / 2;
  float origin_y = cubesizey / 2;
  float origin_z = 0;

  int rand_y, rand_x, rand_z;

  float slowrate, gravity;

  // Particles and their position, x,y,z and their movement, dx, dy, dz
  float particles[n][6];

  long  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////


    hue = random(360) ;


    origin_x = random(cubesizex) ;
    origin_y = random(cubesizey) ;
    origin_z = random(cubesizez) ;


    // shoot a particle up in the air
    for (e = 0; e < cubesizez; ++e)
    {
      cube.HSV( e, origin_x, origin_y, hue, 1, 1);
      //      setvoxel(origin_x,origin_y,e);

      cube.drawVoxels();
      delay(shiftDelay);

    }

    // Fill particle array
    for (f = 0; f < n; ++f)
    {
      // Position
      particles[f][0] = origin_x;
      particles[f][1] = origin_y;
      particles[f][2] = maxVoxelValue;   // origin_z;

      rand_x = random(256) % 200;
      rand_y = random(256) % 200;
      rand_z = random(256) % 200;

      // Movement
      particles[f][3] = 1.0 - (float)rand_x / 100; // dx
      particles[f][4] = 1.0 - (float)rand_y / 100; // dy
      particles[f][5] = 1.0 - (float)rand_z / 100; // dz
    }

    // explode
    for (e = 0; e < 25; ++e)
    {
      slowrate = 1 + tan((e + 0.1) / 20.0) * 25.0;

      gravity = tan((e + 0.1) / 20.0) / 2.0;

      for (f = 0; f < n; ++f)
      {
        particles[f][0] += particles[f][3] / slowrate;
        particles[f][1] += particles[f][4] / slowrate;
        particles[f][2] += particles[f][5] / slowrate;
        particles[f][2] -= gravity;


        z = particles[f][2];
        x = particles[f][0];
        y = particles[f][1];

        if (x < 0)
        {
          x = 0;
        }
        if (y < 0)
        {
          y = 0;
        }

        if (x > cubesizex - 1)
        {
          x = cubesizex - 1;
        }
        if (y > cubesizey - 1)
        {
          y = cubesizey - 1;
        }


        if (z > maxVoxelValue)
        {
          z = maxVoxelValue;
        }
        V = float(1.0 / 10.0 + (z * maxVoxelValue / 10.0) );

        if (z >= 0)
        {

          // cube.HSV(particles[f][2], particles[f][0], particles[f][1],   hue, 1.0, float( (max_brightness/(cubesize/2))+ max_brightness *    particles[f][2] / (cubesize/2) )   );

          cube.HSV(z, x, y,   hue, 1.0 , V);

        } // z

      } //

      cube.drawVoxels();
      delay(shiftDelay);

    } //e

  } //while



}//void

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void cornerCube(float time, USCL & cube, uint16_t resizeDelay /* =75 */, uint8_t minimumCubeSize /* = 2 */)
{

  /* Corner cube parameters
    - time:                       Define the length of time that the animation should be executed.
    - cubeRef:                    Define the instance of the 'LedCube' class that you made in your main program.
    - resizeDelay (optional):       Define the delay (in ms) between each iteration of changing the cubes to a new size. Normally a value of
                          50 - 100 ms will suffice.
    - minimumCubeSize (optional):   Define the minimum size the cube will shrink to. Values smaller than '0' or bigger than the maximum cube size will be omitted.
  */


  // Voxel offset
  uint8_t zOffset;
  uint8_t xOffset;
  uint8_t yOffset;

  // For loop iterators
  uint8_t i;
  uint8_t j;

  // Other variables
  uint8_t maxVoxelValue = cube.getCubeSizeZ() - 1;
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  uint8_t cubeSize = cube.getCubeSizeZ() - 1;
  int max_brightness = cube.getMaxBrightness();
  uint8_t cornerSelectBits;
  int8_t increment;
  uint8_t resizeCount;

  int hue = 0;

  // Restrict input values
  if (minimumCubeSize == 0)
    resizeCount = (cubesizez - 2) * 2;
  else if (minimumCubeSize < cubesizez)
    resizeCount = (cubesizez - minimumCubeSize) * 2;
  else
    resizeCount = (cubesizez - maxVoxelValue) * 2;

  long  startTime = millis();
  while (millis() - startTime < time)
  {
    increment = -1; // Shrink the cube
    uint8_t selectBits = random(8);
    while (selectBits == cornerSelectBits) {
      selectBits = random(8);
    }; // Ensure the new select bits do not equal the previous corner select bits.
    cornerSelectBits = selectBits;
    zOffset = 0;
    xOffset = 0;
    yOffset = 0;
    for (i = 0; i < resizeCount; i++)
    {
      for (j = 0; j <= cubeSize; j++)
      {
        // Lower layer
        cube.HSV(0 + zOffset, 0 + xOffset, j + yOffset, hue + j, 1, 1);
        cube.HSV(0 + zOffset, j + xOffset, 0 + yOffset, hue + j, 1, 1);
        cube.HSV(0 + zOffset, cubeSize + xOffset, j + yOffset, hue + j, 1, 1);
        cube.HSV(0 + zOffset, j + xOffset, cubeSize + yOffset, hue + j, 1, 1);

        // Upper layer
        cube.HSV(cubeSize + zOffset, 0 + xOffset, j + yOffset, hue + j, 1, 1);
        cube.HSV(cubeSize + zOffset, j + xOffset, 0 + yOffset, hue + j, 1, 1);
        cube.HSV(cubeSize + zOffset, cubeSize + xOffset, j + yOffset, hue + j, 1, 1);
        cube.HSV(cubeSize + zOffset, j + xOffset, cubeSize + yOffset, hue + j, 1, 1);

        // Inner layers
        cube.HSV(j + zOffset, 0 + xOffset, 0 + yOffset, hue + j, 1, 1);
        cube.HSV(j + zOffset, 0 + xOffset, cubeSize + yOffset, hue + j, 1, 1);
        cube.HSV(j + zOffset, cubeSize + xOffset, 0 + yOffset, hue + j, 1, 1);
        cube.HSV(j + zOffset, cubeSize + xOffset, cubeSize + yOffset, hue + j, 1, 1);
      }
      cube.drawVoxels();
      delay(resizeDelay);

      // Make increment positive when cube is at its minimum selected size
      if (i == (resizeCount / 2))
        increment = 1;
      cubeSize += increment;

      hue = hue + 1;
      if (hue > 360 ) {
        hue = 0;
      }

      // Calculate needed offset according to the corner select bits (LSB numbering)
      // bit 0: offset for z position
      // bit 1: offset for x position
      // bit 2: offset for y position
      // bit 3...7: not used
      zOffset = (maxVoxelValue - cubeSize) * (cornerSelectBits & 1); // Right shift not needed
      xOffset = (maxVoxelValue - cubeSize) * ((cornerSelectBits & 2) >> 1); // Right shift bit to use it as '1' or '0' multiplier
      yOffset = (maxVoxelValue - cubeSize) * ((cornerSelectBits & 4 ) >> 2);  // Right shift bit to use it as '1' or '0' multiplier
    }
  }
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void plasma2(float time, USCL & cube, int shiftDelay)
{
  /* plasma2 parameters   ***   ***
    - time:                 Define the length of time that the animation should be executed.
    - cubeRef:            Define the instance of the 'LedCube' class that you made in your main program.
      - shiftDelay:     Define the delay (in ms) . Normally a value of 25 - 100 ms will suffice.
  */

  // usual  variables
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  float S = 1;
  float V = 1;

  // Other variables

  float xoffset = PI / (25 + random(90));//19
  float yoffset = PI / (25 + random(90));//37
  float zoffset = PI / (25 + random(90)); //24

  float x_max = 120.0 * PI;
  float y_max = 120.0 * PI;
  float z_max = 120.0 * PI;

  float xx_time = 0;
  float yy_time = 0;
  float zz_time = 0;


  float plasma;
  float hue;
  int int_hue;



  float xmap[cubesizex];
  float ymap[cubesizey];
  float zmap[cubesizez];
  for ( int cc = 0 ; cc < cubesizex; cc++)
  {
    xmap[cc] = map(cc, 0, cubesizex - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizex))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  for ( int cc = 0 ; cc < cubesizey; cc++)
  {
    ymap[cc] = map(cc, 0, cubesizey - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizey))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  for ( int cc = 0 ; cc < cubesizez; cc++)
  {
    zmap[cc] = map(cc, 0, cubesizez - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizez))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }



  float fx;
  float fy;
  float fz;

  float  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    for ( int x = 0; x < cubesizex ; x++)
    {
      for ( int y = 0 ; y < cubesizey ; y++ )
      {
        for ( int z = 0; z < cubesizez; z++)
        {


          fx =  xmap[x] / 1000.0;
          fx = fx * (xx_time);

          fy = ymap[y] / 1000.0;
          fy = fy * (yy_time);

          fz =  zmap[z] / 1000.0;
          fz =  fz * (zz_time );
          plasma =  sin((fy + fz  + fx ) / 3.0 );

          hue = 10000 * cos((fy + fz + fx) / 3);
          int_hue = map(hue, -10000, 10000, 0, 360);

          if (plasma > 0) {
            cube.HSV( z, x, y, int_hue   , 1.0, plasma);//  cube.HSV(z, x, y, 1, plasma * max_brightness);
          }


        }//  z
      }  //  y
    }    //  x


    xx_time = xx_time + xoffset;
    if (xx_time > x_max) {
      xoffset = float(PI / (25 + random(90)));
      xx_time = 0.0;
    }
    zz_time = zz_time + zoffset;
    if (zz_time > z_max) {
      zoffset = float( PI / (25 + random(90)));
      zz_time = 0.0;
    }
    yy_time = yy_time + yoffset;
    if (yy_time > y_max) {
      yoffset =  float(PI / (25 + random(90)));
      yy_time = 0.0;
    }

    cube.drawVoxels();
    if (shiftDelay > 0) {
      delay(shiftDelay);
    }

    // hue
    ///////////////////////
  } // while



}// void




// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



void rain(float time, USCL & cube, int rainVoxels /* = 2 */, int floorVoxels /* =4  */, int rainDelay /* = fps */, bool invertDirection /* = false */)
{
  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


  /* Rain parameters
    - time:                     Define the length of time that the animation should be executed.
    - cube:                Define the instance of the 'USCL' class that you made in your main program.
    - rainVoxels:               Define the number of voxels that should be drawn onto the top layer each iteration (rain density).
    - floorVoxels (optional):       With this value you can define how long a voxels should be remembered when hitting the last layer of
                      the cube. For example: make this value '5', whenever a voxel hits the last layer it will then stay lit
                      for another five iterations. Make this value '0' to turn this option off.
    - rainDelay (optional):     Define the delay (in ms) between each iteration of the rain drops falling down. Normally a value of 25 - 100 ms will suffice.
    - invertDirection (optional): With this value you can invert the direction which way the rain drops 'fall'.
  */

  // usual  variables
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  byte maxVoxelValue = cubesizez - 1;
  float S = 1;
  float V = 1;
  // Voxel arrays
  int xVoxel  [rainVoxels + 1];
  int yVoxel  [rainVoxels + 1];
  int zVoxel [rainVoxels + 1];
  int xVoxelFloor[rainVoxels + 1];
  int yVoxelFloor[rainVoxels + 1];
  int countFloor[rainVoxels + 1];
  int hueRain[rainVoxels + 1];
  //  int8_t hueFloor [floorVoxels + 1];

  // Voxel coordinates
  uint8_t z;
  int8_t x;
  int8_t y;

  // For loop iterators
  int i;
  int j;



  // Initialize rain drops values

  for (j = 0; j < rainVoxels; j++)
  {
    xVoxel[j] = random(cubesizex);
    yVoxel[j] = random(cubesizey);
    zVoxel[j] = (cubesizez - 1);
    hueRain[j] = random(360);
    countFloor[j] = 1 + random(1 + floorVoxels);
  }




  long  startTime = millis();
  while (millis() - startTime < time)
  {


    // draw rain drops
    for (j = 0; j < rainVoxels; j++)
    {

      if (zVoxel[j] >= 1)
      { //  still droping
        V = 1.00;
      }
      if (zVoxel[j] == 0)
      { //  it's on floor
        V = float( float(countFloor[j]) / float(floorVoxels) );
      }


      if (invertDirection == true)
      {
        cube.HSV( (cubesizez - 1) - zVoxel[j], xVoxel[j] , yVoxel[j], hueRain[j], S, V);
      }
      if (invertDirection == false)
      {
        cube.HSV(  zVoxel[j], xVoxel[j] , yVoxel[j], hueRain[j], S, V);
      }
    }
    cube.drawVoxels();
    delay(rainDelay);

    // calculate drops

    for (j = 0; j < rainVoxels; j++)
    {
      if (zVoxel[j] >= 1)
      {
        zVoxel[j] = zVoxel[j] - 1;
      }
      else
      {
        if ( countFloor[j] == 0)
        {
          // time for new drop
          xVoxel[j] = random(cubesizex);
          yVoxel[j] = random(cubesizey);
          zVoxel[j] = (cubesizez - 1);
          hueRain[j] = random(360);
          countFloor[j] = 1 + random(1 + floorVoxels);
        }
        else
        {
          // decrease floor count
          countFloor[j] = countFloor[j] - 1;
        }
      }
    }












  } // while

} // loop

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


/* explodingimplodingsphere parameters   ***   ***
  - time:                 Define the length of time that the animation should be executed.
  - cube:            Define the instance of the 'USCL' class that you made in your main program.
  - shiftDelay :        Define the delay (in ms) . Normally a value of
                  50 - 100 ms will suffice.
*/
void explodingimplodingsphere(float time, USCL & cube, uint16_t shiftDelay /* = 75 */)
{
  // Declare and initialize other variables needed for the animation
  // float omega = (2.0 * PI) / (2 * cube.getCubeSize()); // Calculate the radial frequency omega
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  int hue;
  float S = 1;
  float V = 1;
  float radspeed = 0.20;

  float radius = cubesizez / 2;
  float radius2;
  float distance;

  float center_x = (cubesizex / 2) - 1;
  float center_y = (cubesizey / 2) - 1;
  float center_z = (cubesizez / 2) - 1;

  float x1, y1, z1;
  float czzp, cyyp, cxxp, czzn, cyyn, cxxn;
  // Other variables
  float br;


  long  startTime = millis();
  while (millis() - startTime < time)
  {

    ///////////////////
    center_x = (cubesizex / 4.0) + random(cubesizex / 2.0);
    center_y = (cubesizey / 4.0) + random(cubesizey / 2.0);
    center_z = (cubesizez / 4.0) + random(cubesizez / 2.0);



    for (  radius = 0; radius <=  cubesizez - 1  ; radius = radius + radspeed)
    {
      radius2 = radius * radius;
      for ( float x = 0; x < cubesizex ; x++)
      {
        x1 = x * x;
        for ( float y = 0; y < cubesizey ; y++)
        {
          y1 = x1 + (y * y);
          for ( float z = 0; z < cubesizez ; z++)
          {

            distance =  y1 + (z * z)  ;
            if (distance < radius2 )
            {
              br =  (max_brightness / (radius + 1)) * cubesizez;


              czzp = center_z + z;
              cyyp = center_y + y;
              cxxp = center_x + x;
              czzn = center_z - z;
              cyyn = center_y - y;
              cxxn = center_x - x;
              /*
                            if (czzn < 0) {
                              czzn = 0;
                            }
                            if (cyyn < 0) {
                              cyyn = 0;
                            }
                            if (cxxn < 0) {
                              cxxn = 0;
                            }

                            if (czzp > cubesize - 1) {
                              czzp = cubesize - 1;
                            }
                            if (cyyp > cubesize - 1) {
                              cyyp = cubesize - 1;
                            }
                            if (cxxp > cubesize - 1) {
                              cxxp = cubesize - 1;
                            }
              */

              cube.HSV(czzp, cyyp, cxxp , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzp, cyyp, cxxn , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzp, cyyn, cxxp, int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzp, cyyn, cxxn , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzn, cyyp, cxxp , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzn, cyyp, cxxn , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzn, cyyn, cxxp , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzn, cyyn, cxxn , int (360 / (radius2 + 1)), 1, br);


            }


          }
        }
      }
      cube.drawVoxels();
      delay(shiftDelay);
    }



    for (  radius = cubesizez - 1; radius >=  0  ; radius = radius - radspeed )
    {
      radius2 = radius * radius;
      for ( float x = 0; x < cubesizex ; x++)
      {
        x1 = x * x;
        for ( float y = 0; y < cubesizey ; y++)
        {
          y1 = x1 + (y * y);
          for ( float z = 0; z < cubesizez ; z++)
          {

            distance =  y1 + (z * z)  ;
            if (distance < radius2 )
            {
              br = 15 - radius * 2;
              czzp = center_z + z;
              cyyp = center_y + y;
              cxxp = center_x + x;
              czzn = center_z - z;
              cyyn = center_y - y;
              cxxn = center_x - x;

              /*
                            if (czzn < 0) {
                              czzn = 0;
                            }
                            if (cyyn < 0) {
                              cyyn = 0;
                            }
                            if (cxxn < 0) {
                              cxxn = 0;
                            }

                            if (czzp > cubesize - 1) {
                              czzp = cubesize - 1;
                            }
                            if (cyyp > cubesize - 1) {
                              cyyp = cubesize - 1;
                            }
                            if (cxxp > cubesize - 1) {
                              cxxp = cubesize - 1;
                            }
              */
              cube.HSV(czzp, cyyp, cxxp , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzp, cyyp, cxxn , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzp, cyyn, cxxp, int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzp, cyyn, cxxn ,  int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzn, cyyp, cxxp , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzn, cyyp, cxxn ,  int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzn, cyyn, cxxp , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzn, cyyn, cxxn ,  int (360 / (radius2 + 1)), 1, br);

            }


          }
        }
      }
      cube.drawVoxels();
      delay(shiftDelay);
    }



    ///////////////////////
  } // while
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




void explodingsphere(float time, USCL & cube, uint16_t shiftDelay /* = 75 */)
{
  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // Declare and initialize other variables needed for the animation
  // float omega = (2.0 * PI) / (2 * cube.getCubeSize()); // Calculate the radial frequency omega
  /* explodingsphere parameters   ***   ***
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'USCL' class that you made in your main program.
    - shiftDelay :        Define the delay (in ms) . Normally a value of
                    50 - 100 ms will suffice.
  */
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  int hue;
  float S = 1;
  float V = 1;
  float radspeed = 1;
  float radius = cubesizez / 2;
  float radius2;
  float distance;

  int center_x = (cubesizex / 2) - 1;
  int center_y = (cubesizey / 2) - 1;
  int center_z = (cubesizez / 2) - 1;

  //float x, y, z;
  float x1, y1, z1;
  float x2, y2, z2;
  // Other variables
  int br;
  int M = 8;
  int N = 8;
  int n, m;
  int x, y, z;


  long  startTime = millis();
  while (millis() - startTime < time)
  {

    int hue = random(360);
    ///////////////////
    center_x = (cubesizex / 4) + random(cubesizex / 2);
    center_y = (cubesizey / 4) + random(cubesizey / 2);
    center_z = (cubesizez / 4) + random(cubesizez / 2);



    for (  radius = 0; radius <  cubesizez   ; radius = radius + radspeed)
    {

      for (  m = 1; m < M ; m++)
      {

        for (  n = 1; n < N ; n++)
        {

          x = radius * (sin(PI * m / M) * cos(2 * PI * n / N));

          y = radius * (sin(PI * m / M) * sin(2 * PI * n / N));

          z = radius * cos(PI * m / M);

          //     (x, y, z) = (sin(Pi * m / M) cos(2Pi * n / N), sin(Pi * m / M) sin(2Pi * n / N), cos(Pi * m / M));

          cube.HSV(center_z + z, center_y + y, center_z + x , hue, S, V);





        }
      }
      cube.drawVoxels();
      delay(shiftDelay);

    }
    //    cube.drawVoxels();
    //   delay(shiftDelay);





    ///////////////////////
  } // while

}


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void folder(float time, USCL & cube, uint16_t shiftDelay)
{

  /* folder parameters   ***   ***
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'USCL' class that you made in your main program.
    - shiftDelay :        Define the delay (in ms) . Normally a value of 20 - 100 ms will suffice.

  */
  // Declare and initialize other variables needed for the animation


  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  int hue = random(360);
  float S = 1;
  float V = float(float(random(max_brightness + 1)) / float(max_brightness)); //float V=float(random(max_brightness+1)/(1+max_brightness)  );
  // Other variables
  byte select;


  float  startTime = millis();
  while (millis() - startTime < time)
  {

    //    cube.HSV(z, y, x, 1, bightness);
    ///////////////////

    select = random(8);



    //////////////////////////////////////////// FrontZ to FrontX
    hue =  random(360);
    V = float(float(1.0 + random(max_brightness )) / float(max_brightness)); //float V=float(random(max_brightness+1)/(1+max_brightness)  );
    for (byte a = 0; a < cubesizey - 1; a++)
    {
      for (byte z = 0; z < cubesizex; z++)
      {
        cube.HSV_line(0, 0, z,  a, cubesizey - 1, z, hue, S, V );

      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    for (byte a = 0; a < cubesizey; a++)
    {
      for (byte z = 0; z < cubesizey; z++)
      {
        cube.HSV_line(0, 0, z,  cubesizez - 1, cubesizey - 1 - a, z, hue, S, V);
      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    ///////////////////////////////////////////



    //////////////////////////////////////////// FrontX to BackZ
    hue = random(360 );
    V = float(float(1.0 + random(max_brightness )) / float(max_brightness)); //float V=float(random(max_brightness+1)/(1+max_brightness)  );
    for (byte a = 0; a < cubesizex - 1; a++)
    {
      for (byte z = 0; z < cubesizey; z++)
      {
        cube.HSV_line( cubesizez - 1 , 0, z , 0, a , z, hue, S, V);

      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    for (byte a = 0; a < cubesizez; a++)
    {
      for (byte z = 0; z < cubesizey; z++)
      {
        cube.HSV_line((cubesizez - 1), 0, z,  a, cubesizey - 1 , z, hue, S, V);
      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    ///////////////////////////////////////////


    //////////////////////////////////////////// BackZ to BackY
    hue =  random(360 );
    V = float(float(1.0 + random(max_brightness )) / float(max_brightness)); //float V=float(random(max_brightness+1)/(1+max_brightness)  );
    for (byte a = 0; a < cubesizez - 1; a++)
    {
      for (byte z = 0; z < cubesizex; z++)
      {
        cube.HSV_line(cubesizez - 1, z, cubesizey - 1, cubesizez - 1 - a, z , 0, hue, S, V);

      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    for (byte a = 0; a < cubesizey; a++)
    {
      for (byte z = 0; z < cubesizex; z++)
      {
        cube.HSV_line(cubesizez - 1, z, cubesizey - 1, 0, z , a, hue, S, V);
      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    ///////////////////////////////////////////


    //////////////////////////////////////////// BackY to BackX
    hue =  random(360);
    V = float(float(1.0 + random(max_brightness )) / float(max_brightness)); //float V=float(random(max_brightness+1)/(1+max_brightness)  );
    for (byte a = 0; a < cubesizey - 1; a++)
    {
      for (byte z = 0; z < cubesizez; z++)
      {
        cube.HSV_line(z, cubesizex - 1, cubesizey - 1, z, 0 , cubesizey - 1 - a, hue, S, V);

      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    for (byte a = 0; a < cubesizex; a++)
    {
      for (byte z = 0; z < cubesizez; z++)
      {
        cube.HSV_line(z, cubesizex - 1, cubesizey - 1, z, a , 0, hue, S, V);
      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    ///////////////////////////////////////////

    //////////////////////////////////////////// BackX to FrontY
    hue = random(360 );
    V = float(float(1.0 + random(max_brightness )) / float(max_brightness)); //float V=float(random(max_brightness+1)/(1+max_brightness)  );
    for (byte a = 0; a < cubesizex - 1; a++)
    {
      for (byte z = 0; z < cubesizez; z++)
      {
        cube.HSV_line(z, cubesizex - 1, 0, z, cubesizex - 1 - a , cubesizey - 1, hue, S, V);

      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    for (byte a = 0; a < cubesizey; a++)
    {
      for (byte z = 0; z < cubesizez; z++)
      {
        cube.HSV_line(z, cubesizex - 1, 0, z, 0 , cubesizey - 1 - a, hue, S, V);
      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    ///////////////////////////////////////////


    //////////////////////////////////////////// FrontWallY to FrontWallX
    hue =  random(360 );
    V = float(float(1.0 + random(max_brightness )) / float(max_brightness)); //float V=float(random(max_brightness+1)/(1+max_brightness)  );
    for (byte a = 0; a < cubesizey - 1; a++)
    {
      for (byte z = 0; z < cubesizez; z++)
      {
        cube.HSV_line(z, 0, 0,  z, cubesizex - 1, a, hue, S, V);
      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    for (byte a = 0; a < cubesizex; a++)
    {
      for (byte z = 0; z < cubesizez; z++)
      {
        cube.HSV_line(z, 0, 0,  z, cubesizex - 1 - a, cubesizey - 1, hue, S, V);
      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    ///////////////////////////////////////////

    //////////////////////////////////////////// FrontX to FrontZ
    hue =  random(360 );
    V = float(float(1.0 + random(max_brightness )) / float(max_brightness)); //float V=float(random(max_brightness+1)/(1+max_brightness)  );
    for (byte a = 0; a < cubesizex - 1; a++)
    {
      for (byte z = 0; z < cubesizey; z++)
      {
        cube.HSV_line(0, 0, z,  cubesizez - 1, a, z, hue, S, V );

      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    for (byte a = 0; a < cubesizez; a++)
    {
      for (byte z = 0; z < cubesizey; z++)
      {
        cube.HSV_line(0, 0, z,  cubesizez - 1 - a, cubesizex - 1 , z, hue, S, V);
      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    ///////////////////////////////////////////



    //    delay(shiftDelay);
    ///////////////////////
  } // while



}// void folder


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void starfield(float time, USCL & cube, uint8_t stars,  uint16_t shiftDelay)
{
  /* starfield parameters
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'USCL' class that you made in your main program.
    - numberOfVoxels:  Define the number of voxels that should be drawn each frame.
    - shiftDelay (optional):    Define the delay (in ms) . Normally a value of
                  50 - 100 ms will suffice.
  */
  // Declare and initialize other variables needed for the animation
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  float star_x [stars];
  float star_y [stars];
  float star_z [stars];
  float star_speed[stars];
  int star_hue[stars];

  int i;
  int trail;
  // Other variables
  for (i = 0; i < stars ; i++) {
    star_x [i] = random(cubesizex);
    star_y [i] = cubesizey + random(cubesizey); // field move along Y axis
    star_z [i] = random(cubesizez);
    star_speed[i] = 0.3 + (float((random(150) / 150.0))); // speed of individual stars
    star_hue[i] = random(360); // random color
  }


  float  startTime = millis();
  while (millis() - startTime < time)
  {

    //    cube.HSV(z, y, x,hue, 1, brightness);
    ///////////////////

    // display stars:
    for (i = 0 ; i < stars; i++)
    {
      for ( trail = 0 ; trail <= (cubesizey ); trail++)
      {

        if (star_y[i] + trail < cubesizey)
        {
          cube.HSV(star_z[i], star_x[i], star_y[i] + trail,  star_hue[i] , 1.0, float((max_brightness - (max_brightness / (1.0 + trail)  )) / float(max_brightness) )  );
        }
      }// trail
    }// i

    delay(shiftDelay);
    cube.drawVoxels();

    // star calculations:

    for (i = 0; i < stars; i++) {

      star_y[i] = star_y[i] - star_speed[i];

      if (star_y[i] <= -cubesizey)
      { // star has "passed", calculate new star position

        star_x [i] = random(cubesizex);
        star_y [i] = cubesizey + random(cubesizey); // field move along Y axis
        star_z [i] = random(cubesizez);
        star_speed[i] = float(0.05) + (float((random(float(10)) / float(10)))); // speed of individual stars
        star_hue[i] = random(360);
      }



    } // i stars













    //    delay(shiftDelay);
    ///////////////////////
  } // while


} // void

void plasma1(float time, USCL & cube, uint16_t shiftDelay)
{
  /* plasma1 parameters   ***   ***
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'USCL' class that you made in your main program.
    - shiftDelay :        Define the delay (in ms) . Normally a value of  0 - 100 ms will suffice.
  */
  // Declare and initialize other variables needed for the animation
  //  float omega = (2.0 * PI) / (2 * cube.getCubeSize()); // Calculate the radial frequency omega
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();

  // Other variables

  float xoffset = float(PI / (((90.00) + random(90)) / float(cubesizex))); //19//24
  float yoffset = float(PI / (((90.0) + random(90)) / float(cubesizey))); //37//37
  float zoffset = float(PI / (((90.0) + random(90)) / float(cubesizez))); //24//19

  float xx;
  float yy;
  float zz;


  float plasma;
  float hue;
  int int_hue;

  float xmap[cubesizex];
  float ymap[cubesizey];
  float zmap[cubesizez];
  for ( int cc = 0 ; cc < cubesizex; cc++)
  {
    xmap[cc] = map(cc, 0, cubesizex - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizex))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  for ( int cc = 0 ; cc < cubesizey; cc++)
  {
    ymap[cc] = map(cc, 0, cubesizey - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizey))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  for ( int cc = 0 ; cc < cubesizez; cc++)
  {
    zmap[cc] = map(cc, 0, cubesizez - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizez))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }

  float fx;
  float fy;
  float fz;


  float  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    for ( int x = 0; x < cubesizex ; x++)
    {
      //fx = float(sin(xx + xmap[x])) ;
      fx = sin(xx + (xmap[x] / 1000.0));
      for (int   z = 0; z < cubesizez; z++)
      {

        // fz = float(sin(zz + xmap[z])) ;
        fz = sin(zz + (zmap[z]) / 1000.0) ;
        for ( int  y = 0 ; y < cubesizey ; y++ )
        {
          // fy = float(sin(yy + xmap[y])) ;
          fy = sin(yy + (ymap[y] / 1000.0));
          plasma = ((fx + fz + fy) / 3.0);
          //plasma = sin(plasma / 2.0);
          //   plasma =   sin ( 2 * PI * (fy + fz + fx) / 3 ); // - 1 , 1  // (0-6) * 2.5 = (0 - 15)
          hue = 180 + 180 * cos((fy + fz + fx) / 3);
          int_hue = int(hue);

          // cube.HSV(z, x, y, hue / 5 , 1,  sin (2 * PI * sin((sin(yy + xmap[y]) + sin(zz + xmap[z]) + sin(xx + xmap[x])) / 3 )/8));
          if (plasma >= 0.0) {
            cube.HSV( z, x, y, int_hue   , 1.0, plasma);
          }


        }//  y
      }  //  z
    }    //  x

    xx = xx + xoffset;
    zz = zz + zoffset;
    yy = yy + yoffset;

    if (xx > 10.0 * PI) {
      xx = 0;
    }
    if (yy > 10.0 * PI) {
      yy = 0;
    }
    if (zz > 10.0 * PI) {
      zz = 0;
    }


    cube.drawVoxels();
    if (shiftDelay > 0) {
      delay(shiftDelay);
    }


    // hue
  } // while

  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


}//void plasma1




// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void test (float time, USCL & cube, uint16_t shiftDelay)
{
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  float max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  unsigned int int_max_brightness = max_brightness;
  unsigned int int_plasma;
  //  unsigned int hue;
  float S = 1.0; // saturation
  float V = 1.0; // value (alpha)
  int hue = 0;
  float  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    //    for (V = 0.067; V <= 1; V=V+(0.067))
    //     {
    for ( hue = 0; hue < 360 ; hue = hue + 60)
    {

      for ( int x = 0; x < cubesizex ; x++)
      {

        for ( int y = 0; y < cubesizey; y++)
        {

          for ( int z = 0 ; z < cubesizez ; z++ )
          {

            cube.HSV(z, x, y, hue, 1.00, 1.00 );
          }//  z
          cube.drawVoxels();
          if (shiftDelay > 0) {
            delay(shiftDelay);
          }
          //
        }  //  y
      }    //  x


      //   cube.drawVoxels();
      //   if (shiftDelay > 0) {
      //     delay(shiftDelay);
      //   }

      ///////////////////////
    } // hue
    //    } //V


  } // While
}// void

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void HSV_test (float time, USCL & cube, uint16_t shiftDelay)
{
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();
  
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  float max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  unsigned int int_max_brightness = max_brightness;
  unsigned int int_plasma;
  //  unsigned int hue;
  float S = 1.0; // saturation
  float V = 1.0; // value (alpha)

  float  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    //    for (V = 0.067; V <= 1; V=V+(0.067))
    //     {
    for (int hue = 0; hue < 360; hue = hue + 1)
    {


      for ( byte x = 0; x < cubesizex ; x++)
      {

        for ( byte y = 0; y < cubesizey; y++)
        {

          for ( byte z = 0 ; z < cubesizez ; z++ )
          {
            S = float ( 1.0 / (cubesizey - y - 1) );
            V = float ( 1.0 / (cubesizex - x - 1) );
            cube.HSV(z, x, y, hue, S, V );


          }//  z
        }  //  y
      }    //  x


      cube.drawVoxels();
      if (shiftDelay > 0) {
        delay(shiftDelay);
      }

      ///////////////////////
    } // hue
    //    } //V


  } // While
}// void
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//****setup****setup****setup****setup****setup****setup****setup****setup****setup****setup****setup****setup****setup
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void setup()
{

  //Start the cube
  cube.begin();
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
////***start loop***start loop***start loop***start loop***start loop***start loop***start loop***start loop***start loop
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop()
{
  int cubesize = cube.getCubeSizeZ();
  int frames =  cube.getFPS();
  int delay_FPS = 1000 / frames;
  int j = 1 + random(8);

  plasma6(animtime, cube ,  0 ); // animation time lenght, cube, delay in frames (if it's too fast)
  rotating_circles(animtime, cube,   0 * delay_FPS); // animation time lenght, cube, delay time
  rotating_star(animtime, cube,   0 * delay_FPS); // animation time lenght, cube, delay time
  rotating_cube(animtime, cube,   0 * delay_FPS); // animation time lenght, cube, delay time
  plasma_sinx2y2(animtime, cube, 0 * delay_FPS); // animation time lenght, cube, delay time
  plasma_sineWave(animtime, cube, 2 * delay_FPS, j * cubesize); // shiftDelay ; period = *8
  plasma6(animtime, cube ,  0 ); // animation time lenght, cube, delay in frames (if it's too fast)
  plasma5(animtime * 3, cube, 0); // animatime,cube, ms delay
  plasma4(animtime * 3, cube, 0); // animatime,cube, ms delay
  color_wheelTWO(animtime, cube, delay_FPS * frames / 8); // animation time lenght, cube, delay time
  sineWaveThree(animtime, cube, 1.5 * delay_FPS, j * cubesize, j  * cubesize); // animation time lenght, cube, delay time, period ; baseWavePeriodicFactor
  sineWave(animtime, cube, delay_FPS, j * cubesize); // shiftDelay ; period = *8
  RandomLedFade(animtime, cube,  j * cubesize,  2 * delay_FPS); // animation time lenght, cube, number of fading leds, delay time
  sineWaveTwo(animtime, cube, 1.5 * delay_FPS, 24, 32); // animation time lenght, cube, delay time,  periodX ; periodY
  snake(animtime, cube, 120, j + cubesize , 2 * delay_FPS); //animation time lenght, cube, delay time,snake lenght , collision modde
  plasma3(animtime, cube, delay_FPS);// animatime,cube, ms delay
  wipeout(animtime, cube, 3.5 * delay_FPS); // animation time lenght, cube, delay time
  MY_4D_function2(animtime, cube, delay_FPS);// animatime,cube, ms delay
  MY_4D_function1(animtime, cube, 2 * delay_FPS); // animation time lenght, cube, delay time
  explodingimplodingsphere(animtime, cube, delay_FPS); // animation time lenght, cube, delay time
  sinx2y2(animtime, cube,  delay_FPS); // animation time lenght, cube, delay time
  plasma2(animtime, cube, delay_FPS);// animatime,cube, ms delay
  plasma1(animtime, cube, 0);// animatime,cube, ms delay
  slantingLines(animtime, cube, 2 * delay_FPS, true); // animation time lenght, cube, delay time, invertDirection = false/true
  slantingLines(animtime, cube, 2 * delay_FPS, false); // animation time lenght, cube, delay time, invertDirection = false/true
  fireworks(animtime , cube, j * cubesize , 2 * delay_FPS); // animation time lenght, cube, number of particles, delay time
  cornerCube(animtime, cube, 3 * delay_FPS, 1); // animation time lenght, cube, resizeDelay ; minimumCubeSize
  rain(animtime, cube , j * cubesize ,  cube.getMaxBrightness()  , 3 * delay_FPS , false ); // animation time lenght, cube, rainVoxels; floorVoxels; rainDelay; invertDirection = false/true
  explodingsphere(animtime, cube, 3 * delay_FPS); // animation time lenght, cube, delay time
  folder(animtime, cube, 1 * delay_FPS ); // animation time lenght, cube, delay time
  starfield(animtime,  cube, cubesize * j,  3 * delay_FPS); //  animation time lenght, cube,number of stars, delay time
} // loop
