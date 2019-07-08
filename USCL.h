/*
  LedCube.h - Library for controlling shift register driven LED cubes
  Copyright (C) 2015  Mike Neuhaus.

  This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#ifndef USCL_h
#define USCL_h

#include <inttypes.h>

// Supported cube sizes
// from
// 1x1x1  
// to
// 128x128x128



// Suported SPI clock speed (CPU Frequency / number )
#define SPI_speed_2 SPI_CLOCK_DIV2
#define SPI_speed_4 SPI_CLOCK_DIV4
#define SPI_speed_8 SPI_CLOCK_DIV8
#define SPI_speed_16 SPI_CLOCK_DIV16
#define SPI_speed_32 SPI_CLOCK_DIV32
#define SPI_speed_64 SPI_CLOCK_DIV64
#define SPI_speed_128 SPI_CLOCK_DIV128

#define RGB_CUBE 0x01
#define LED_CUBE 0x00


typedef struct
{
  uint8_t z;
  uint8_t x;
  uint8_t y;
} VoxelCoordinate;

#ifdef AVR
typedef struct
{
  uint16_t value;
  uint8_t selectBits;
} Prescaler;
#endif
class USCL
{
  public:
    USCL(uint8_t cubeSizeX, uint8_t cubeSizeY, uint8_t cubeSizeZ, uint8_t cube_mode , uint16_t LE_pin, uint16_t OE_pin, uint16_t fps, int MODULATION_BIT_DEPTH , uint32_t SPI_speed);
    ~USCL(void);

    void begin(void);
    void HSV(uint8_t z, uint8_t x, uint8_t y, int hue, float sat , float val);
    void RGB(uint8_t z, uint8_t x, uint8_t y, int red, int green , int blue);
    void LED(uint8_t z, uint8_t x, uint8_t y, int brightness);
    void RED(uint8_t z, uint8_t x, uint8_t y, int brightness);
    void GREEN(uint8_t z, uint8_t x, uint8_t y, int brightness);
    void BLUE(uint8_t z, uint8_t x, uint8_t y, int brightness);
    void drawVoxels(bool vsync = true);
    void copyCurrentFrame(void);
    // Draw functions
    void HSV_line(uint8_t z0, uint8_t x0, uint8_t y0, uint8_t z1, uint8_t x1, uint8_t y1, int hue, float sat , float val);
    void RGB_line(uint8_t z0, uint8_t x0, uint8_t y0, uint8_t z1, uint8_t x1, uint8_t y1, int red, int green , int blue);
    void LED_line(uint8_t z0, uint8_t x0, uint8_t y0, uint8_t z1, uint8_t x1, uint8_t y1, int brightness);
    void GREEN_line(uint8_t z0, uint8_t x0, uint8_t y0, uint8_t z1, uint8_t x1, uint8_t y1, int brightness);
    void BLUE_line(uint8_t z0, uint8_t x0, uint8_t y0, uint8_t z1, uint8_t x1, uint8_t y1, int brightness);
    void RED_line(uint8_t z0, uint8_t x0, uint8_t y0, uint8_t z1, uint8_t x1, uint8_t y1, int brightness);

    static inline void handleInterrupt(void);

    // Accessors

    uint8_t getCubeSizeX() const {
      return _cubeSizeX;
    }
     uint8_t getCubeSizeY() const {
      return _cubeSizeY;
    }
    uint8_t getCubeSizeZ() const {
      return _cubeSizeZ;
    }       
    uint8_t getFPS() const {
      return _fps;
    }
    uint8_t getMaxBrightness() const {
      return _maxBrightness;
    }
    void setAutoClearBackBuffer(bool value) {
      _autoClearBackBuffer = value;
    }

  private:

    void initInterrupt(void);
    void allocateVoxelMapping(void);
    inline void pageFlipBuffering(void);
    inline void swapBuffer(uint8_t*& frontBuffer, uint8_t*& backBuffer);
    inline void setVoxel(uint8_t *buffer, VoxelCoordinate voxelCoordinate, uint8_t brightness);
    inline void refreshData(void);
    inline uint16_t myPow(uint8_t x, uint8_t n);

    static USCL *_objPtr;
#ifdef AVR
    static const Prescaler _prescalers[5];
    Prescaler calcPrescaler(double period);
    uint16_t _refreshFrequency;
#endif
    uint32_t _SPI_speed;
    uint16_t _ISR_microseconds;
    uint16_t _fps;
    uint8_t _cubeSizeX;
    uint8_t _cubeSizeY;
    uint8_t _cubeSizeZ;        
    uint8_t _maxVoxelValueX;
    uint8_t _maxVoxelValueY;    
    uint8_t _maxVoxelValueZ;    
    uint8_t *_voxelMappingFrontBuffer_R;
    uint8_t *_voxelMappingBackBuffer_R;
    uint8_t *_voxelMappingFrontBuffer_G;
    uint8_t *_voxelMappingBackBuffer_G;
    uint8_t *_voxelMappingFrontBuffer_B;
    uint8_t *_voxelMappingBackBuffer_B;
    uint8_t _zPositionCounter;
    uint8_t _layerArrSize;
    uint8_t _cubeArrSize;
    uint8_t _modulationCounter;
    uint8_t _modulationBitValue;
    uint8_t _maxBrightness;
    uint8_t _modulationOffset;
    uint8_t _globalBrightnessDecrease;
    
     uint8_t _cubeSize;

    volatile bool _invokeBufferSwap;
    bool _autoClearBackBuffer;


};
#endif
