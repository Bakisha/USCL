

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




void explodingsphere(float time, USCL & cube, uint16_t shiftDelay /* = 75 */)
{
  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // Declare and initialize other variables needed for the animation
  // float omega = (2.0 * PI) / (2 * cube.getCubeSize()); // Calculate the radial frequency omega
  /* explodingsphere parameters   ***   ***
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'USCL' class that you made in your main program.
    - shiftDelay :        Define the delay (in ms) . Normally a value of
                    50 - 100 ms will suffice.
  */
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();

  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  int hue;
  float S = 1;
  float V = 1;
  float radspeed = 1;
  float radius = cubesizez / 2;
  float radius2;
  float distance;

  int center_x = (cubesizex / 2) - 1;
  int center_y = (cubesizey / 2) - 1;
  int center_z = (cubesizez / 2) - 1;

  //float x, y, z;
  float x1, y1, z1;
  float x2, y2, z2;
  // Other variables
  int br;
  int M = 8;
  int N = 8;
  int n, m;
  int x, y, z;


  long  startTime = millis();
  while (millis() - startTime < time)
  {

    int hue = random(360);
    ///////////////////
    center_x = (cubesizex / 4) + random(cubesizex / 2);
    center_y = (cubesizey / 4) + random(cubesizey / 2);
    center_z = (cubesizez / 4) + random(cubesizez / 2);



    for (  radius = 0; radius <  cubesizez   ; radius = radius + radspeed)
    {

      for (  m = 1; m < M ; m++)
      {

        for (  n = 1; n < N ; n++)
        {

          x = radius * (sin(PI * m / M) * cos(2 * PI * n / N));

          y = radius * (sin(PI * m / M) * sin(2 * PI * n / N));

          z = radius * cos(PI * m / M);

          //     (x, y, z) = (sin(Pi * m / M) cos(2Pi * n / N), sin(Pi * m / M) sin(2Pi * n / N), cos(Pi * m / M));

          cube.HSV(center_z + z, center_y + y, center_z + x , hue, S, V);





        }
      }
      cube.drawVoxels();
      delay(shiftDelay);

    }
    //    cube.drawVoxels();
    //   delay(shiftDelay);





    ///////////////////////
  } // while

}
