


void plasma5(float time, USCL & cube, uint16_t shiftDelay)
{
  /* plasma1 parameters   ***   ***
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'USCL' class that you made in your main program.
    - shiftDelay :        Define the delay (in ms) . Normally a value of  0 - 100 ms will suffice.
  */
  // Declare and initialize other variables needed for the animation
  //  float omega = (2.0 * PI) / (2 * cube.getCubeSize()); // Calculate the radial frequency omega
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();

  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();

  // Other variables
  float step_offset = 180.00;
  float xoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizex))); //19//24
  float yoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizey))); //37//37
  float zoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizez))); //24//19

  float xx;
  float yy;
  float zz;


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
      //fx = float(sin(xx + xmap[x])) ;
      fx = sin(xx + (xmap[x] / 1000.0));
      for (int   z = 0; z < cubesizez; z++)
      {
        fz = sin(zz + (zmap[z]) / 1000.0) ;
        for ( int  y = 0 ; y < cubesizey ; y++ )
        {
          fy = cos(yy + (ymap[y] / 1000.0));
          plasma = 180 + 70 * (fx + fz + fy) ; // *max=3
          if (plasma >= 0.0) {
            if (plasma <= 360.0 ) {
              cube.HSV( z, x, y, plasma   , 1.0, float( 1.1 / max_brightness) );
            }
          }
        }//  y
      }  //  z
    }    //  x
    xx = xx + xoffset;
    if (xx > 2.0 * PI)
    {
      xx = 0;
      yy = yy + yoffset;
      if (yy > 2.0 * PI)
      {
        yy = 0;

      }
    }

    zz = zz + zoffset;
    if (zz > 2.0 * PI)
    {
      zz = 0;
    }





    cube.drawVoxels();
    if (shiftDelay > 0) {
      delay(shiftDelay);
    }


    // hue
  } // while

  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


}//void plasma5

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
