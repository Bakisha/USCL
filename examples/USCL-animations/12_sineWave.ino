

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
