

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void color_wheelTWO(float time, USCL & cube, uint16_t shiftDelay /* = 75 */)
{
  /*
    color_wheelTWO cube parameters
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'LedCube' class that you made in your main program.
    - shiftDelay (optional):    Define the delay (in ms) . Normally a value of
                    50 - 100 ms will suffice.
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
  int hue;


  //*****colorWheelTwo*****colorWheelTwo*****colorWheelTwo*****colorWheelTwo*****colorWheelTwo
  int xx, yy, zz, ww, rr = 1, gg = 1, bb = 1, ranx, rany , ranz, select, swiper;


  long  startTime = millis();
  while (millis() - startTime < time)
  {

    swiper = random(6);

    hue =  random(360 );




    if (swiper == 0) {
      for (yy = 0; yy < cubesizey; yy++) { //left to right
        for (xx = 0; xx < cubesizex; xx++) {
          for (zz = 0; zz < cubesizez; zz++) {
            cube.HSV(zz, yy, xx, hue, 1, 1);
          }
        }
        cube.drawVoxels();
        delay(shiftDelay);
      }
    }
    if (swiper == 1) { //bot to top
      for (xx = 0; xx < cubesizex; xx++) {
        for (yy = 0; yy < cubesizey; yy++) {
          for (zz = 0; zz < cubesizez; zz++) {
            cube.HSV(zz, yy, xx, hue, 1, 1);
          }
        }
        cube.drawVoxels();
        delay(shiftDelay);
      }
    }
    if (swiper == 2) { //back to front
      for (zz = 0; zz < cubesizez; zz++) {
        for (xx = 0; xx < cubesizex; xx++) {
          for (yy = 0; yy < cubesizey; yy++) {
            cube.HSV(zz, yy, xx, hue, 1, 1);
          }
        }
        cube.drawVoxels();
        delay(shiftDelay);
      }
    }
    if (swiper == 3) {
      for (yy = cubesizey - 1; yy >= 0; yy--) { //right to left
        for (xx = 0; xx < cubesizex; xx++) {
          for (zz = 0; zz < cubesizez; zz++) {
            cube.HSV(zz, yy, xx, hue, 1, 1);
          }
        }
        cube.drawVoxels();
        delay(shiftDelay);
      }
    }
    if (swiper == 4) { //top to bot
      for (xx = cubesizex - 1; xx >= 0; xx--) {
        for (yy = 0; yy < cubesizey; yy++) {
          for (zz = 0; zz < cubesizez; zz++) {
            cube.HSV(zz, yy, xx, hue, 1, 1);
          }
        }
        cube.drawVoxels();
        delay(shiftDelay);
      }
    }
    if (swiper == 5) { //front to back
      for (zz = cubesizez - 1; zz >= 0; zz--) {
        for (xx = 0; xx < cubesizex; xx++) {
          for (yy = 0; yy < cubesizey; yy++) {
            cube.HSV(zz, yy, xx, hue, 1, 1);
          }
        }
        cube.drawVoxels();
        delay(shiftDelay);
      }
    }




  }//while

}//color wheel

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
