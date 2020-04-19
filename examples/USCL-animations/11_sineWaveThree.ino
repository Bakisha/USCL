

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
