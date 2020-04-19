

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
