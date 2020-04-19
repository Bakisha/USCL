

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void rotating_cube(float time, USCL & cube, unsigned int drawDelay)
{
  /* rotating_cube parameters
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'LedCube' class that you made in your main  loop.
    - shiftDelay (optional):    Define the delay (in ms) . Normally a value of
                    50 - 100 ms will suffice.
  */

  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();

  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  int max_brightness = cube.getMaxBrightness();
  //  int maxVoxelValue = cubesize - 1;
  float S = 1.0;
  float V = 1.0;
  int hue = 0;

  // Declare and initialize other variables needed for the animation

  // 3d matrix for points to draw
  float half_cubesize = cubesizez / 2.0;
  int number_of_points = 8; //  total number of points to draw

  // matrix for initial points to be drawn, in this case, it's a   ----   CUBE  -----
  float point [8] [3] = { // a float matrix, must be same number as integer number_of_points
    { -1, -1, -1},
    {  1, -1, -1},
    {  1, 1, -1},
    { -1, 1, -1},
    { -1, -1, 1},
    {  1, -1, 1},
    {  1, 1, 1},
    { -1, 1, 1}
  };  // cube is in -1, +1 boundary


  float rotated_points [8] [3]; // matrix to store calculated 3d points // value = number of current point, x , y ,z
  int xx[8], yy[8], zz[8]; // array for actuate coordinates in led cube for number of points
  float angle_x = 0; // variables for rotations around x,y,z axis
  float angle_y = 0;
  float angle_z = 0;


  float angle_speed_x =  2 * PI / (3 * FPS) ; // 120.00; // speed of x,y,z rotation // variable number is amount of seconds that axis rotate in 1 second
  float angle_speed_y =  2 * PI / (60 * FPS); // experiment with these values for best effect
  float angle_speed_z =  3 * PI / (180 * FPS); // try to keep points inside cubesize value
  float zoom = 0.7 ; // variable for zooming in/out


  // Voxel coordinates

  float x, x1, x2, x3;
  float y, y1, y2, y3;
  float z, z1, z2, z3;
  //  int xx, yy, zz;

  int hue_fps = 0 ; // color
  int i, j; // temps and counters



  long startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////
    for (i = 0; i < number_of_points; i++) {

      x = point [i] [0]; // x from point matrix
      y = point [i] [1]; // y from point matrix
      z = point [i] [2]; // z from point matrix


      // rotating x axis
      x1 = x;
      y1 = cos(angle_x) * y - sin(angle_x) * z;
      z1 = sin(angle_x) * y + cos(angle_x) * z;

      // rotating y axis
      x2 = cos(angle_y) * x1 + sin(angle_y) * z1;
      y2 = y1;
      z2 = -sin(angle_y) * x1 + cos(angle_y) * z1;

      // rotating z axis

      x3 = cos(angle_z) * x2 - sin(angle_z) * y2;
      y3 = sin(angle_z) * x2 + cos(angle_z) * y2;
      z3 = z2;

      rotated_points [i] [0] = x3; // rotated x point
      rotated_points [i] [1] = y3; // rotated y point
      rotated_points [i] [2] = z3; // rotated z point

    } // number of points


    // calculate x,y,z coordinates for led cube
    for (i = 0; i < number_of_points; i++)
    {
      xx[i] = zoom * ((rotated_points [i] [0]) * half_cubesize) + half_cubesize;
      yy[i] = zoom * ((rotated_points [i] [1]) * half_cubesize) + half_cubesize;
      zz[i] = zoom * ((rotated_points [i] [2]) * half_cubesize) + half_cubesize;
    } //
    // color of animation
    hue_fps = hue_fps + 1;
    if (hue_fps >  FPS)
    {
      hue_fps = 0;
      hue = hue +  cubesizez / 2;
      if (hue > 360 / 2) {
        hue = 0;
      }
    } // change hue once in 1 second

    // connect dots (square sides of a cube)
    // 0-1
    // 1-2
    // 2-3
    // 3-0
    //
    // 4-5
    // 5-6
    // 6-7
    // 7-4
    //
    // 0-4
    // 1-5
    // 2-6
    // 3-7
    //
    // 1-connect (i,(i+1)%4 )
    // 2-connect (i+4,((i+1)%4)+4 )
    // 3-connect(i,i+4)
    // inspiration from youtube : Coding challenge #112 24:50 https://youtu.be/p4Iz0XJY-Qk?t=1480


    // 1-connect (i,(i+1)%4 )
    for (i = 0; i < 4; i++)  // for connecting 4 sides of cube
    {
      x1 = xx [i] ;
      x2 = xx [(i + 1) % 4] ;
      y1 = yy [i];
      y2 = yy [(i + 1) % 4] ;
      z1 = zz [i] ;
      z2 = zz [(i + 1) % 4] ;
      // now draw a line from point to point

      cube.HSV_line(z1, x1, y1, z2, x2, y2, hue , S, V);
      /*
        int8_t zDirectionVector = z2 - z1;
        int8_t xDirectionVector = x2 - x1;
        int8_t yDirectionVector = y2 - y1;
        for (j = 0; j < cubesize; j++)
        {
        z = round((j / (float)maxVoxelValue) * zDirectionVector + z1);
        x = round((j / (float)maxVoxelValue) * xDirectionVector + x1);
        y = round((j / (float)maxVoxelValue) * yDirectionVector + y1);
        cube.HSV(z, x, y, (hue) % 360, S, V);
        } // j
      */
    } // i
    // 2-connect (i+4,((i+1)%4)+4 )
    for (i = 0; i < 4; i++)  // for connecting 4 sides of cube
    {
      x1 = xx [i + 4] ;
      x2 = xx [((i + 1) % 4) + 4 ] ;
      y1 = yy [i + 4];
      y2 = yy [((i + 1) % 4) + 4 ] ;
      z1 = zz [i + 4] ;
      z2 = zz [((i + 1) % 4) + 4 ] ;
      // now draw a line from point to point
      cube.HSV_line(z1, x1, y1, z2, x2, y2, hue + 180, S, V);
      /*
        int8_t zDirectionVector = z2 - z1;
        int8_t xDirectionVector = x2 - x1;
        int8_t yDirectionVector = y2 - y1;
        for (j = 0; j < cubesize; j++)
        {
        z = round((j / (float)maxVoxelValue) * zDirectionVector + z1);
        x = round((j / (float)maxVoxelValue) * xDirectionVector + x1);
        y = round((j / (float)maxVoxelValue) * yDirectionVector + y1);
        cube.HSV(z, x, y, (hue + 120) % 360, S, V);
        } // j
      */
    } // i
    /*
        //  3-connect(i,i+4)
        for (i = 0; i < 4; i++)  // for connecting 4 sides of cube
        {
          x1 = xx [i] ;
          x2 = xx [i + 4] ;
          y1 = yy [i];
          y2 = yy [i + 4] ;
          z1 = zz [i] ;
          z2 = zz [i + 4] ;
          // now draw a line from point to point
          cube.HSV_line(z1, x1, y1, z2, x2, y2, (hue + 240) % 360, S, V);
          /*

            int zDirectionVector = z2 - z1;
            int xDirectionVector = x2 - x1;
            int yDirectionVector = y2 - y1;
            for (j = 0; j < cubesize; j++)
            {
            z = ((j / (float)maxVoxelValue) * zDirectionVector + z1);
            x = ((j / (float)maxVoxelValue) * xDirectionVector + x1);
            y = ((j / (float)maxVoxelValue) * yDirectionVector + y1);

            cube.HSV(z, x, y, (hue + 240) % 360, S, V);
            } // j
    */
    /*
       } // i

    */

    angle_x = angle_x + angle_speed_x;
    if (angle_x > 2 * PI) {
      angle_x = 0;
    }
    angle_y = angle_y + angle_speed_y;
    if (angle_y > 2 * PI) {
      angle_y = 0;
    }
    angle_z = angle_z + angle_speed_z;
    if (angle_z > 2 * PI) {
      angle_z = 0;
    }


    ////////////////////////////////////////// draw leds
    cube.drawVoxels(); // Draw the frame to the cube explicitly using vsync
    delay(drawDelay);
    /////////////////////////////////////////////////

  }//while
} //void
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
