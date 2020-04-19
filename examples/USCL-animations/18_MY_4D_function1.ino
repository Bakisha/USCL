


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void MY_4D_function1 (float time, USCL & cube, uint16_t shiftDelay)
{
  //
  /* MY_4D_function1 cube parameters
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'LedCube' class that you made in your main program.
    - shiftDelay (optional):    Define the delay (in ms) . Normally a value of 50 - 100 ms will suffice.
    -function:
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
  float hue = 180;
  int int_hue;

  float  hue_min = 0;
  float hue_max = 343.47;





  float  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    for (   float a = -4 * cubesizez; a < 4 * 2 * cubesizez; a = a + 0.25)
    {
      for ( int x = 0; x <  cubesizex ; x++)
      {

        for ( int y = 0; y < cubesizey; y++)
        {

          for ( int z = 0 ; z <  cubesizez ; z++ )
          {

            // hue = (x + a) * 2.0 - ( (y + a) / 2.0 ) + (((z + a) * 3.) / 2.0);
            hue = (x + a) * (x + a) + (y + a) * (y + a) + (z + a) * (z + a); // min : 0  ; max : 343.47

            int_hue =     map(int(hue), int(hue_min), int(hue_max), 0, 360);
            //        int_hue = int(hue);
            //            Serial.println(int_hue);
            cube.HSV(z, x, y, int_hue , 1, 1 );

          }//  z
        }  //  y
      }    //  x


      cube.drawVoxels();

      if (shiftDelay > 0) {
        delay(shiftDelay);
      }
    } // a
    ///////////////////////




  } // While
}// void

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
