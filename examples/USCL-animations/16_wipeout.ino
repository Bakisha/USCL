

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------





void    wipeout(float time, USCL & cube, unsigned int drawDelay)
{
  /* wipeout parameters
    - time:                     Define the length of time that the animation should be executed.
    - cubeRef:                Define the instance of the 'LedCube' class that you made in your main program.
    - timeDelay (optional):     Define the delay (in ms) between each iteration of the rain drops falling down. Normally a value of
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


  int xxx = 0, yyy = 0, zzz = 0;
  int fx = random(cubesizex), fy = random(cubesizey), fz = random(cubesizez), direct, fxm = 1, fym = 1, fzm = 1, fxo = 0, fyo = 0, fzo = 0;
  int  ftx = random(cubesizex), fty = random(cubesizey), ftz = random(cubesizez), ftxm = 1, ftym = 1, ftzm = 1, ftxo = 0, ftyo = 0, ftzo = 0;
  int select, rr, gg, bb, rrt, ggt, bbt;

  rr = random(0, max_brightness);
  delay(1);
  gg = random(0, max_brightness);
  delay(1);
  bb = random(0, max_brightness);
  delay(1);

  rrt = random(0, max_brightness);
  delay(1);
  ggt = random(0, max_brightness);
  delay(1);
  bbt = random(0, max_brightness);
  delay(1);

  long  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////


    cube.RGB(ftx, fty, ftz,  rr, gg, bb);
    cube.RGB(ftx, fty, ftz + 1, rr, gg, bb);
    cube.RGB(ftx, fty, ftz - 1,  rr, gg, bb);
    cube.RGB(ftx + 1, fty, ftz, rr, gg, bb);
    cube.RGB(ftx - 1, fty, ftz, rr, gg, bb);
    cube.RGB(ftx, fty + 1, ftz, rr, gg, bb);
    cube.RGB(ftx, fty - 1, ftz, rr, gg, bb);

    cube.RGB(fx, fy, fz,  rrt, ggt, bbt);
    cube.RGB(fx, fy, fz + 1,  rrt, ggt, bbt);
    cube.RGB(fx, fy, fz - 1,  rrt, ggt, bbt);
    cube.RGB(fx + 1, fy, fz,  rrt, ggt, bbt);
    cube.RGB(fx - 1, fy, fz,  rrt, ggt, bbt);
    cube.RGB(fx, fy + 1, fz, rrt, ggt, bbt);
    cube.RGB(fx, fy - 1, fz, rrt, ggt, bbt);


    cube.drawVoxels(); // Draw the frame to the cube explicitly using vsync
    delay(drawDelay);

    fxo = fx;
    fyo = fy;
    fzo = fz;

    ftxo = ftx;
    ftyo = fty;
    ftzo = ftz;

    direct = random(3);
    if (direct == 0)
      fx = fx + fxm;
    if (direct == 1)
      fy = fy + fym;
    if (direct == 2)
      fz = fz + fzm;
    if (fx < 0) {
      fx = 0; fxm = 1;
    }
    if (fx > cubesizex - 1) {
      fx = cubesizex - 1; fxm = -1;
    }
    if (fy < 0) {
      fy = 0; fym = 1;
    }
    if (fy > cubesizey - 1) {
      fy = cubesizey - 1; fym = -1;
    }
    if (fz < 0) {
      fz = 0; fzm = 1;
    }
    if (fz > cubesizez - 1) {
      fz = cubesizez - 1; fzm = -1;
    }

    direct = random(3);
    if (direct == 0)
      ftx = ftx + ftxm;
    if (direct == 1)
      fty = fty + ftym;
    if (direct == 2)
      ftz = ftz + ftzm;
    if (ftx < 0) {
      ftx = 0; ftxm = 1;
    }
    if (ftx > cubesizex - 1) {
      ftx = cubesizex - 1; ftxm = -1;
    }
    if (fty < 0) {
      fty = 0; ftym = 1;
    }
    if (fty > cubesizey - 1) {
      fty = cubesizey - 1; ftym = -1;
    }
    if (ftz < 0) {
      ftz = 0; ftzm = 1;
    }
    if (ftz > cubesizez - 1) {
      ftz = cubesizez - 1; ftzm = -1;
    }






  }//while

}//wipeout







// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
