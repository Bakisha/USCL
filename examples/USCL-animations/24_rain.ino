


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
