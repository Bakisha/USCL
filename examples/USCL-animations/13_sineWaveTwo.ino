


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
