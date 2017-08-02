#ifndef __HL_OBJECTS_H__
#define __HL_OBJECTS_H__

#include "MyHeader.h"

class FEI4hit{
public:
  int col,row;
  FEI4hit(int c_=0, int r_=0);
};

class track{
public:
  float xPos;
  float yPos;
  float dxdz;
  float dydz;
  track(float x_ = 0, float y_ = 0, float dx_ = 0, float dy_ = 0);
};


#endif


