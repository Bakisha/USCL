

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void test (float time, USCL & cube, uint16_t shiftDelay)
{
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();

  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  float max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  unsigned int int_max_brightness = max_brightness;
  unsigned int int_plasma;
  //  unsigned int hue;
  float S = 1.0; // saturation
  float V = 1.0; // value (alpha)
  int hue = 0;
  float  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    //    for (V = 0.067; V <= 1; V=V+(0.067))
    //     {
    for ( hue = 0; hue < 360 ; hue = hue + 60)
    {

      for ( int x = 0; x < cubesizex ; x++)
      {

        for ( int y = 0; y < cubesizey; y++)
        {

          for ( int z = 0 ; z < cubesizez ; z++ )
          {

            cube.HSV(z, x, y, hue, 1.00, 1.00 );
          }//  z
          cube.drawVoxels();
          if (shiftDelay > 0) {
            delay(shiftDelay);
          }
          //
        }  //  y
      }    //  x


      //   cube.drawVoxels();
      //   if (shiftDelay > 0) {
      //     delay(shiftDelay);
      //   }

      ///////////////////////
    } // hue
    //    } //V


  } // While
}// void
