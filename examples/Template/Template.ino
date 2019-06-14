#include<SPI.h>
#include<USCL.h>
// Use this as template to create new animation
float animtime = 60000; // global duration of animations
USCL cube(CUBE_SIZE_8x8x8, RGB_CUBE , PA0, PA0,  30 , 2, SPI_speed_8); // change it to mach your cube



// ------------------------c-o-p-y-----f-r-o-m-----h-e-r-e---------------------------------------------------------------------------------------------------------------------------------------------
  /* My_animation1 parameters   ***   ***
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'USCL' class that you made in your main program.
    - shiftDelay :        Define the delay in frames
  */
void My_animation1 (float time, USCL & cube, uint16_t shiftDelay)
{
  byte cubesize = cube.getCubeSize();
  int FPS = cube.getFPS();
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesize - 1;
  int hue;
  float S = 0.0; // saturation
  float V = 1.0; // value (alpha)

  float  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    for (int hue = 0; hue < 360; hue = hue + 1)
    {
      for ( byte x = 0; x < cubesize ; x++)
      {
        for ( byte y = 0; y < cubesize; y++)
        {
          for ( byte z = 0 ; z < cubesize ; z++ )
          {
            cube.HSV(z, x, y, hue, S, V );
          }//  z
        }  //  y
      }    //  x


      cube.drawVoxels();
      for (int varbl = 0; varbl < shiftDelay; varbl++) // delay x number of frames
      {
        cube.copyCurrentFrame();
        cube.drawVoxels();
      }


    } // hue
    ///////////////////////
  } // While
}// void
// ---------------------c-o-p-y---t-o----h-e-r-e-------------------------------------------------------------------------------------------------------------------------------------------------------



void setup()
{
  //Start the cube
  cube.begin();
}
void loop()
{
  My_animation1(animtime, cube ,  0 ); // animation time lenght, cube, delay in frames (if it's too fast)
}
