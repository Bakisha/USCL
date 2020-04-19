
// name zz_ so this tab will be last (and setup and loop must be in last tab)


void setup() {

  //Start the cube
  pinMode(pinOE, OUTPUT);
  digitalWrite(pinOE, HIGH); // untill all is initialized, turn off leds
  cube.begin();

}



void loop() {



  // list of animations. Comment out animations if it can't fit in flash memory
  int cubesize = cube.getCubeSizeZ();
  int frames =  cube.getFPS();
  int delay_FPS = 1000 / frames;
  int j = 1 + random(8);

  rotating_circles(animtime, cube,   0 * delay_FPS); // animation time lenght, cube, delay time
  plasma6(animtime, cube ,  0 ); // animation time lenght, cube, delay in frames (if it's too fast)
  rotating_star(animtime, cube,   0 * delay_FPS); // animation time lenght, cube, delay time
  rotating_cube(animtime, cube,   0 * delay_FPS); // animation time lenght, cube, delay time
  plasma_sinx2y2(animtime, cube, 0 * delay_FPS); // animation time lenght, cube, delay time
  plasma_sineWave(animtime, cube, 2 * delay_FPS, j * cubesize); // shiftDelay ; period = *8
  plasma6(animtime, cube ,  0 ); // animation time lenght, cube, delay in frames (if it's too fast)
  plasma5(animtime * 3, cube, 0); // animatime,cube, ms delay
  plasma4(animtime * 3, cube, 0); // animatime,cube, ms delay
  color_wheelTWO(animtime, cube, delay_FPS * frames / 8); // animation time lenght, cube, delay time
  sineWaveThree(animtime, cube, 1.5 * delay_FPS, j * cubesize, j  * cubesize); // animation time lenght, cube, delay time, period ; baseWavePeriodicFactor
  sineWave(animtime, cube, delay_FPS, j * cubesize); // shiftDelay ; period = *8
  RandomLedFade(animtime, cube,  j * cubesize,  2 * delay_FPS); // animation time lenght, cube, number of fading leds, delay time
  sineWaveTwo(animtime, cube, 1.5 * delay_FPS, 24, 32); // animation time lenght, cube, delay time,  periodX ; periodY
  snake(animtime, cube, 120, j + cubesize , 2 * delay_FPS); //animation time lenght, cube, delay time,snake lenght , collision mode
  plasma3(animtime, cube, delay_FPS);// animatime,cube, ms delay
  wipeout(animtime, cube, 3.5 * delay_FPS); // animation time lenght, cube, delay time
  MY_4D_function2(animtime, cube, delay_FPS);// animatime,cube, ms delay
  MY_4D_function1(animtime, cube, 2 * delay_FPS); // animation time lenght, cube, delay time
  explodingimplodingsphere(animtime, cube, delay_FPS); // animation time lenght, cube, delay time
  sinx2y2(animtime, cube,  delay_FPS); // animation time lenght, cube, delay time
  plasma2(animtime, cube, delay_FPS);// animatime,cube, ms delay
  plasma1(animtime, cube, 0);// animatime,cube, ms delay
  slantingLines(animtime, cube, 2 * delay_FPS, true); // animation time lenght, cube, delay time, invertDirection = false/true
  slantingLines(animtime, cube, 2 * delay_FPS, false); // animation time lenght, cube, delay time, invertDirection = false/true
  fireworks(animtime , cube, j * cubesize , 2 * delay_FPS); // animation time lenght, cube, number of particles, delay time
  cornerCube(animtime, cube, 3 * delay_FPS, 1); // animation time lenght, cube, resizeDelay ; minimumCubeSize
  rain(animtime, cube , j * cubesize ,  cube.getMaxBrightness()  , 3 * delay_FPS , false ); // animation time lenght, cube, rainVoxels; floorVoxels; rainDelay; invertDirection = false/true
  explodingsphere(animtime, cube, 3 * delay_FPS); // animation time lenght, cube, delay time
  folder(animtime, cube, 1 * delay_FPS ); // animation time lenght, cube, delay time
  starfield(animtime,  cube, cubesize * j,  3 * delay_FPS); //  animation time lenght, cube,number of stars, delay time

} // loop
