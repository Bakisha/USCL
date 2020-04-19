

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
