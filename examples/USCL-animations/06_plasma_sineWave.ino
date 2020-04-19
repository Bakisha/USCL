

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



void plasma_sineWave(float time, USCL & cube, uint16_t shiftDelay /* = 75 */, uint8_t period /* = 16 */)
{
  /* Plasma colored Sine wave parameters
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

  float step_offset = 180.00;
  float xoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizex))); //19//24
  float yoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizey))); //37//37
  float zoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizez))); //24//19


  float xmap[cubesizex];
  float ymap[cubesizey];
  float zmap[cubesizez];
  for ( int cc = 0 ; cc < cubesizex; cc++)
  {
    xmap[cc] = map(cc, 0, cubesizex - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizex))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  for ( int cc = 0 ; cc < cubesizey; cc++)
  {
    ymap[cc] = map(cc, 0, cubesizey - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizey))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }
  for ( int cc = 0 ; cc < cubesizez; cc++)
  {
    zmap[cc] = map(cc, 0, cubesizez - 1, 0.0, 1000.0 * ((float(1.0 / float(1.0 * float(cubesizez))) * PI)) );
    //    ymap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
    //    zmap[cc] = map(cc, 0, cubesize - 1, 0, 2 * PI);
  }



  float fx;
  float fy;
  float fz;

  float plasma;

  float xx;
  float yy;
  float zz;

  long  startTime = millis();
  while (millis() - startTime < time)
  {





    for (i = timeCount; i < (timeCount + cubesizez); i++)
    {
      z = round(sineOffset * sin(i * omega) + sineOffset); // Calculate z position
      x = i - timeCount;
      if (x < 0) x = 0;
      if (x > cubesizex - 1) x = cubesizex - 1;
      fx = sin(xx + (xmap[x] / 1000.0));
      fz = sin(zz + (zmap[z]) / 1000.0) ;
      for (j = 0; j < cubesizey; j++)
      {
        y = j;



        fy = cos(yy + (ymap[y] / 1000.0));

        plasma = 180 + 60 * (fx + fz + fy) ;

        if (plasma >= 0.0) {
          if (plasma <= 360.0 ) {
            cube.HSV(z, x, y, plasma, 1.00, 1.00);
          }
        }


      } // j
    } // i
    cube.drawVoxels();
    delay(shiftDelay);
    timeCount++;
    if (timeCount >= period)
      timeCount = 0;


    xx = xx + xoffset;
    if (xx > 2.0 * PI)
    {
      xx = 0;

    }
    yy = yy + yoffset;
    if (yy > 2.0 * PI)
    {
      yy = 0;

    }
    zz = zz + zoffset;
    if (zz > 2.0 * PI)
    {
      zz = 0;
    }


  }//while
}//void
