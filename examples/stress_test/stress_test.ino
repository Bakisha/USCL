#include<SPI.h>
#include<USCL.h>
float animtime = 60000; // global duration of animations
USCL cube(4, 4, 4, RGB_CUBE , PA0, PA1,  60 , 2, SPI_speed_16); // change it to mach your cube

// ------------------------c-o-p-y-----f-r-o-m-----h-e-r-e---------------------------------------------------------------------------------------------------------------------------------------------
/* led test  parameters   ***  test ALL LED ON (white) ***
  - time:                 Define the length of time that the animation should be executed.
  - cube:            Define the instance of the 'USCL' class that you made in your main program.
*/
void stress_test (float time, USCL & cube )
{
  byte cubesizeX = cube.getCubeSizeX();
  byte cubesizeY = cube.getCubeSizeY();
  byte cubesizeZ = cube.getCubeSizeZ();
  int FPS = cube.getFPS() ;
  int delay_FPS =  5000; // test for 5 seconds then OFF 5 seconds
  int max_brightness = cube.getMaxBrightness();
  int hue;
  float S = 1.0; // saturation
  float V = 1.0; // value (alpha)

  float  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////
    for ( byte z = 0 ; z < cubesizeZ ; z++ )
    {
      for ( byte y = 0; y < cubesizeY; y++)
      {
        for ( byte x = 0; x < cubesizeX ; x++)
        {
          cube.RGB(z, x, y, max_brightness, max_brightness, max_brightness);

        }//  z
      }  //  y
    }    //  x
    cube.drawVoxels();
    delay(delay_FPS);
    cube.drawVoxels(); // back buffer is empty
    delay(delay_FPS);
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
  stress_test(animtime, cube ); // animation time lenght, cube name
}
