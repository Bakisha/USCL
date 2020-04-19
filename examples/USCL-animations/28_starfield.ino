


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void starfield(float time, USCL & cube, uint8_t stars,  uint16_t shiftDelay)
{
  /* starfield parameters
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'USCL' class that you made in your main program.
    - numberOfVoxels:  Define the number of voxels that should be drawn each frame.
    - shiftDelay (optional):    Define the delay (in ms) . Normally a value of
                  50 - 100 ms will suffice.
  */
  // Declare and initialize other variables needed for the animation
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();

  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  float star_x [stars];
  float star_y [stars];
  float star_z [stars];
  float star_speed[stars];
  int star_hue[stars];

  int i;
  int trail;
  // Other variables
  for (i = 0; i < stars ; i++) {
    star_x [i] = random(cubesizex);
    star_y [i] = cubesizey + random(cubesizey); // field move along Y axis
    star_z [i] = random(cubesizez);
    star_speed[i] = 0.3 + (float((random(150) / 150.0))); // speed of individual stars
    star_hue[i] = random(360); // random color
  }


  float  startTime = millis();
  while (millis() - startTime < time)
  {

    //    cube.HSV(z, y, x,hue, 1, brightness);
    ///////////////////

    // display stars:
    for (i = 0 ; i < stars; i++)
    {
      for ( trail = 0 ; trail <= (cubesizey ); trail++)
      {

        if (star_y[i] + trail < cubesizey)
        {
          cube.HSV(star_z[i], star_x[i], star_y[i] + trail,  star_hue[i] , 1.0, float((max_brightness - (max_brightness / (1.0 + trail)  )) / float(max_brightness) )  );
        }
      }// trail
    }// i

    delay(shiftDelay);
    cube.drawVoxels();

    // star calculations:

    for (i = 0; i < stars; i++) {

      star_y[i] = star_y[i] - star_speed[i];

      if (star_y[i] <= -cubesizey)
      { // star has "passed", calculate new star position

        star_x [i] = random(cubesizex);
        star_y [i] = cubesizey + random(cubesizey); // field move along Y axis
        star_z [i] = random(cubesizez);
        star_speed[i] = float(0.05) + (float((random(float(10)) / float(10)))); // speed of individual stars
        star_hue[i] = random(360);
      }



    } // i stars













    //    delay(shiftDelay);
    ///////////////////////
  } // while


} // void
