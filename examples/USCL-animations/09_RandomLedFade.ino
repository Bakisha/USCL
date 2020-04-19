


void RandomLedFade(float time, USCL & cube, unsigned char numberOfVoxels, unsigned int drawDelay)
{
  /* RandomLedAnimator parameters
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'LedCube' class that you made in your main program.
    - numberOfVoxels:  Define the number of voxels that fade.
    - shiftDelay (optional):    Define the delay (in ms) . Normally a value of
                    50 - 100 ms will suffice.
  */

  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();

  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  int maxVoxelValue = cubesizez - 1;
  float S = 1;
  float V = 1;
  int hue;

  // Declare and initialize other variables needed for the animation
  // Voxel coordinates
  uint8_t z;
  uint8_t x;
  uint8_t y;
  uint8_t br;

  // Always set the timestamp to 0
  long  timestamp = 0;
  int ledZ[numberOfVoxels];
  int ledX[numberOfVoxels];
  int ledY[numberOfVoxels];
  int ledHUE[numberOfVoxels];
  int fade_speed[numberOfVoxels];
  float ledV[numberOfVoxels];
  float ledS[numberOfVoxels];
  for (int i = 0; i < numberOfVoxels; i++)
  {

    ledZ[i] = random(cubesizez);
    ledX[i] = random(cubesizex);
    ledY[i] = random(cubesizey);
    ledHUE[i] =  random(360 );
    fade_speed[i] = 1 + random(max_brightness);
    ledV[i] = 100.00;
    ledS[i] = 0.0;
  }



  long startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    // draw leds
    for (int i = 0; i < numberOfVoxels; i++)
    {
      cube.HSV(ledZ[i], ledX[i], ledY[i], ledHUE[i], ledS[i] / 100.0, ledV[i] / 100.0); // Set a voxel brightness in the current frame with the random generated z, x, y coordinates
    }
    cube.drawVoxels(); // Draw the frame to the cube explicitly using vsync
    delay(drawDelay);

    for (int i = 0; i < numberOfVoxels; i++)
    {
      if (ledS[i] <= 100.0)
      {
        ledS[i] = ledS[i] + fade_speed[i];
      }
      else
      {
        ledV[i] = ledV[i] - fade_speed[i];
        if (ledV[i] < 0) // led has faded, calculate new led
        {
          ledZ[i] = random(cubesizez);
          ledX[i] = random(cubesizex);
          ledY[i] = random(cubesizey);
          ledHUE[i] = random(360 );
          fade_speed[i] = 1 + random(max_brightness);
          ledV[i] = 100.00;
          ledS[i] = 0.0;
        }
      }
    }




  }//while
} //void
