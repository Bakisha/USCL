


void fireworks(float time, USCL & cube,  int n, int shiftDelay)
{
  /*   fireworks animation

    - time:               Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'LedCube' class that you made in your main program.
    - iterations:         Define the number of iterations
    - n         :         Define number of particles
    - shiftDelay :        Define the delay (in ms) . Normally a value of 50 - 100 ms will suffice.
  */
  // usual  variables
  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();

  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  byte maxVoxelValue = cubesizez - 1;
  float S = 1;
  float V = 1;
  int hue = 180;

  int i, f, e;

  int z, x, y;

  float origin_x = cubesizex / 2;
  float origin_y = cubesizey / 2;
  float origin_z = 0;

  int rand_y, rand_x, rand_z;

  float slowrate, gravity;

  // Particles and their position, x,y,z and their movement, dx, dy, dz
  float particles[n][6];

  long  startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////


    hue = random(360) ;


    origin_x = random(cubesizex) ;
    origin_y = random(cubesizey) ;
    origin_z = random(cubesizez) ;


    // shoot a particle up in the air
    for (e = 0; e < cubesizez; ++e)
    {
      cube.HSV( e, origin_x, origin_y, hue, 1, 1);
      //      setvoxel(origin_x,origin_y,e);

      cube.drawVoxels();
      delay(shiftDelay);

    }

    // Fill particle array
    for (f = 0; f < n; ++f)
    {
      // Position
      particles[f][0] = origin_x;
      particles[f][1] = origin_y;
      particles[f][2] = maxVoxelValue;   // origin_z;

      rand_x = random(256) % 200;
      rand_y = random(256) % 200;
      rand_z = random(256) % 200;

      // Movement
      particles[f][3] = 1.0 - (float)rand_x / 100; // dx
      particles[f][4] = 1.0 - (float)rand_y / 100; // dy
      particles[f][5] = 1.0 - (float)rand_z / 100; // dz
    }

    // explode
    for (e = 0; e < 25; ++e)
    {
      slowrate = 1 + tan((e + 0.1) / 20.0) * 25.0;

      gravity = tan((e + 0.1) / 20.0) / 2.0;

      for (f = 0; f < n; ++f)
      {
        particles[f][0] += particles[f][3] / slowrate;
        particles[f][1] += particles[f][4] / slowrate;
        particles[f][2] += particles[f][5] / slowrate;
        particles[f][2] -= gravity;


        z = particles[f][2];
        x = particles[f][0];
        y = particles[f][1];

        if (x < 0)
        {
          x = 0;
        }
        if (y < 0)
        {
          y = 0;
        }

        if (x > cubesizex - 1)
        {
          x = cubesizex - 1;
        }
        if (y > cubesizey - 1)
        {
          y = cubesizey - 1;
        }


        if (z > maxVoxelValue)
        {
          z = maxVoxelValue;
        }
        V = float(1.0 / 10.0 + (z * maxVoxelValue / 10.0) );

        if (z >= 0)
        {

          // cube.HSV(particles[f][2], particles[f][0], particles[f][1],   hue, 1.0, float( (max_brightness/(cubesize/2))+ max_brightness *    particles[f][2] / (cubesize/2) )   );

          cube.HSV(z, x, y,   hue, 1.0 , V);

        } // z

      } //

      cube.drawVoxels();
      delay(shiftDelay);

    } //e

  } //while



}//void
