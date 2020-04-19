// --------------------------------------------------------------------------------------------------------------------------------------------------------------------
/* My_animation1 parameters   ***   ***
  - time:                 Define the length of time that the animation should be executed.
  - cube:            Define the instance of the 'USCL' class that you made in your main program.
  - shiftDelay :        Define the delay in frames
*/
void My_animation1 (float time, USCL & cube, uint16_t shiftDelay)
{
  byte cubesizeX = cube.getCubeSizeX();
  byte cubesizeY = cube.getCubeSizeY();
  byte cubesizeZ = cube.getCubeSizeZ();
  //byte cubesize = cube.getCubeSize();
  int FPS = cube.getFPS() ;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();

  int hue;
  float S = 1.0; // saturation
  float V = 1.0; // value (alpha)

  float  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    for (int hue = 0; hue < 360; hue = hue + 1)
    {
      for ( V = 0.0; V <= 1.0; V = V + float(1.0 / float(max_brightness)) )
      {


        for ( byte z = 0 ; z < cubesizeZ ; z++ )
        {
          for ( byte y = 0; y < cubesizeY; y++)
          {
            for ( byte x = 0; x < cubesizeX ; x++)
            {
              //           cube.RED(z, x, y, max_brightness );
              //           cube.GREEN(z, x, y, max_brightness );
              //          cube.BLUE(z, x, y, max_brightness );

              cube.HSV(z, x, y, hue, S, V);

            }//  z
          }  //  y
        }    //  x

        cube.drawVoxels();
        // delay(500);

      } // V

      for ( V = 1.0; V >= 0.0; V = V - float(1.0 / float(max_brightness)) )
      {


        for ( byte z = 0 ; z < cubesizeZ ; z++ )
        {
          for ( byte y = 0; y < cubesizeY; y++)
          {
            for ( byte x = 0; x < cubesizeX ; x++)
            {
              //           cube.RED(z, x, y, max_brightness );
              //           cube.GREEN(z, x, y, max_brightness );
              //          cube.BLUE(z, x, y, max_brightness );

              cube.HSV(z, x, y, hue, S, V);

            }//  z
          }  //  y
        }    //  x

        cube.drawVoxels();
        // delay(500);

      } // V
    } // hue
    ///////////////////////
  } // While
}// void
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
