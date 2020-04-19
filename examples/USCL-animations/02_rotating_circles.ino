


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void rotating_circles(float time, USCL & cube, unsigned int drawDelay)
{
  /* rotating circles parameters
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'LedCube' class that you made in your main  loop.
    - shiftDelay (optional):    Define the delay (in ms) . Normally a value of
                    50 - 100 ms will suffice.
  */

  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();

  int max_brightness = cube.getMaxBrightness();
  //  int maxVoxelValue = cubesize - 1;
  int FPS = cube.getFPS() ;
  int delay_FPS = 1000 / FPS;
  float S = 1.0;
  float V = 1.0;
  int hue = 0;


  // Declare and initialize other variables needed for the animation

  // 3d matrix for points to draw
  ///////////////////////////////////////////// numbers to play with :-) ///////////////////////////////////////////////
  const int number_of_points = 9; //  total number of points to draw
  int  number_of_circles = 2; //(cubesizex / 4) + 1; // number of rotating circles
  float angle_speed_x = 1.0 * 2.0 * PI / (2.0 * FPS) ; // speed of x,y,z rotation // variable number is amount of seconds that axis rotate in 1 second
  float angle_speed_y = 1.0 * 2.0 * PI / (90.0 * FPS); // experiment with these values for best effect
  float angle_speed_z = 1.0 * 2.0 * PI / (30.0 * FPS);
  float zoom = 1.0 ; // variable for zooming in/out // for now, it is not per x,y,z, but one for all
  float zoom_speed = 0.001;
  int zoom_direction = -1;
  float zoom_min = 0.5;
  float zoom_max = 1.2; // try to keep points inside cubesize value
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  float half_cubesize = float(cubesizez / 2.0);
  // matrix for initial points to be drawn, in this case, it's a   ----   circle  -----
  float point [number_of_points] [3] =

    // a float matrix, must be same number as integer number_of_points
  {
    {  sin(0.00 * PI / 8.00), 0, cos(0.00 * PI / 8.00)},
    {  sin(2.00 * PI / 8.00), 0, cos(2.00 * PI / 8.00)},
    {  sin(4.00 * PI / 8.00), 0, cos(4.00 * PI / 8.00)},
    {  sin(6.00 * PI / 8.00), 0, cos(6.00 * PI / 8.00)},
    {  sin(8.00 * PI / 8.00), 0, cos(8.00 * PI / 8.00)},
    {  sin(10.00 * PI / 8.00), 0, cos(10.00 * PI / 8.00)},
    {  sin(12.00 * PI / 8.00), 0, cos(12.00 * PI / 8.00)},
    {  sin(14.00 * PI / 8.00), 0, cos(14.00 * PI / 8.00)},
    {  sin(0.00 * PI / 8.00), 0, cos(0.00 * PI / 8.00)}
  };  // -1, +1 boundary
  float rotated_points [number_of_points] [3]; // matrix to store calculated 3d points // value = number of current point, x , y ,z
  int xx[number_of_points], yy[number_of_points], zz[number_of_points]; // array for actuate coordinates in led cube for number of points
  float angle_x ; // variables for rotations around x,y,z axis
  float angle_y ;
  float angle_z ;


  // Voxel coordinates

  float x, x1, x2, x3;
  float y, y1, y2, y3;
  float z, z1, z2, z3;
  //  int xx, yy, zz;

  int hue_fps = 0 ; // color
  int i, j; // temps and counters
  float temp1, temp2, temp3;



  float startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    // color of animation
    hue_fps = hue_fps + 1;
    if (hue_fps >  FPS)
    {
      hue_fps = 0;
      hue = hue +  1;
      if (hue > 360 / number_of_circles )
      {
        hue = 0;
      }
    } // change hue once in 1 second


    for (j = 0; j < number_of_circles; j++)
    {

      temp1 = angle_x;
      temp2 = angle_y;
      temp3 = angle_z;
      angle_x = angle_x + float(j * 2.0  * PI / ( 2.0 * number_of_circles));
      angle_y = angle_y + float(j * 2.0  * PI / ( 2.0 * number_of_circles));
      angle_z = angle_z + float(j * 2.0  * PI / ( 2.0 * number_of_circles));

      for (i = 0; i < number_of_points ; i++)
      {
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

      // connect dots - connect (i,(i+1) ),last-0
      for (i = 0; i < number_of_points - 1 ; i++) // for connecting 4 sides of cube
      {
        x1 = xx [i] ;
        x2 = xx [i + 1 ] ;
        y1 = yy [i];
        y2 = yy [i + 1] ;
        z1 = zz [i] ;
        z2 = zz [i + 1] ;
        // now draw a line from point to point
        cube.HSV_line(z1, x1, y1, z2, x2, y2, hue + j * 360 / number_of_circles, S, V);

      } // i

      angle_x  = temp1 ;
      angle_y = temp2 ;
      angle_z = temp3;

    } // j // do it -number_of_circles- times


    ////////////////////////////////////////// draw leds
    cube.drawVoxels(); // Draw the frame to the cube explicitly using vsync
    if (drawDelay > 0) {
      delay(drawDelay);
    }
    /////////////////////////////////////////////////

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

    zoom = zoom + zoom_direction * zoom_speed;
    if (zoom < zoom_min || zoom > zoom_max)
    {
      zoom_direction = -zoom_direction;
    }



  }//while
} //void
