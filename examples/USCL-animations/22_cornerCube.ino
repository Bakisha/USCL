

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
