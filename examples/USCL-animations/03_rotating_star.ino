

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void rotating_star(float time, USCL & cube, unsigned int drawDelay)
{
  /* rotating star parameters
    - time:                 Define the length of time that the animation should be executed.
    - cube:            Define the instance of the 'LedCube' class that you made in your main  loop.
    - shiftDelay (optional):    Define the delay (in ms) . Normally a value of
                    50 - 100 ms will suffice.
  */

  byte cubesizex = cube.getCubeSizeX();
  byte cubesizey = cube.getCubeSizeY();
  byte cubesizez = cube.getCubeSizeZ();

  int max_brightness = cube.getMaxBrightness();
  int FPS = cube.getFPS() / 2;
  int delay_FPS = 1000 / FPS;
  // int maxVoxelValue = cubesize - 1;
  float S = 1.0;
  float V = 1.0;
  int hue = 0;

  // Declare and initialize other variables needed for the animation

  // 3d matrix for points to draw
  float half_cubesize = float(cubesizez / 2.0);
  const int number_of_points_star = 11; //  total number of points to draw
  float inner_r = 0.5; // iner circle of a star
  float outer_r = 1.0; // outer circle of a star
  // star coordinates taken from https://www.codeproject.com/Articles/18149/Draw-a-US-Flag-using-C-and-GDI

  // matrix for initial points to be drawn, in this case, it's a   ----   STAR  -----
  float point [number_of_points_star] [3] = { // a float matrix, must be same number as integer number_of_points

    { 0.0, -outer_r, 0.0},
    {  inner_r * float(sin(2.0 * PI / 10.0)), -inner_r *  float(cos(2.0 * PI / 10.0)), 0.0},
    {  outer_r *  float(sin(2 * PI / 5.0)), -outer_r *  float(cos(2.0 * PI / 5.0)), 0.0},
    { inner_r *  float(sin(2.0 * PI / 5.0)), inner_r *  float(cos(2.0 * PI / 5.0)), 0.0},
    { outer_r *  float(sin(2.0 * PI / 10.0)), outer_r *  float(cos(2.0 * PI / 10.0)), 0.0},
    {  0.0, inner_r, 0.0},
    {  -outer_r *  float(sin(2.0 * PI / 10.0)), outer_r *  float(cos(2.0 * PI / 10.0)), 0.0},
    { -inner_r *  float(sin(2.0 * PI / 5.0)), inner_r *  float(cos(2.0 * PI / 5.0)), 0.0},
    { -outer_r *  float(sin(2.0 * PI / 5.0)),   -outer_r *  float(cos(2.0 * PI / 5.0)), 0.0},
    { -inner_r *  float(sin(2.0 * PI / 10.0)),  -inner_r *  float(cos(2.0 * PI / 10.0)), 0.0},
    { 0.0, -outer_r, 0.0}
  };  // star is in -1, +1 boundary


  float rotated_points [number_of_points_star] [3]; // matrix to store calculated 3d points // value = number of current point, x , y ,z
  int xx[number_of_points_star], yy[number_of_points_star], zz[number_of_points_star]; // array for actuate coordinates in led cube for number of points
  float angle_x = 0; // variables for rotations around x,y,z axis
  float angle_y = 0;
  float angle_z = 0;


  float angle_speed_x =  2 * PI / (2 * FPS) ; //  speed of x,y,z rotation // variable number is amount of seconds that axis rotate in 1 second
  float angle_speed_y =  2 * PI / (90 * FPS); // experiment with these values for best effect
  float angle_speed_z =  2 * PI / (180 * FPS); // try to keep points inside cubesize value
  float zoom = 1.0 ; // variable for zooming in/out
  float zoom_speed = 0.0001;
  int zoom_direction = -1;


  // Voxel coordinates

  float x, x1, x2, x3;
  float y, y1, y2, y3;
  float z, z1, z2, z3;
  //  int xx, yy, zz;

  int hue_fps = 0; // color
  int i, j; // temps and counters


  long startTime = millis();
  while (millis() - startTime < time)
  {
    ///////////////////

    for (i = 0; i < number_of_points_star; i++) {

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
    // color of animation
    hue_fps = hue_fps + 1;
    if (hue_fps >  FPS)
    {
      hue_fps = 0;
      hue = hue +  1; // max_brightness / 4;
      if (hue > 360 ) {
        hue = 0;
      }
    } // change hue once in 1 second

    // calculate x,y,z coordinates for led cube
    for (i = 0; i < number_of_points_star; i++)
    {
      xx[i] = zoom * ((rotated_points [i] [0]) * half_cubesize) + half_cubesize;
      yy[i] = zoom * ((rotated_points [i] [1]) * half_cubesize) + half_cubesize;
      zz[i] = zoom * ((rotated_points [i] [2]) * half_cubesize) + half_cubesize;
    } //

    // connect dots - connect (i,(i+1) ),last-0
    for (i = 0; i < number_of_points_star - 1; i++) // for connecting 4 sides of cube
    {
      x1 = xx [i] ;
      x2 = xx [i + 1 ] ;
      y1 = yy [i];
      y2 = yy [i + 1] ;
      z1 = zz [i] ;
      z2 = zz [i + 1] ;
      // now draw a line from point to point
      cube.HSV_line(z1, x1, y1, z2, x2, y2, hue , S, V);
    } // i


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
    if (zoom < 0.6 || zoom > 1.1)
    {
      zoom_direction = -zoom_direction;
    }


    ////////////////////////////////////////// draw leds
    cube.drawVoxels(); // Draw the frame to the cube explicitly using vsync
    if (drawDelay > 0) {
      delay(drawDelay);
    }
    /////////////////////////////////////////////////


  }//while
} //void
