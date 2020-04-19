

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




void snake(float time, USCL & cube, int shiftDelay, uint16_t iterations,  unsigned char mode)
{

  /*   snake animation ****NOTE ****

    - time:               Define the length of time that the animation should be executed.
    - cube:               Define the instance of the 'LedCube' class that you made in your main program.
    - shiftDelay :        Define the delay (in ms) . Normally a value of 50 - 100 ms will suffice.
    - iterations:         Define the number of iterations
    - mode :              Define mode (1 - enable ; 0 - disable): bit1 - crash action  ; bit2 - avoid corners
    - drawmode :          Define drawmode : 0 - pixel snake ; 1 - draw snake
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


  //  fill(0x00);   // Blank the cube

  int x, y, z;    // Current coordinates for the point
  int dx, dy, dz; // Direction of movement
  int lol, i;   // lol?
  unsigned char crash_x, crash_y, crash_z;

  y = random(cubesizey) % (cubesizey - 1);
  x = random(cubesizex) % (cubesizex - 1);
  z = random(cubesizez) % (cubesizez - 1);

  // Coordinate array for the snake.
  int snake[iterations][3];
  for (i = 0; i < iterations; ++i)
  {
    snake[i][0] = x;
    snake[i][1] = y;
    snake[i][2] = z;
  }
  dx = 1;
  dy = 1;
  dz = 1;

  int hue [iterations];
  int start_hue = random(180);
  int end_hue = 180 + random(180);
  int step_hue = (end_hue - start_hue) / iterations;


  for (i = 0; i < iterations ; i++)
  {
    hue [i] = start_hue + (i * step_hue);
  }


  long  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////




    crash_x = 0;
    crash_y = 0;
    crash_z = 0;


    // Let's mix things up a little:
    if (random(cubesizez) % 3 == 0)
    {
      // Pick a random axis, and set the speed to a random number.
      lol = random(cubesizez) % 3;
      if (lol == 0)
        dx = random(cubesizex) % 3 - 1;

      if (lol == 1)
        dy = random(cubesizey) % 3 - 1;

      if (lol == 2)
        dz = random(cubesizez) % 3 - 1;
    }

    // The point has reached 0 on the x-axis and is trying to go to -1
    // aka a crash
    if (dx == -1 && x == 0)
    {
      crash_x = 0x01;
      if (random(cubesizex) % 3 == 1)
      {
        dx = 1;
      } else
      {
        dx = 0;
      }
    }

    // y axis 0 crash
    if (dy == -1 && y == 0)
    {
      crash_y = 0x01;
      if (random(cubesizey) % 3 == 1)
      {
        dy = 1;
      } else
      {
        dy = 0;
      }
    }

    // z axis 0 crash
    if (dz == -1 && z == 0)
    {
      crash_z = 0x01;
      if (random(cubesizez) % 3 == 1)
      {
        dz = 1;
      } else
      {
        dz = 0;
      }
    }

    // x axis 7 crash
    if (dx == 1 && x == (cubesizex - 1))
    {
      crash_x = 0x01;
      if (random(cubesizex) % 3 == 1)
      {
        dx = -1;
      } else
      {
        dx = 0;
      }
    }

    // y axis 7 crash
    if (dy == 1 && y == (cubesizey - 1))
    {
      crash_y = 0x01;
      if (random(cubesizey) % 3 == 1)
      {
        dy = -1;
      } else
      {
        dy = 0;
      }
    }

    // z azis 7 crash
    if (dz == 1 && z == (cubesizez - 1))
    {
      crash_z = 0x01;
      if (random(cubesizez) % 3 == 1)
      {
        dz = -1;
      } else
      {
        dz = 0;
      }
    }

    // mode bit 0 sets crash action enable
    if (mode | 0x01)
    {
      if (crash_x)
      {
        if (dy == 0)
        {
          if (y == (cubesizey - 1))
          {
            dy = -1;
          } else if (y == 0)
          {
            dy = +1;
          } else
          {
            if (random(cubesizey) % 2 == 0)
            {
              dy = -1;
            } else
            {
              dy = 1;
            }
          }
        }
        if (dz == 0)
        {
          if (z == (cubesizez - 1))
          {
            dz = -1;
          } else if (z == 0)
          {
            dz = 1;
          } else
          {
            if (random(cubesizez) % 2 == 0)
            {
              dz = -1;
            } else
            {
              dz = 1;
            }
          }
        }
      }

      if (crash_y)
      {
        if (dx == 0)
        {
          if (x == (cubesizex - 1))
          {
            dx = -1;
          } else if (x == 0)
          {
            dx = 1;
          } else
          {
            if (random(cubesizex) % 2 == 0)
            {
              dx = -1;
            } else
            {
              dx = 1;
            }
          }
        }
        if (dz == 0)
        {
          if (z == 3)
          {
            dz = -1;
          } else if (z == 0)
          {
            dz = 1;
          } else
          {
            if (random(cubesizez) % 2 == 0)
            {
              dz = -1;
            } else
            {
              dz = 1;
            }
          }
        }
      }

      if (crash_z)
      {
        if (dy == 0)
        {
          if (y == (cubesizey - 1))
          {
            dy = -1;
          } else if (y == 0)
          {
            dy = 1;
          } else
          {
            if (random(cubesizey) % 2 == 0)
            {
              dy = -1;
            } else
            {
              dy = 1;
            }
          }
        }
        if (dx == 0)
        {
          if (x == (cubesizex - 1))
          {
            dx = -1;
          } else if (x == 0)
          {
            dx = 1;
          } else
          {
            if (random(cubesizex) % 2 == 0)
            {
              dx = -1;
            } else
            {
              dx = 1;
            }
          }
        }
      }
    }

    // mode bit 1 sets corner avoid enable
    if (mode | 0x02)
    {
      if (  // We are in one of 8 corner positions
        (x == 0 && y == 0 && z == 0) ||
        (x == 0 && y == 0 && z == (cubesizez - 1)) ||
        (x == 0 && y == (cubesizey - 1) && z == 0) ||
        (x == 0 && y == (cubesizey - 1) && z == (cubesizez - 1)) ||
        (x == (cubesizex - 1) && y == 0 && z == 0) ||
        (x == (cubesizex - 1) && y == 0 && z == (cubesizez - 1)) ||
        (x == (cubesizex - 1) && y == (cubesizey - 1) && z == 0) ||
        (x == (cubesizex - 1) && y == (cubesizey - 1) && z == (cubesizez - 1))
      )
      {
        // At this point, the voxel would bounce
        // back and forth between this corner,
        // and the exact opposite corner
        // We don't want that!

        // So we alter the trajectory a bit,
        // to avoid corner stickyness
        lol = random(cubesizez) % 3;
        if (lol == 0)
          dx = 0;

        if (lol == 1)
          dy = 0;

        if (lol == 2)
          dz = 0;
      }
    }

    // one last sanity check
    if (x == 0 && dx == -1)
      dx = 1;

    if (y == 0 && dy == -1)
      dy = 1;

    if (z == 0 && dz == -1)
      dz = 1;

    if (x == (cubesizex - 1) && dx == 1)
      dx = -1;

    if (y == (cubesizey - 1) && dy == 1)
      dy = -1;

    if (z == (cubesizez - 1) && dz == 1)
      dz = -1;


    // Finally, move the voxel.
    x = x + dx;
    y = y + dy;
    z = z + dz;


    for (i = iterations - 1; i >= 0; --i)
    {
      snake[i][0] = snake[i - 1][0];
      snake[i][1] = snake[i - 1][1];
      snake[i][2] = snake[i - 1][2];
    }
    snake[0][0] = x;
    snake[0][1] = y;
    snake[0][2] = z;

    for (i = 0; i < iterations; ++i)
    {
      cube.HSV(snake[i][2],  snake[i][1], snake[i][0],  hue[i], 1, 1); //(snake[i][0], snake[i][1], snake[i][2]);
    }


    cube.drawVoxels();

    delay(shiftDelay);







  }//while
} // void



// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
