



// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


/* explodingimplodingsphere parameters   ***   ***
  - time:                 Define the length of time that the animation should be executed.
  - cube:            Define the instance of the 'USCL' class that you made in your main program.
  - shiftDelay :        Define the delay (in ms) . Normally a value of
                  50 - 100 ms will suffice.
*/
void explodingimplodingsphere(float time, USCL & cube, uint16_t shiftDelay /* = 75 */)
{
  // Declare and initialize other variables needed for the animation
  // float omega = (2.0 * PI) / (2 * cube.getCubeSize()); // Calculate the radial frequency omega
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
  float radspeed = 0.20;

  float radius = cubesizez / 2;
  float radius2;
  float distance;

  float center_x = (cubesizex / 2) - 1;
  float center_y = (cubesizey / 2) - 1;
  float center_z = (cubesizez / 2) - 1;

  float x1, y1, z1;
  float czzp, cyyp, cxxp, czzn, cyyn, cxxn;
  // Other variables
  float br;


  long  startTime = millis();
  while (millis() - startTime < time)
  {

    ///////////////////
    center_x = (cubesizex / 4.0) + random(cubesizex / 2.0);
    center_y = (cubesizey / 4.0) + random(cubesizey / 2.0);
    center_z = (cubesizez / 4.0) + random(cubesizez / 2.0);



    for (  radius = 0; radius <=  cubesizez - 1  ; radius = radius + radspeed)
    {
      radius2 = radius * radius;
      for ( float x = 0; x < cubesizex ; x++)
      {
        x1 = x * x;
        for ( float y = 0; y < cubesizey ; y++)
        {
          y1 = x1 + (y * y);
          for ( float z = 0; z < cubesizez ; z++)
          {

            distance =  y1 + (z * z)  ;
            if (distance < radius2 )
            {
              br =  (max_brightness / (radius + 1)) * cubesizez;


              czzp = center_z + z;
              cyyp = center_y + y;
              cxxp = center_x + x;
              czzn = center_z - z;
              cyyn = center_y - y;
              cxxn = center_x - x;
              /*
                            if (czzn < 0) {
                              czzn = 0;
                            }
                            if (cyyn < 0) {
                              cyyn = 0;
                            }
                            if (cxxn < 0) {
                              cxxn = 0;
                            }

                            if (czzp > cubesize - 1) {
                              czzp = cubesize - 1;
                            }
                            if (cyyp > cubesize - 1) {
                              cyyp = cubesize - 1;
                            }
                            if (cxxp > cubesize - 1) {
                              cxxp = cubesize - 1;
                            }
              */

              cube.HSV(czzp, cyyp, cxxp , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzp, cyyp, cxxn , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzp, cyyn, cxxp, int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzp, cyyn, cxxn , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzn, cyyp, cxxp , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzn, cyyp, cxxn , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzn, cyyn, cxxp , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzn, cyyn, cxxn , int (360 / (radius2 + 1)), 1, br);


            }


          }
        }
      }
      cube.drawVoxels();
      delay(shiftDelay);
    }



    for (  radius = cubesizez - 1; radius >=  0  ; radius = radius - radspeed )
    {
      radius2 = radius * radius;
      for ( float x = 0; x < cubesizex ; x++)
      {
        x1 = x * x;
        for ( float y = 0; y < cubesizey ; y++)
        {
          y1 = x1 + (y * y);
          for ( float z = 0; z < cubesizez ; z++)
          {

            distance =  y1 + (z * z)  ;
            if (distance < radius2 )
            {
              br = 15 - radius * 2;
              czzp = center_z + z;
              cyyp = center_y + y;
              cxxp = center_x + x;
              czzn = center_z - z;
              cyyn = center_y - y;
              cxxn = center_x - x;

              /*
                            if (czzn < 0) {
                              czzn = 0;
                            }
                            if (cyyn < 0) {
                              cyyn = 0;
                            }
                            if (cxxn < 0) {
                              cxxn = 0;
                            }

                            if (czzp > cubesize - 1) {
                              czzp = cubesize - 1;
                            }
                            if (cyyp > cubesize - 1) {
                              cyyp = cubesize - 1;
                            }
                            if (cxxp > cubesize - 1) {
                              cxxp = cubesize - 1;
                            }
              */
              cube.HSV(czzp, cyyp, cxxp , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzp, cyyp, cxxn , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzp, cyyn, cxxp, int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzp, cyyn, cxxn ,  int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzn, cyyp, cxxp , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzn, cyyp, cxxn ,  int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzn, cyyn, cxxp , int (360 / (radius2 + 1)), 1, br);
              cube.HSV(czzn, cyyn, cxxn ,  int (360 / (radius2 + 1)), 1, br);

            }


          }
        }
      }
      cube.drawVoxels();
      delay(shiftDelay);
    }



    ///////////////////////
  } // while
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
