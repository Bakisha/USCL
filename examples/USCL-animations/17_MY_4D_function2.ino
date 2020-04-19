


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
