

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void plasma2(float time, USCL & cube, int shiftDelay)
{
  /* plasma2 parameters   ***   ***
    - time:                 Define the length of time that the animation should be executed.
    - cubeRef:            Define the instance of the 'LedCube' class that you made in your main program.
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

  // Other variables

  float xoffset = PI / (25 + random(90));//19
  float yoffset = PI / (25 + random(90));//37
  float zoffset = PI / (25 + random(90)); //24

  float x_max = 120.0 * PI;
  float y_max = 120.0 * PI;
  float z_max = 120.0 * PI;

  float xx_time = 0;
  float yy_time = 0;
  float zz_time = 0;


  float plasma;
  float hue;
  int int_hue;



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

  float  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    for ( int x = 0; x < cubesizex ; x++)
    {
      for ( int y = 0 ; y < cubesizey ; y++ )
      {
        for ( int z = 0; z < cubesizez; z++)
        {


          fx =  xmap[x] / 1000.0;
          fx = fx * (xx_time);

          fy = ymap[y] / 1000.0;
          fy = fy * (yy_time);

          fz =  zmap[z] / 1000.0;
          fz =  fz * (zz_time );
          plasma =  sin((fy + fz  + fx ) / 3.0 );

          hue = 10000 * cos((fy + fz + fx) / 3);
          int_hue = map(hue, -10000, 10000, 0, 360);

          if (plasma > 0) {
            cube.HSV( z, x, y, int_hue   , 1.0, plasma);//  cube.HSV(z, x, y, 1, plasma * max_brightness);
          }


        }//  z
      }  //  y
    }    //  x


    xx_time = xx_time + xoffset;
    if (xx_time > x_max) {
      xoffset = float(PI / (25 + random(90)));
      xx_time = 0.0;
    }
    zz_time = zz_time + zoffset;
    if (zz_time > z_max) {
      zoffset = float( PI / (25 + random(90)));
      zz_time = 0.0;
    }
    yy_time = yy_time + yoffset;
    if (yy_time > y_max) {
      yoffset =  float(PI / (25 + random(90)));
      yy_time = 0.0;
    }

    cube.drawVoxels();
    if (shiftDelay > 0) {
      delay(shiftDelay);
    }

    // hue
    ///////////////////////
  } // while



}// void
