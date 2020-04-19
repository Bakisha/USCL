/*
  USCL.cpp - Library for controlling shift register driven LED cubes
  Copyright (C) 2015  Mike Neuhaus.
  Edited for bluepill STM32 board by Bakisha 2018

  This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

// for 4x4x4 cube, sending data is about 20uS for 4bit BAM
// for 8x8x8 cube, sending data is about 45uS for 4bit BAM

// 1 frame = pow(2,BAM_BIT_DEPTH) * cubesize * 2



#include <USCL.h>
#include <SPI.h>
#include <inttypes.h>
#include <math.h>

#define CPU_MHZ (F_CPU/1000000)





#ifdef AVR // arduino 
#include <Arduino.h>
const Prescaler USCL::_prescalers[] = { {1, 0b001}, {8, 0b010}, {64, 0b011}, {256, 0b100}, {1024, 0b101} };  // Largest prescaler must be the last item.
#define PRESCALER_ARR_SIZE sizeof(_prescalers)/sizeof(Prescaler)
#endif

USCL *USCL::_objPtr = 0;

// Macros

#define VOXELMAPPING_ARR_LAYER_SIZE ceil((_cubeSizeX * _cubeSizeY) / 8.0)

int _LE_pin ;  // just declare it
int _OE_pin ;
int MODULATION_BIT_DEPTH ;
uint32_t _SPI_speed;
uint8_t _cube_mode;
uint32_t now = 0; // for wemos d1 mini interrupt

// Constructor
USCL::USCL(int8_t cubeSizeX, int8_t cubeSizeY, int8_t cubeSizeZ, int8_t cube_mode , uint16_t OE_pin, uint16_t LE_pin, int16_t fps, uint16_t _MODULATION_BIT_DEPTH , const uint32_t SPI_speed )
{

  // Set variables
  _cube_mode = cube_mode;
  _SPI_speed = SPI_speed;
  _OE_pin = OE_pin;
  _LE_pin = LE_pin;
  MODULATION_BIT_DEPTH = _MODULATION_BIT_DEPTH;
  _cubeSizeX = cubeSizeX;
  _cubeSizeY = cubeSizeY;
  _cubeSizeZ = cubeSizeZ;
  _maxVoxelValueX = cubeSizeX - 1;
  _maxVoxelValueY = cubeSizeY - 1;
  _maxVoxelValueZ = cubeSizeZ - 1;
  _layerArrSize = VOXELMAPPING_ARR_LAYER_SIZE;
  _cubeArrSize = _layerArrSize * cubeSizeZ;
  _voxelMappingFrontBuffer_R = 0;
  _voxelMappingBackBuffer_R = 0;
  _voxelMappingFrontBuffer_G = 0;
  _voxelMappingBackBuffer_G = 0;
  _voxelMappingFrontBuffer_G = 0;
  _voxelMappingBackBuffer_G = 0;
  _zPositionCounter = 0;
  _modulationCounter = 0;
  _modulationOffset = 0;
  _modulationBitValue = myPow(2, _modulationOffset);
  _maxBrightness = myPow(2, MODULATION_BIT_DEPTH) - 1;
  _invokeBufferSwap = false;
  _autoClearBackBuffer = true;

  _cubeSize = _cubeSizeY;
  if (_cubeSizeX >= _cubeSizeY)
  {
    _cubeSize = _cubeSizeX;
    if (_cubeSizeZ >= _cubeSizeX)
    {
      _cubeSize = _cubeSizeZ;
    }
  }

  if (_cubeSizeY >= _cubeSizeX)
  {
    _cubeSize = _cubeSizeY;
    if (_cubeSizeZ >= _cubeSizeY)
    {
      _cubeSize = _cubeSizeZ;
    }
  }


  if (fps > 0)
    _fps = fps;
  else
    _fps = 60;


}

// Destructor
USCL::~USCL(void)
{
  // TODO: end();
}


// SPI.transfer macro
#if 0
#elif defined(USE_HAL_DRIVER)
inline void  SPI_TRANSFER (uint8_t x)  { // 0.3uS overhead between bytes when optimized // 3.25 uS unoptimized
 //SPI.transfer(x);  // send byte
  SPI1->DR = (x) ;
  while (!(SPI1->SR & SPI_SR_TXE) ); 
};
#elif defined(__STM32F1__)
inline void  SPI_TRANSFER (uint8_t x)  { // 0.375uS overhead between bytes when optimized // 1.0 uS unoptimized
 //SPI.transfer(x);  // send byte
  SPI1->regs->DR = (x) ;
  while (!(SPI1->regs->SR & SPI_SR_TXE) ); //wait until send buf is clear
};
#elif defined(AVR)
inline void  SPI_TRANSFER (uint8_t x)  { // optimized // 1.4uS overhead between bytes / 19uS untill layer is sent
  //SPI.transfer(x);  // send byte // 1.8uS between bytes / 19uS untill layer is sent
SPDR = x;       
while ( ! ( (SPSR) & (1 << (SPIF)) ) ) {} ; //wait until send buf is clear

};
#elif defined(ESP8266)
inline void  SPI_TRANSFER (uint8_t x)  { // 1.5uS overhead between bytes
  SPI.transfer(x);  // send byte
};
#endif



// Interrupt handler
inline void USCL::handleInterrupt(void) {
  if (_objPtr)    _objPtr->refreshData();
}

#ifdef USE_HAL_DRIVER
inline void USE_HAL_DRIVER_handleInterrupt(HardwareTimer*) {
  USCL::handleInterrupt();
}
#endif

#ifdef AVR
// Timer interrupt
ISR(TIMER1_COMPA_vect)
{
  USCL::handleInterrupt();
}
#endif







// Start displaying animations on the cube
void USCL::begin(void)
{

  SPI.begin();
  //SPI.setBitOrder(MSBFIRST);
  //SPI.setDataMode(SPI_MODE0);
  //SPI.setClockDivider(_SPI_speed);
  SPI.beginTransaction(SPISettings(_SPI_speed, MSBFIRST, SPI_MODE0)); // must be called after SPI.begin()

  
  
  noInterrupts();// kill interrupts until everybody is set up

  _objPtr = this; // Reference pointer to current instance
  allocateVoxelMapping();

  // Set pins
  pinMode(_OE_pin, OUTPUT);
  digitalWrite(_OE_pin, HIGH);
  pinMode(_LE_pin, OUTPUT);
  digitalWrite(_LE_pin, LOW);

#ifdef AVR
  //initInterrupt();
  // Calculate prescaler and timer counter

  _refreshFrequency = (pow(2, MODULATION_BIT_DEPTH) * _cubeSizeZ  * _fps);
  double period = 1.0 / _refreshFrequency;
  Prescaler prescaler = calcPrescaler(period);
  uint16_t timerCounter = period / ((double)prescaler.value / F_CPU);
  // Set timer parameters
  // Initialize timer interrupt

  //cli(); // Clear global interrupt flag
  TCNT1  = 0;
  TCCR1A = 0;
  TCCR1B = prescaler.selectBits;
  OCR1A = timerCounter;
  TCCR1B |= (1 << WGM12);
  TIMSK1 |= (1 << OCIE1A);

  //  sei(); // Set global interrupt flag
#endif

#ifdef __STM32F1__// bluepill
  // Setup IRQ Timer
  _ISR_microseconds = 1000000 / (pow(2, MODULATION_BIT_DEPTH) * _cubeSizeZ  * _fps);
  Timer2.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);
  Timer2.setPeriod(_ISR_microseconds); // in microseconds
  Timer2.attachInterrupt(TIMER_CH1, handleInterrupt);

#endif

#ifdef USE_HAL_DRIVER

#if defined(TIM1)
  TIM_TypeDef *Instance = TIM1;
#else
  TIM_TypeDef *Instance = TIM2;
#endif

  // Instantiate HardwareTimer object. Thanks to 'new' instanciation, HardwareTimer is not destructed when setup() function is finished.
  HardwareTimer *IRQtimer = new HardwareTimer(Instance);
  _ISR_microseconds = 1000000 / (pow(2, MODULATION_BIT_DEPTH) * _cubeSizeZ  * _fps);
  IRQtimer->setMode(1, TIMER_OUTPUT_COMPARE);
  IRQtimer->setOverflow( _ISR_microseconds , MICROSEC_FORMAT);
  IRQtimer->attachInterrupt(USE_HAL_DRIVER_handleInterrupt);
  IRQtimer->refresh();
  IRQtimer->resume();

#endif

#ifdef ESP8266
  _ISR_microseconds = 1000000 / (pow(2, MODULATION_BIT_DEPTH) * _cubeSizeZ  * _fps);
  now = _ISR_microseconds;
  timer0_isr_init();
  timer0_attachInterrupt(handleInterrupt);
  timer0_write(ESP.getCycleCount() + _ISR_microseconds * CPU_MHZ); // 80/160 when running at 80/160mhz

#endif

  interrupts();//let the show begin, this lets the multiplexing start


#ifdef ESP8266 // for WEMOS D1 mini
ESP.wdtDisable();
#endif

}



// Allocate memory needed to map the voxels of the cube
void USCL::allocateVoxelMapping(void)
{
  uint16_t size = _cubeArrSize * MODULATION_BIT_DEPTH;  // Calculate max. needed bytes

  // Allocate memory and initialize it with zeros
  if (!_voxelMappingFrontBuffer_R)
    _voxelMappingFrontBuffer_R = (uint8_t*) calloc(size, sizeof(uint8_t));
  if (!_voxelMappingBackBuffer_R)
    _voxelMappingBackBuffer_R = (uint8_t*) calloc(size, sizeof(uint8_t));
  if (!_voxelMappingFrontBuffer_R || !_voxelMappingBackBuffer_R)
    abort(); // Add error status
  if (_cube_mode == RGB_CUBE)
  {
    if (!_voxelMappingFrontBuffer_G)
      _voxelMappingFrontBuffer_G = (uint8_t*) calloc(size, sizeof(uint8_t));
    if (!_voxelMappingBackBuffer_G)
      _voxelMappingBackBuffer_G = (uint8_t*) calloc(size, sizeof(uint8_t));
    if (!_voxelMappingFrontBuffer_G || !_voxelMappingBackBuffer_R)
      abort(); // Add error status

    if (!_voxelMappingFrontBuffer_B)
      _voxelMappingFrontBuffer_B = (uint8_t*) calloc(size, sizeof(uint8_t));
    if (!_voxelMappingBackBuffer_B)
      _voxelMappingBackBuffer_B = (uint8_t*) calloc(size, sizeof(uint8_t));
    if (!_voxelMappingFrontBuffer_B || !_voxelMappingBackBuffer_R)
      abort(); // Add error status
  }
}
#ifdef AVR
// Prescaler calculation
Prescaler USCL::calcPrescaler(double period)
{
  uint16_t pr = ceil((F_CPU * period) / 65535.0);
  for (uint8_t i = 0; i < PRESCALER_ARR_SIZE; i++)
  {
    if (pr <= _prescalers[i].value)
      return _prescalers[i];
  }
  // At this point return the largest available prescaler
  return _prescalers[PRESCALER_ARR_SIZE - 1];
}
#endif

// Page flip buffering routine
inline void USCL::pageFlipBuffering(void)
{
  if (!_invokeBufferSwap)
    return;
  if (_cube_mode == RGB_CUBE)
  {
    swapBuffer(_voxelMappingFrontBuffer_G, _voxelMappingBackBuffer_G);
    swapBuffer(_voxelMappingFrontBuffer_B, _voxelMappingBackBuffer_B);
  }
  swapBuffer(_voxelMappingFrontBuffer_R, _voxelMappingBackBuffer_R);


  _invokeBufferSwap = false;
}

// Refresh the cube with new data
inline void USCL::refreshData(void)
{
  digitalWrite(_OE_pin, HIGH); // Clear all data   // to keep BAM PWM accurate LEDs are not shown during transfer

#ifdef ESP8266 // for WEMOS D1 mini
  timer0_write(ESP.getCycleCount() + now * CPU_MHZ); // 80/160 when running at 80/160mhz
  //yield();
  ESP.wdtFeed(); // feed the dog  // must be called so watchdog won't reset board // around 0.5uS overhead
#endif

  //  delayMicroseconds(1); // in case of OE signal doesn't travel to last 595 before transfer //
  uint16_t offset = _zPositionCounter * _layerArrSize + _modulationOffset * _cubeArrSize;
  if (_cube_mode == RGB_CUBE)
  {
    for (uint8_t i = _layerArrSize; i-- > 0;) {
      //  per core: //  SPI.transfer(_voxelMappingFrontBuffer_B[ i + offset]);  // send blue first
      SPI_TRANSFER(_voxelMappingFrontBuffer_B[ i + offset]);
    }
    for (uint8_t i = _layerArrSize; i-- > 0;) {
      //  per core: //     SPI.transfer(_voxelMappingFrontBuffer_G[ i + offset]);  // send green
      SPI_TRANSFER(_voxelMappingFrontBuffer_G[ i + offset]);
    }
  }
  for (uint8_t i = _layerArrSize; i-- > 0;) {
    //  per core: //     SPI.transfer(_voxelMappingFrontBuffer_R[ i + offset]);  // send red last
    SPI_TRANSFER(_voxelMappingFrontBuffer_R[ i + offset]);
  }


  for (int j = (_cubeSizeZ - 1) / 8; j >= 0 ; j-- )
  {
    if ( j == ((_zPositionCounter) / 8) )
    {
      // % - moduo - remaining of result from integer division
      //  per core: //     SPI.transfer(byte((1 << (_zPositionCounter % 8)))); // send curent layer byte //
      SPI.transfer(byte((1 << (_zPositionCounter % 8)))); // do the unoptimized spi transfer, optimized version overshoot (hardware still sending data without software)
    }
    else
    {
      //  per core: //     SPI.transfer(byte((0 << (_zPositionCounter % 8)))); // not in current byte, so send 0
      SPI.transfer(byte((0 << (_zPositionCounter % 8)))); // do the unoptimized spi transfer, optimized version overshoot (hardware still sending data without software)
    }
  }



  _modulationCounter++;
  if (_modulationCounter == _modulationBitValue)
  {
    _modulationCounter = 0;
    _modulationOffset++;
    if (_modulationOffset == MODULATION_BIT_DEPTH)
    {
      _modulationOffset = 0;
      _zPositionCounter++;
      if (_zPositionCounter >= _cubeSizeZ)
      {
        _zPositionCounter = 0;
        pageFlipBuffering();
      }
    }
    _modulationBitValue = myPow(2, _modulationOffset);
  }

  //delayMicroseconds(1); //  clock before latch tweaks
  // Latch the data
  digitalWrite(_LE_pin, HIGH);
  //    delayMicroseconds(1); //  tweaks
  // This part is tricky. Latch must last long enough that signal at last 595 stay ON long enough for data to be latched. It is not much problem for small cubes, as it is for bigger ones
  // and that's why digitalWrite is here. It is not just poor programming skills, it NEED to be slow enough. And yes, i have poor programming skills :-)
  // don't ask me to write faster code for arduino. It have 4 digitalWrite() commands, so in total 12uS wasted. So it is around less then 50uS long. Not worth the effort.


  //delayMicroseconds(2);        // latch tweak
  digitalWrite(_LE_pin, LOW);  // ok, now is the time to release latch
  //delayMicroseconds(1);        // tweak
  digitalWrite(_OE_pin, LOW); //  almout at the end IRQ, time to enable OE
}


// Set voxel (z,x,y) the (hue (0-360) , saturation (0-1), value (0-1) state
void USCL::HSV(int8_t z, int8_t x, int8_t y, int hue, float sat , float val)

{
  VoxelCoordinate currVoxel;
  // take care of boundaries

  if (z >= 128)
    return; //z = 0;
  if (z >= _cubeSizeZ)
    return; //currVoxel.z = _cubeSize - 1;
  if (z < 0 )
    return; //;
  currVoxel.z = z;

  if (y >= 128)
    return; //y = 0;
  if (y < 0)
    return;
  if (y >= _cubeSizeY)
    return; //currVoxel.y = _cubeSize - 1;

  currVoxel.y = y;

  if (x < 0)
    return;
  if (x >= 128)
    return; //x = 0;
  if (x >= _cubeSizeX)
    return; //currVoxel.z = _cubeSize - 1;

  currVoxel.x = x;



  // HSV2RGB
  ////////////////////////////////////
  // Make sure our arguments stay in-range
  if (hue < 0) hue = 0;
  if (hue > 359) hue = 359;
  //hue = max(0, min(360, hue));
  if (sat < 0.0) sat = 0.0;
  if (sat > 1.0) sat = 1.0;
  //sat = max(0, min(1.0, sat));
  if (val < 0) val = 0;
  if (val > 1.0) val = 1.0;
  //val = max(0, min(1.0, val));


  double hs = hue / 60.0; // sector 0 to 5
  int i = floor(hs);
  double f = hs - i; // factorial part of h
  double p = val * (1 - sat);
  double q = val * (1 - sat * f);
  double t = val * (1 - sat * (1 - f));
  double r, g, b;
  switch (i)
  {
    case 0:
      r = val;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = val;
      b = p;
      break;
    case 2:
      r = p;
      g = val;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = val;
      break;
    case 4:
      r = t;
      g = p;
      b = val;
      break;
    default: // case 5:
      r = val;
      g = p;
      b = q;
  }
  r = round(r * _maxBrightness);
  g = round(g * _maxBrightness);
  b = round(b * _maxBrightness);


  if (sat == 0)
  {

    r = round(val * _maxBrightness);
    g = round(val * _maxBrightness);
    b = round(val * _maxBrightness);

  }
  int red, green, blue;
  red = r;
  green = g;
  blue = b;
  /////////////////////////////////////

  if (_cube_mode == LED_CUBE)
  {
    // single color led cube - lets decolorize

    int temp = (red + green + blue) / 3;
    red = temp;
  }

  setVoxel(_voxelMappingBackBuffer_R, currVoxel, red); // Set actual voxel value
  if (_cube_mode == RGB_CUBE)
  {
    setVoxel(_voxelMappingBackBuffer_G, currVoxel, green); // Set actual voxel value
    setVoxel(_voxelMappingBackBuffer_B, currVoxel, blue); // Set actual voxel value
  }
}

// Set voxel (z,y,x) the (red,green,blue) state
void USCL::RGB(int8_t z, int8_t x, int8_t y, int red, int green , int blue)

{
  VoxelCoordinate currVoxel;

  // take care of boundaries

  if (z >= 128)
    return; //z = 0;
  if (z >= _cubeSizeZ)
    return; //currVoxel.z = _cubeSize - 1;
  if (z < 0 )
    return; //;
  currVoxel.z = z;

  if (y >= 128)
    return; //y = 0;
  if (y < 0)
    return;
  if (y >= _cubeSizeY)
    return; //currVoxel.y = _cubeSize - 1;

  currVoxel.y = y;

  if (x < 0)
    return;
  if (x >= 128)
    return; //x = 0;
  if (x >= _cubeSizeX)
    return; //currVoxel.z = _cubeSize - 1;

  currVoxel.x = x;
  if (_cube_mode == LED_CUBE)
  {
    // single color led cube - lets decolorize

    int temp = (red + green + blue) / 3;
    red = temp;
  }

  setVoxel(_voxelMappingBackBuffer_R, currVoxel, red); // Set actual voxel value
  if (_cube_mode == RGB_CUBE)
  {
    setVoxel(_voxelMappingBackBuffer_G, currVoxel, green); // Set actual voxel value
    setVoxel(_voxelMappingBackBuffer_B, currVoxel, blue); // Set actual voxel value
  }
}


void USCL::LED(int8_t z, int8_t x, int8_t y, int brightness)

{
  VoxelCoordinate currVoxel;

  // take care of boundaries

  if (z >= 128)
    return; //z = 0;
  if (z >= _cubeSizeZ)
    return; //currVoxel.z = _cubeSize - 1;
  if (z < 0 )
    return; //;
  currVoxel.z = z;

  if (y >= 128)
    return; //y = 0;
  if (y < 0)
    return;
  if (y >= _cubeSizeY)
    return; //currVoxel.y = _cubeSize - 1;

  currVoxel.y = y;

  if (x < 0)
    return;
  if (x >= 128)
    return; //x = 0;
  if (x >= _cubeSizeX)
    return; //currVoxel.z = _cubeSize - 1;

  currVoxel.x = x;

  setVoxel(_voxelMappingBackBuffer_R, currVoxel, brightness); // Set actual voxel value
  if (_cube_mode == RGB_CUBE)
  {
    setVoxel(_voxelMappingBackBuffer_G, currVoxel, brightness); // Set actual voxel value
    setVoxel(_voxelMappingBackBuffer_B, currVoxel, brightness); // Set actual voxel value
  }
}

void USCL::RED(int8_t z, int8_t x, int8_t y, int brightness)
{
  VoxelCoordinate currVoxel;

  // take care of boundaries

  if (z >= 128)
    return; //z = 0;
  if (z >= _cubeSizeZ)
    return; //currVoxel.z = _cubeSize - 1;
  if (z < 0 )
    return; //;
  currVoxel.z = z;

  if (y >= 128)
    return; //y = 0;
  if (y < 0)
    return;
  if (y >= _cubeSizeY)
    return; //currVoxel.y = _cubeSize - 1;

  currVoxel.y = y;

  if (x < 0)
    return;
  if (x >= 128)
    return; //x = 0;
  if (x >= _cubeSizeX)
    return; //currVoxel.z = _cubeSize - 1;

  currVoxel.x = x;

  setVoxel(_voxelMappingBackBuffer_R, currVoxel, brightness); // Set actual voxel value

}


void USCL::GREEN(int8_t z, int8_t x, int8_t y, int brightness)
{
  VoxelCoordinate currVoxel;

  // take care of boundaries

  if (z >= 128)
    return; //z = 0;
  if (z >= _cubeSizeZ)
    return; //currVoxel.z = _cubeSize - 1;
  if (z < 0 )
    return; //;
  currVoxel.z = z;

  if (y >= 128)
    return; //y = 0;
  if (y < 0)
    return;
  if (y >= _cubeSizeY)
    return; //currVoxel.y = _cubeSize - 1;

  currVoxel.y = y;

  if (x < 0)
    return;
  if (x >= 128)
    return; //x = 0;
  if (x >= _cubeSizeX)
    return; //currVoxel.z = _cubeSize - 1;

  currVoxel.x = x;

  if (_cube_mode == RGB_CUBE)
  {
    setVoxel(_voxelMappingBackBuffer_G, currVoxel, brightness); // Set actual voxel value
  }
  else {
    setVoxel(_voxelMappingBackBuffer_R, currVoxel, brightness); // Set actual voxel value
  }
}


void USCL::BLUE(int8_t z, int8_t x, int8_t y, int brightness)
{
  VoxelCoordinate currVoxel;

  // take care of boundaries

  if (z >= 128) // in case a negative number is passed
    return; //z = 0;
  if (z >= _cubeSizeZ)
    return; //currVoxel.z = _cubeSize - 1;
  if (z < 0 )
    return; //;
  currVoxel.z = z;

  if (y >= 128)
    return; //y = 0;
  if (y < 0)
    return;
  if (y >= _cubeSizeY)
    return; //currVoxel.y = _cubeSize - 1;

  currVoxel.y = y;

  if (x < 0)
    return;
  if (x >= 128)
    return; //x = 0;
  if (x >= _cubeSizeX)
    return; //currVoxel.z = _cubeSize - 1;

  currVoxel.x = x;

  if (_cube_mode == RGB_CUBE)
  {
    setVoxel(_voxelMappingBackBuffer_B, currVoxel, brightness); // Set actual voxel value
  }
  else {
    setVoxel(_voxelMappingBackBuffer_R, currVoxel, brightness); // Set actual voxel value
  }

}


inline void USCL::setVoxel(uint8_t* buffer, VoxelCoordinate voxelCoordinate, uint8_t brightness)
{
  // Return if memory isn't allocated
  if (!buffer)
    return;

  if (brightness < 0 )
    brightness = 0;
  if (brightness > _maxBrightness )
    brightness = _maxBrightness;

  uint16_t voxelPos = voxelCoordinate.y + voxelCoordinate.x * _cubeSizeX; // Calculate the position of the voxel from a single layer
  uint8_t arrPos = voxelPos / 8 + voxelCoordinate.z * _layerArrSize;  // Calculate index of the voxelmapping array based on the position of the voxel
  uint8_t arrShift = voxelPos % 8; // Calculate the needed shift to set the correct bit

  for (uint8_t i = 0; i < MODULATION_BIT_DEPTH; i++)
  {
    if (!!(brightness & (1 << i)))
      buffer[arrPos + i * _cubeArrSize] |= (1 << arrShift); // Set bit to '1'
    else
      buffer[arrPos + i * _cubeArrSize] &= ~(1 << arrShift); // Set bit to '0'
  }
}

// Draw voxels to the cube
void USCL::drawVoxels(bool vsync  /* = true */ )
{
  // Make sure the buffer gets swapped when a multiplex cycle is finished
  if (vsync)
  {
    _invokeBufferSwap = true; // Invoke the buffer swap, this value is toggled to false once it is swapped inside the ISR
    while (_invokeBufferSwap); // Block until buffers are swapped
  }
  else
  {
    if (_cube_mode == RGB_CUBE)
    {
      swapBuffer(_voxelMappingFrontBuffer_G, _voxelMappingBackBuffer_G);
      swapBuffer(_voxelMappingFrontBuffer_B, _voxelMappingBackBuffer_B);
    }
    swapBuffer(_voxelMappingFrontBuffer_R, _voxelMappingBackBuffer_R);
  }

  // Clear back buffer once it is swapped
  if (_autoClearBackBuffer)
  {
    memset(_voxelMappingBackBuffer_R, 0, (_cubeArrSize * MODULATION_BIT_DEPTH) * sizeof(*_voxelMappingBackBuffer_R));
    if (_cube_mode == RGB_CUBE)
    {
      memset(_voxelMappingBackBuffer_G, 0, (_cubeArrSize * MODULATION_BIT_DEPTH) * sizeof(*_voxelMappingBackBuffer_G));
      memset(_voxelMappingBackBuffer_B, 0, (_cubeArrSize * MODULATION_BIT_DEPTH) * sizeof(*_voxelMappingBackBuffer_B));
    }
  }
}

void USCL::copyCurrentFrame(void)
{
  // Return if memory isn't allocated
  if (!_voxelMappingFrontBuffer_R || !_voxelMappingBackBuffer_R) {
    return;
    if (_cube_mode == RGB_CUBE)
    {
      if (!_voxelMappingFrontBuffer_G || !_voxelMappingBackBuffer_G)
        return;

      if (!_voxelMappingFrontBuffer_B || !_voxelMappingBackBuffer_B)
        return;
    }
  }

  memcpy(_voxelMappingBackBuffer_R, _voxelMappingFrontBuffer_R, (_cubeArrSize * MODULATION_BIT_DEPTH) * sizeof(*_voxelMappingBackBuffer_R));
  if (_cube_mode == RGB_CUBE)
  {
    memcpy(_voxelMappingBackBuffer_G, _voxelMappingFrontBuffer_G, (_cubeArrSize * MODULATION_BIT_DEPTH) * sizeof(*_voxelMappingBackBuffer_G));
    memcpy(_voxelMappingBackBuffer_B, _voxelMappingFrontBuffer_B, (_cubeArrSize * MODULATION_BIT_DEPTH) * sizeof(*_voxelMappingBackBuffer_B));
  }
}

// Swap buffers
inline void USCL::swapBuffer(uint8_t*& frontBuffer, uint8_t*& backBuffer)
{
  if (!frontBuffer || !backBuffer)
    return;
  uint8_t* tmpBuffer = frontBuffer;
  frontBuffer = backBuffer;
  backBuffer = tmpBuffer;
}





// Custom pow function
inline uint16_t USCL::myPow(uint8_t x, uint8_t n)
{
  uint16_t result = x;
  if (!n)
    result = 1;
  else
  {
    for (uint8_t i = (n - 1); i-- > 0;)
      result *= x;
  }
  return result;
}

// Draw HSV line in back buffer
void USCL::HSV_line(int8_t z1, int8_t x1, int8_t y1, int8_t z2, int8_t x2, int8_t y2, int hue, float S , float V)
{

  // Voxel coordinates
  int8_t z;
  int8_t x;
  int8_t y;

  // For loop iterators
  uint8_t i;

  int8_t zDirectionVector = z2 - z1;
  int8_t xDirectionVector = x2 - x1;
  int8_t yDirectionVector = y2 - y1;

  for (i = _cubeSizeZ; i-- > 0;)
  {
    z = round(float(i / (float)_maxVoxelValueZ) * zDirectionVector + z1);
    x = round(float(i / (float)_maxVoxelValueX) * xDirectionVector + x1);
    y = round(float(i / (float)_maxVoxelValueY) * yDirectionVector + y1);
    HSV(z, x, y, hue, S, V);

  }

}
// Draw RGB line in back buffer
void USCL::RGB_line(int8_t z1, int8_t x1, int8_t y1, int8_t z2, int8_t x2, int8_t y2, int red, int green , int blue)
{

  // Voxel coordinates
  int8_t z;
  int8_t x;
  int8_t y;

  // For loop iterators
  uint8_t i;

  int8_t zDirectionVector = z2 - z1;
  int8_t xDirectionVector = x2 - x1;
  int8_t yDirectionVector = y2 - y1;

  for (i = _cubeSizeZ; i-- > 0;)
  {
    z = round(float(i / (float)_maxVoxelValueZ) * zDirectionVector + z1);
    x = round(float(i / (float)_maxVoxelValueX) * xDirectionVector + x1);
    y = round(float(i / (float)_maxVoxelValueY) * yDirectionVector + y1);
    RGB(z, x, y, red, green, blue);

  }

}
// Draw LED (white) line in back buffer
void USCL::LED_line(int8_t z1, int8_t x1, int8_t y1, int8_t z2, int8_t x2, int8_t y2,  int brightness)
{

  // Voxel coordinates
  int8_t z;
  int8_t x;
  int8_t y;

  // For loop iterators
  uint8_t i;

  int8_t zDirectionVector = z2 - z1;
  int8_t xDirectionVector = x2 - x1;
  int8_t yDirectionVector = y2 - y1;

  for (i = _cubeSizeZ; i-- > 0;)
  {
    z = round(float(i / (float)_maxVoxelValueZ) * zDirectionVector + z1);
    x = round(float(i / (float)_maxVoxelValueX) * xDirectionVector + x1);
    y = round(float(i / (float)_maxVoxelValueY) * yDirectionVector + y1);
    LED(z, x, y,   brightness);

  }

}
// Draw RED line in back buffer
void USCL::RED_line(int8_t z1, int8_t x1, int8_t y1, int8_t z2, int8_t x2, int8_t y2,  int brightness)
{

  // Voxel coordinates
  int8_t z;
  int8_t x;
  int8_t y;

  // For loop iterators
  uint8_t i;

  int8_t zDirectionVector = z2 - z1;
  int8_t xDirectionVector = x2 - x1;
  int8_t yDirectionVector = y2 - y1;

  for (i = _cubeSizeZ; i-- > 0;)
  {
    z = round(float(i / (float)_maxVoxelValueZ) * zDirectionVector + z1);
    x = round(float(i / (float)_maxVoxelValueX) * xDirectionVector + x1);
    y = round(float(i / (float)_maxVoxelValueY) * yDirectionVector + y1);
    RED(z, x, y,   brightness);

  }

}
// Draw GREEN  line in back buffer
void USCL::GREEN_line(int8_t z1, int8_t x1, int8_t y1, int8_t z2, int8_t x2, int8_t y2,  int brightness)
{

  // Voxel coordinates
  int8_t z;
  int8_t x;
  int8_t y;

  // For loop iterators
  uint8_t i;

  int8_t zDirectionVector = z2 - z1;
  int8_t xDirectionVector = x2 - x1;
  int8_t yDirectionVector = y2 - y1;

  for (i = _cubeSizeZ; i-- > 0;)
  {
    z = round(float(i / (float)_maxVoxelValueZ) * zDirectionVector + z1);
    x = round(float(i / (float)_maxVoxelValueX) * xDirectionVector + x1);
    y = round(float(i / (float)_maxVoxelValueY) * yDirectionVector + y1);
    GREEN(z, x, y,   brightness);

  }

}
// Draw BLUE  line in back buffer
void USCL::BLUE_line(int8_t z1, int8_t x1, int8_t y1, int8_t z2, int8_t x2, int8_t y2,  int brightness)
{

  // Voxel coordinates
  int8_t z;
  int8_t x;
  int8_t y;

  // For loop iterators
  uint8_t i;

  int8_t zDirectionVector = z2 - z1;
  int8_t xDirectionVector = x2 - x1;
  int8_t yDirectionVector = y2 - y1;

  for (i = _cubeSizeZ; i-- > 0;)
  {
    z = round(float(i / (float)_maxVoxelValueZ) * zDirectionVector + z1);
    x = round(float(i / (float)_maxVoxelValueX) * xDirectionVector + x1);
    y = round(float(i / (float)_maxVoxelValueY) * yDirectionVector + y1);
    BLUE(z, x, y,   brightness);

  }

}
