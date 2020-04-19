
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
  
   
  
   My_animation1(animtime, cube ,  0*delay_FPS ); // animation time lenght, cube, delay in frames (if it's too fast)

} // loop
