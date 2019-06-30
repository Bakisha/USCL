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


#ifdef AVR // arduino 
#include <Arduino.h>
const Prescaler USCL::_prescalers[] = { {1, 0b001}, {8, 0b010}, {64, 0b011}, {256, 0b100}, {1024, 0b101} };  // Largest prescaler must be the last item.
#define PRESCALER_ARR_SIZE sizeof(_prescalers)/sizeof(Prescaler)
#endif

USCL *USCL::_objPtr = 0;

// Macros

#define VOXELMAPPING_ARR_LAYER_SIZE ceil((_cubeSize * _cubeSize) / 8.0)

int _LE_pin ;  // just declare it
int _OE_pin ;
int MODULATION_BIT_DEPTH ;
uint32_t _SPI_speed;
uint8_t _cube_mode;
uint32_t now = 0; // for wemos d1 mini interrupt

// Constructor
USCL::USCL(uint8_t cubeSize, uint8_t cube_mode , uint16_t OE_pin, uint16_t LE_pin, uint16_t fps, int _MODULATION_BIT_DEPTH , uint32_t SPI_speed )
{

  // Set variables
  _cube_mode = cube_mode;
  _SPI_speed = SPI_speed;
  _OE_pin = OE_pin;
  _LE_pin = LE_pin;
  MODULATION_BIT_DEPTH = _MODULATION_BIT_DEPTH;
  _cubeSize = cubeSize;
  _maxVoxelValue = cubeSize - 1;
  _layerArrSize = VOXELMAPPING_ARR_LAYER_SIZE;
  _cubeArrSize = _layerArrSize * cubeSize;
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

  if (fps > 0)
    _fps = fps;
  else
    _fps = 60;
  // Set pins
  pinMode(OE_pin, OUTPUT);
  digitalWrite(OE_pin, HIGH);
  pinMode(LE_pin, OUTPUT);
  digitalWrite(LE_pin, LOW);

}

// Destructor
USCL::~USCL(void)
{
  // TODO: end();
}

// Start displaying animations on the cube
void USCL::begin(void)
{

  _objPtr = this; // Reference pointer to current instance
  allocateVoxelMapping();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(_SPI_speed);
#ifdef AVR
  //initInterrupt();
  // Calculate prescaler and timer counter

  _refreshFrequency = (pow(2, MODULATION_BIT_DEPTH) * _cubeSize  * _fps);
  double period = 1.0 / _refreshFrequency;
  Prescaler prescaler = calcPrescaler(period);
  uint16_t timerCounter = period / ((double)prescaler.value / F_CPU);
  // Set timer parameters
  // Initialize timer interrupt
  noInterrupts();// kill interrupts until everybody is set up
  //cli(); // Clear global interrupt flag
  TCNT1  = 0;
  TCCR1A = 0;
  TCCR1B = prescaler.selectBits;
  OCR1A = timerCounter;
  TCCR1B |= (1 << WGM12);
  TIMSK1 |= (1 << OCIE1A);
  interrupts();//let the show begin, this lets the multiplexing start
  //  sei(); // Set global interrupt flag
#endif

#ifdef __STM32F1__// bluepill
  // Setup IRQ Timer
  _ISR_microseconds = 1000000 / (pow(2, MODULATION_BIT_DEPTH) * _cubeSize  * _fps);
  noInterrupts();// kill interrupts until everybody is set up
  Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  Timer2.setPeriod(_ISR_microseconds); // in microseconds
  Timer2.setCompare(TIMER_CH1, 1);      // overflow might be small
  Timer2.attachInterrupt(TIMER_CH1, handleInterrupt);
  interrupts();//let the show begin, this lets the multiplexing start
#endif

#ifdef ESP8266
  _ISR_microseconds = 1000000 / (pow(2, MODULATION_BIT_DEPTH) * _cubeSize  * _fps);
  now=_ISR_microseconds;
	noInterrupts();
  timer0_isr_init();
  timer0_attachInterrupt(handleInterrupt);
  timer0_write(ESP.getCycleCount() + _ISR_microseconds * 80); // 160 when running at 160mhz
  interrupts();
  #endif
  


  SPI.begin();
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
#ifdef ESP8266 // for WEMOS D1 mini
timer0_write(ESP.getCycleCount() + now * 80); // 160 when running at 160mhz 
#endif
  
  digitalWrite(_OE_pin, HIGH); // Clear all data   // to keep BAM PWM accurate LEDs are not shown during transfer
  //  delayMicroseconds(1); // in case of OE signal doesn't travel to last 595 before transfer //
 
  uint16_t offset = _zPositionCounter * _layerArrSize + _modulationOffset * _cubeArrSize;
  if (_cube_mode == RGB_CUBE)
  {
    for (uint8_t i = _layerArrSize; i-- > 0;)
      SPI.transfer(_voxelMappingFrontBuffer_B[ i + offset]);  // send blue first
    for (uint8_t i = _layerArrSize; i-- > 0;)
      SPI.transfer(_voxelMappingFrontBuffer_G[ i + offset]);  // send green
  }
  for (uint8_t i = _layerArrSize; i-- > 0;)
    SPI.transfer(_voxelMappingFrontBuffer_R[ i + offset]);  // send red last

  SPI.transfer((1 << _zPositionCounter)); // Send Current layer byte




/*
_zPositionCounter++;
if (_zPositionCounter >= _cubeSize)
{
  _zPositionCounter = 0;
  _modulationCounter++;
  if (_modulationCounter == _modulationBitValue)
  {
    _modulationCounter = 0;
    _modulationOffset++;
    if (_modulationOffset == MODULATION_BIT_DEPTH)
    {
      _modulationOffset = 0;
      pageFlipBuffering();
    }
  }
  _modulationBitValue = myPow(2, _modulationOffset);
}
*/

_modulationCounter++;
	if(_modulationCounter == _modulationBitValue)
	{
		_modulationCounter = 0;
		_modulationOffset++;
		if(_modulationOffset == MODULATION_BIT_DEPTH)
		{
			_modulationOffset = 0;
			_zPositionCounter++;
			if(_zPositionCounter >= _cubeSize)
			{
				_zPositionCounter = 0;
				pageFlipBuffering();
			}
		}
		_modulationBitValue = myPow(2, _modulationOffset);
	}


// Latch the data
   digitalWrite(_LE_pin, HIGH);
  //    delayMicroseconds(1); //  tweaks
  // This part is tricky. Latch must last long enough that signal at last 595 stay ON long enough for data to be latched. It is not much problem for small cubes, as it is for bigger ones
  // and that's why digitalWrite is here. It is not just poor programming skills, it NEED to be slow enough. And yes, i have poor programming skills :-)


delayMicroseconds(2);        // latch tweak
digitalWrite(_LE_pin, LOW);  // ok, now is the time to release latch
delayMicroseconds(2);        // tweak
 digitalWrite(_OE_pin, LOW); //  almout at the end IRQ, time to enable OE
}

// Interrupt handler
inline void USCL::handleInterrupt(void)
{
  if (_objPtr)
    _objPtr->refreshData();
}

#ifdef AVR
// Timer interrupt
ISR(TIMER1_COMPA_vect)
{
  USCL::handleInterrupt();
}
#endif




// Set voxel (z,x,y) the (hue (0-360) , saturation (0-1), value (0-1) state
void USCL::HSV(uint8_t z, uint8_t x, uint8_t y, int hue, float sat , float val)

{
  VoxelCoordinate currVoxel;
  // take care of boundaries

  if (z >= 128)
    return; //z = 0;
  if (z >= _cubeSize)
    return; //currVoxel.z = _cubeSize - 1;
  if (z < 0 )
    return; //;
  currVoxel.z = z;

  if (y >= 128)
    return; //y = 0;
  if (y < 0)
    return;
  if (y >= _cubeSize)
    return; //currVoxel.y = _cubeSize - 1;

  currVoxel.y = y;

  if (x < 0)
    return;
  if (x >= 128)
    return; //x = 0;
  if (x >= _cubeSize)
    return; //currVoxel.z = _cubeSize - 1;

  currVoxel.x = x;



  // HSV2RGB
  ////////////////////////////////////
  // Make sure our arguments stay in-range
  if (hue<0) hue=0;
  if (hue>359) hue = 359;
  //hue = max(0, min(360, hue));
  if (sat<0.0) sat=0.0;
  if (sat>1.0) sat = 1.0;
  //sat = max(0, min(1.0, sat));
    if (val<0) val=0;
  if (val>1.0) hue = 1.0;
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
  byte red, green, blue;
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
void USCL::RGB(uint8_t z, uint8_t x, uint8_t y, int red, int green , int blue)

{
  VoxelCoordinate currVoxel;

  // take care of boundaries

  if (z >= 128)
    return; //z = 0;
  if (z >= _cubeSize)
    return; //currVoxel.z = _cubeSize - 1;
  if (z < 0 )
    return; //;
  currVoxel.z = z;

  if (y >= 128)
    return; //y = 0;
  if (y < 0)
    return;
  if (y >= _cubeSize)
    return; //currVoxel.y = _cubeSize - 1;

  currVoxel.y = y;

  if (x < 0)
    return;
  if (x >= 128)
    return; //x = 0;
  if (x >= _cubeSize)
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


void USCL::LED(uint8_t z, uint8_t x, uint8_t y, int brightness)

{
  VoxelCoordinate currVoxel;

  // take care of boundaries

  if (z >= 128)
    return; //z = 0;
  if (z >= _cubeSize)
    return; //currVoxel.z = _cubeSize - 1;
  if (z < 0 )
    return; //;
  currVoxel.z = z;

  if (y >= 128)
    return; //y = 0;
  if (y < 0)
    return;
  if (y >= _cubeSize)
    return; //currVoxel.y = _cubeSize - 1;

  currVoxel.y = y;

  if (x < 0)
    return;
  if (x >= 128)
    return; //x = 0;
  if (x >= _cubeSize)
    return; //currVoxel.z = _cubeSize - 1;

  currVoxel.x = x;

  setVoxel(_voxelMappingBackBuffer_R, currVoxel, brightness); // Set actual voxel value
  if (_cube_mode == RGB_CUBE)
  {
    setVoxel(_voxelMappingBackBuffer_G, currVoxel, brightness); // Set actual voxel value
    setVoxel(_voxelMappingBackBuffer_B, currVoxel, brightness); // Set actual voxel value
  }
}

void USCL::RED(uint8_t z, uint8_t x, uint8_t y, int brightness)
{
  VoxelCoordinate currVoxel;

  // take care of boundaries

  if (z >= 128)
    return; //z = 0;
  if (z >= _cubeSize)
    return; //currVoxel.z = _cubeSize - 1;
  if (z < 0 )
    return; //;
  currVoxel.z = z;

  if (y >= 128)
    return; //y = 0;
  if (y < 0)
    return;
  if (y >= _cubeSize)
    return; //currVoxel.y = _cubeSize - 1;

  currVoxel.y = y;

  if (x < 0)
    return;
  if (x >= 128)
    return; //x = 0;
  if (x >= _cubeSize)
    return; //currVoxel.z = _cubeSize - 1;

  currVoxel.x = x;

  setVoxel(_voxelMappingBackBuffer_R, currVoxel, brightness); // Set actual voxel value

}


void USCL::GREEN(uint8_t z, uint8_t x, uint8_t y, int brightness)
{
  VoxelCoordinate currVoxel;

  // take care of boundaries

  if (z >= 128)
    return; //z = 0;
  if (z >= _cubeSize)
    return; //currVoxel.z = _cubeSize - 1;
  if (z < 0 )
    return; //;
  currVoxel.z = z;

  if (y >= 128)
    return; //y = 0;
  if (y < 0)
    return;
  if (y >= _cubeSize)
    return; //currVoxel.y = _cubeSize - 1;

  currVoxel.y = y;

  if (x < 0)
    return;
  if (x >= 128)
    return; //x = 0;
  if (x >= _cubeSize)
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


void USCL::BLUE(uint8_t z, uint8_t x, uint8_t y, int brightness)
{
  VoxelCoordinate currVoxel;

  // take care of boundaries

  if (z >= 128)
    return; //z = 0;
  if (z >= _cubeSize)
    return; //currVoxel.z = _cubeSize - 1;
  if (z < 0 )
    return; //;
  currVoxel.z = z;

  if (y >= 128)
    return; //y = 0;
  if (y < 0)
    return;
  if (y >= _cubeSize)
    return; //currVoxel.y = _cubeSize - 1;

  currVoxel.y = y;

  if (x < 0)
    return;
  if (x >= 128)
    return; //x = 0;
  if (x >= _cubeSize)
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

  uint16_t voxelPos = voxelCoordinate.y + voxelCoordinate.x * _cubeSize; // Calculate the position of the voxel from a single layer
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
void USCL::drawVoxels(bool vsync /* = true */)
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
void USCL::HSV_line(uint8_t z1, uint8_t x1, uint8_t y1, uint8_t z2, uint8_t x2, uint8_t y2, int hue, float S , float V)
{

  // Voxel coordinates
  uint8_t z;
  uint8_t x;
  uint8_t y;

  // For loop iterators
  uint8_t i;

  int8_t zDirectionVector = z2 - z1;
  int8_t xDirectionVector = x2 - x1;
  int8_t yDirectionVector = y2 - y1;

  for (i = _cubeSize; i-- > 0;)
  {
    z = round(float(i / (float)_maxVoxelValue) * zDirectionVector + z1);
    x = round(float(i / (float)_maxVoxelValue) * xDirectionVector + x1);
    y = round(float(i / (float)_maxVoxelValue) * yDirectionVector + y1);
    HSV(z, x, y, hue, S, V);

  }

}
// Draw RGB line in back buffer
void USCL::RGB_line(uint8_t z1, uint8_t x1, uint8_t y1, uint8_t z2, uint8_t x2, uint8_t y2, int red, int green , int blue)
{

  // Voxel coordinates
  uint8_t z;
  uint8_t x;
  uint8_t y;

  // For loop iterators
  uint8_t i;

  int8_t zDirectionVector = z2 - z1;
  int8_t xDirectionVector = x2 - x1;
  int8_t yDirectionVector = y2 - y1;

  for (i = _cubeSize; i-- > 0;)
  {
    z = round(float(i / (float)_maxVoxelValue) * zDirectionVector + z1);
    x = round(float(i / (float)_maxVoxelValue) * xDirectionVector + x1);
    y = round(float(i / (float)_maxVoxelValue) * yDirectionVector + y1);
    RGB(z, x, y, red, green, blue);

  }

}
// Draw LED (white) line in back buffer
void USCL::LED_line(uint8_t z1, uint8_t x1, uint8_t y1, uint8_t z2, uint8_t x2, uint8_t y2,  int brightness)
{

  // Voxel coordinates
  uint8_t z;
  uint8_t x;
  uint8_t y;

  // For loop iterators
  uint8_t i;

  int8_t zDirectionVector = z2 - z1;
  int8_t xDirectionVector = x2 - x1;
  int8_t yDirectionVector = y2 - y1;

  for (i = _cubeSize; i-- > 0;)
  {
    z = round(float(i / (float)_maxVoxelValue) * zDirectionVector + z1);
    x = round(float(i / (float)_maxVoxelValue) * xDirectionVector + x1);
    y = round(float(i / (float)_maxVoxelValue) * yDirectionVector + y1);
    LED(z, x, y,   brightness);

  }

}
// Draw RED line in back buffer
void USCL::RED_line(uint8_t z1, uint8_t x1, uint8_t y1, uint8_t z2, uint8_t x2, uint8_t y2,  int brightness)
{

  // Voxel coordinates
  uint8_t z;
  uint8_t x;
  uint8_t y;

  // For loop iterators
  uint8_t i;

  int8_t zDirectionVector = z2 - z1;
  int8_t xDirectionVector = x2 - x1;
  int8_t yDirectionVector = y2 - y1;

  for (i = _cubeSize; i-- > 0;)
  {
    z = round(float(i / (float)_maxVoxelValue) * zDirectionVector + z1);
    x = round(float(i / (float)_maxVoxelValue) * xDirectionVector + x1);
    y = round(float(i / (float)_maxVoxelValue) * yDirectionVector + y1);
    RED(z, x, y,   brightness);

  }

}
// Draw GREEN  line in back buffer
void USCL::GREEN_line(uint8_t z1, uint8_t x1, uint8_t y1, uint8_t z2, uint8_t x2, uint8_t y2,  int brightness)
{

  // Voxel coordinates
  uint8_t z;
  uint8_t x;
  uint8_t y;

  // For loop iterators
  uint8_t i;

  int8_t zDirectionVector = z2 - z1;
  int8_t xDirectionVector = x2 - x1;
  int8_t yDirectionVector = y2 - y1;

  for (i = _cubeSize; i-- > 0;)
  {
    z = round(float(i / (float)_maxVoxelValue) * zDirectionVector + z1);
    x = round(float(i / (float)_maxVoxelValue) * xDirectionVector + x1);
    y = round(float(i / (float)_maxVoxelValue) * yDirectionVector + y1);
    GREEN(z, x, y,   brightness);

  }

}
// Draw BLUE  line in back buffer
void USCL::BLUE_line(uint8_t z1, uint8_t x1, uint8_t y1, uint8_t z2, uint8_t x2, uint8_t y2,  int brightness)
{

  // Voxel coordinates
  uint8_t z;
  uint8_t x;
  uint8_t y;

  // For loop iterators
  uint8_t i;

  int8_t zDirectionVector = z2 - z1;
  int8_t xDirectionVector = x2 - x1;
  int8_t yDirectionVector = y2 - y1;

  for (i = _cubeSize; i-- > 0;)
  {
    z = round(float(i / (float)_maxVoxelValue) * zDirectionVector + z1);
    x = round(float(i / (float)_maxVoxelValue) * xDirectionVector + x1);
    y = round(float(i / (float)_maxVoxelValue) * yDirectionVector + y1);
    BLUE(z, x, y,   brightness);

  }

}