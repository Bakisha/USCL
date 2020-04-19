



// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




void plasma6(float time, USCL & cube, uint16_t shiftDelay)
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

  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  int FPS = cube.getFPS() ;
  int delay_FPS = 1000 / FPS;
  float S = 1.0;
  float V = 1.0;
  int hue = 0;

  // Other variables
  float step_offset = 240.0;
  float xoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizex))); //19//24
  float yoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizey))); //37//37
  float zoffset = float(PI / (((step_offset) + random(step_offset)) / float(cubesizez))); //24//19

  float xx;
  float yy;
  float zz;

  float step_offsetV = 180.0;
  float xoffsetV = float(PI / (((step_offsetV) + random(step_offsetV)) / float(cubesizex))); //19//24
  float yoffsetV = float(PI / (((step_offsetV) + random(step_offsetV)) / float(cubesizey))); //37//37
  float zoffsetV = float(PI / (((step_offsetV) + random(step_offsetV)) / float(cubesizez))); //24//19

  float xxV;
  float yyV;
  float zzV;

  float plasma;

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

  float fxV;
  float fyV;
  float fzV;



  float  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    for ( int x = 0; x < cubesizex ; x++)
    {
      //fx = float(sin(xx + xmap[x])) ;
      fx = sin(xx + (xmap[x] / 1000.0));
      fxV = sin(xxV + (xmap[x] / 1000.0));
      for (int   z = 0; z < cubesizez; z++)
      {
        fz = sin(zz + (zmap[z]) / 1000.0) ;
        fz = sin(zzV  + (zmap[z]) / 1000.0) ;
        for ( int  y = 0 ; y < cubesizey ; y++ )
        {
          fy = sin(yy + (ymap[y] / 1000.0));
          fyV = sin(yyV + (ymap[y] / 1000.0));
          plasma =  (180.0 + 60.0 * (fx + fz + fy) ); // *max=3

          hue = int(plasma ) ;
          V =   0.5 + 0.5 * sin(2.0 * PI * ((fxV + fzV +  fyV) / 3) ) ;
          cube.HSV( z, x, y, hue   , 1.0, V);

        }//  y
      }  //  z
    }    //  x


    xx = xx + xoffset;
    if (xx > 2.0 * PI)
    {
      xx = 0;


    }
    zz = zz + zoffset;
    if (zz > 2.0 * PI)
    {
      zz = 0;
    }
    yy = yy + yoffset;
    if (yy > 2.0 * PI)
    {
      yy = 0;

    }






    zzV = zzV + zoffsetV;
    if (zzV > 2.0 * PI)
    {
      zzV = 0;

    }
    xxV = xxV + xoffsetV;
    if (xxV > 2.0 * PI)
    {
      xxV = 0;

    }

    yyV = yyV + yoffsetV;
    if (yyV > 2.0 * PI)
    {
      yyV = 0;

    }

    cube.drawVoxels();
    if (shiftDelay > 0) {
      delay(shiftDelay);
    }


    // hue
  } // while



}//void plasma6
