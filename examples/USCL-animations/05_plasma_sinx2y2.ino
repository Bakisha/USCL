

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void plasma_sinx2y2(float time, USCL & cube, uint16_t shiftDelay /* = 75 */)
{
  /* plasma sinx2y2 cube parameters
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
  // byte maxVoxelValue = cubesize - 1;
  float S = 1;
  float V = 1;
  int hue = 180;
  // Declare and initialize other variables needed for the animation
  float omega = (2.0 * PI) / (2 * cubesizez); // Calculate the radial frequency omega


  // Other variables
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
    ///////////////////


    for ( float offset = 0; offset < 4 * PI ; offset = offset +  0.18 ) {

      //  hue = 180 + 180 * (sin(PI * sin(offset / 4.0)));

      for ( byte x = 0; x < cubesizex; x++) {
        for ( byte y = 0; y < cubesizey; y++)
        {
          float      ipsilon = 10000 *  sin(offset + sqrt(pow(map(x, 0, cubesizez - 1, -PI, PI), 2) + pow(map(y, 0, cubesizez - 1, -PI, PI), 2)));
          // y=sin(z * omega + offset ) * cos( x * omega + offset  );
          float     z   = map(ipsilon, -10000, 10000, 0, cubesizez);

          float      br = 10000 * sin(x * omega + offset / 0.55  );
          fx = sin(xx + (xmap[int(x)] / 1000.0));
          fz = sin(zz + (zmap[int(z)]) / 1000.0) ;
          fy = cos(yy + (ymap[int(y)] / 1000.0));

          br = map(br, -10000, 10000, 1, 10000 );

          plasma = 180 + 60 * (fx + fz + fy) ;
          cube.HSV(z, x, y, plasma, 1, float(br) / 100.00);
        }
      }
      cube.drawVoxels();
      if (shiftDelay > 0) {
        delay(shiftDelay);
      }


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

    } // offset

    ///////////////////////
  } // while
}// void


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
