

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void folder(float time, USCL & cube, uint16_t shiftDelay)
{

  /* folder parameters   ***   ***
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'USCL' class that you made in your main program.
    - shiftDelay :        Define the delay (in ms) . Normally a value of 20 - 100 ms will suffice.

  */
  // Declare and initialize other variables needed for the animation


  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();

  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  int hue = random(360);
  float S = 1;
  float V = float(float(random(max_brightness + 1)) / float(max_brightness)); //float V=float(random(max_brightness+1)/(1+max_brightness)  );
  // Other variables
  byte select;


  float  startTime = millis();
  while (millis() - startTime < time)
  {

    //    cube.HSV(z, y, x, 1, bightness);
    ///////////////////

    select = random(8);



    //////////////////////////////////////////// FrontZ to FrontX
    hue =  random(360);
    V = float(float(1.0 + random(max_brightness )) / float(max_brightness)); //float V=float(random(max_brightness+1)/(1+max_brightness)  );
    for (byte a = 0; a < cubesizey - 1; a++)
    {
      for (byte z = 0; z < cubesizex; z++)
      {
        cube.HSV_line(0, 0, z,  a, cubesizey - 1, z, hue, S, V );

      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    for (byte a = 0; a < cubesizey; a++)
    {
      for (byte z = 0; z < cubesizey; z++)
      {
        cube.HSV_line(0, 0, z,  cubesizez - 1, cubesizey - 1 - a, z, hue, S, V);
      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    ///////////////////////////////////////////



    //////////////////////////////////////////// FrontX to BackZ
    hue = random(360 );
    V = float(float(1.0 + random(max_brightness )) / float(max_brightness)); //float V=float(random(max_brightness+1)/(1+max_brightness)  );
    for (byte a = 0; a < cubesizex - 1; a++)
    {
      for (byte z = 0; z < cubesizey; z++)
      {
        cube.HSV_line( cubesizez - 1 , 0, z , 0, a , z, hue, S, V);

      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    for (byte a = 0; a < cubesizez; a++)
    {
      for (byte z = 0; z < cubesizey; z++)
      {
        cube.HSV_line((cubesizez - 1), 0, z,  a, cubesizey - 1 , z, hue, S, V);
      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    ///////////////////////////////////////////


    //////////////////////////////////////////// BackZ to BackY
    hue =  random(360 );
    V = float(float(1.0 + random(max_brightness )) / float(max_brightness)); //float V=float(random(max_brightness+1)/(1+max_brightness)  );
    for (byte a = 0; a < cubesizez - 1; a++)
    {
      for (byte z = 0; z < cubesizex; z++)
      {
        cube.HSV_line(cubesizez - 1, z, cubesizey - 1, cubesizez - 1 - a, z , 0, hue, S, V);

      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    for (byte a = 0; a < cubesizey; a++)
    {
      for (byte z = 0; z < cubesizex; z++)
      {
        cube.HSV_line(cubesizez - 1, z, cubesizey - 1, 0, z , a, hue, S, V);
      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    ///////////////////////////////////////////


    //////////////////////////////////////////// BackY to BackX
    hue =  random(360);
    V = float(float(1.0 + random(max_brightness )) / float(max_brightness)); //float V=float(random(max_brightness+1)/(1+max_brightness)  );
    for (byte a = 0; a < cubesizey - 1; a++)
    {
      for (byte z = 0; z < cubesizez; z++)
      {
        cube.HSV_line(z, cubesizex - 1, cubesizey - 1, z, 0 , cubesizey - 1 - a, hue, S, V);

      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    for (byte a = 0; a < cubesizex; a++)
    {
      for (byte z = 0; z < cubesizez; z++)
      {
        cube.HSV_line(z, cubesizex - 1, cubesizey - 1, z, a , 0, hue, S, V);
      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    ///////////////////////////////////////////

    //////////////////////////////////////////// BackX to FrontY
    hue = random(360 );
    V = float(float(1.0 + random(max_brightness )) / float(max_brightness)); //float V=float(random(max_brightness+1)/(1+max_brightness)  );
    for (byte a = 0; a < cubesizex - 1; a++)
    {
      for (byte z = 0; z < cubesizez; z++)
      {
        cube.HSV_line(z, cubesizex - 1, 0, z, cubesizex - 1 - a , cubesizey - 1, hue, S, V);

      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    for (byte a = 0; a < cubesizey; a++)
    {
      for (byte z = 0; z < cubesizez; z++)
      {
        cube.HSV_line(z, cubesizex - 1, 0, z, 0 , cubesizey - 1 - a, hue, S, V);
      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    ///////////////////////////////////////////


    //////////////////////////////////////////// FrontWallY to FrontWallX
    hue =  random(360 );
    V = float(float(1.0 + random(max_brightness )) / float(max_brightness)); //float V=float(random(max_brightness+1)/(1+max_brightness)  );
    for (byte a = 0; a < cubesizey - 1; a++)
    {
      for (byte z = 0; z < cubesizez; z++)
      {
        cube.HSV_line(z, 0, 0,  z, cubesizex - 1, a, hue, S, V);
      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    for (byte a = 0; a < cubesizex; a++)
    {
      for (byte z = 0; z < cubesizez; z++)
      {
        cube.HSV_line(z, 0, 0,  z, cubesizex - 1 - a, cubesizey - 1, hue, S, V);
      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    ///////////////////////////////////////////

    //////////////////////////////////////////// FrontX to FrontZ
    hue =  random(360 );
    V = float(float(1.0 + random(max_brightness )) / float(max_brightness)); //float V=float(random(max_brightness+1)/(1+max_brightness)  );
    for (byte a = 0; a < cubesizex - 1; a++)
    {
      for (byte z = 0; z < cubesizey; z++)
      {
        cube.HSV_line(0, 0, z,  cubesizez - 1, a, z, hue, S, V );

      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    for (byte a = 0; a < cubesizez; a++)
    {
      for (byte z = 0; z < cubesizey; z++)
      {
        cube.HSV_line(0, 0, z,  cubesizez - 1 - a, cubesizex - 1 , z, hue, S, V);
      }
      delay(shiftDelay);
      cube.drawVoxels();
    }
    ///////////////////////////////////////////



    //    delay(shiftDelay);
    ///////////////////////
  } // while



}// void folder
