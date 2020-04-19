


void plasma1(float time, USCL & cube, uint16_t shiftDelay)
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

  float xoffset = float(PI / (((90.00) + random(90)) / float(cubesizex))); //19//24
  float yoffset = float(PI / (((90.0) + random(90)) / float(cubesizey))); //37//37
  float zoffset = float(PI / (((90.0) + random(90)) / float(cubesizez))); //24//19

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

        // fz = float(sin(zz + xmap[z])) ;
        fz = sin(zz + (zmap[z]) / 1000.0) ;
        for ( int  y = 0 ; y < cubesizey ; y++ )
        {
          // fy = float(sin(yy + xmap[y])) ;
          fy = sin(yy + (ymap[y] / 1000.0));
          plasma = ((fx + fz + fy) / 3.0);
          //plasma = sin(plasma / 2.0);
          //   plasma =   sin ( 2 * PI * (fy + fz + fx) / 3 ); // - 1 , 1  // (0-6) * 2.5 = (0 - 15)
          hue = 180 + 180 * cos((fy + fz + fx) / 3);
          int_hue = int(hue);

          // cube.HSV(z, x, y, hue / 5 , 1,  sin (2 * PI * sin((sin(yy + xmap[y]) + sin(zz + xmap[z]) + sin(xx + xmap[x])) / 3 )/8));
          if (plasma >= 0.0) {
            cube.HSV( z, x, y, int_hue   , 1.0, plasma);
          }


        }//  y
      }  //  z
    }    //  x

    xx = xx + xoffset;
    zz = zz + zoffset;
    yy = yy + yoffset;

    if (xx > 10.0 * PI) {
      xx = 0;
    }
    if (yy > 10.0 * PI) {
      yy = 0;
    }
    if (zz > 10.0 * PI) {
      zz = 0;
    }


    cube.drawVoxels();
    if (shiftDelay > 0) {
      delay(shiftDelay);
    }


    // hue
  } // while

  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


}//void plasma1
