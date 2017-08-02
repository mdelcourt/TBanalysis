#include "HLobjects.h"

FEI4hit::FEI4hit(int c_, int r_){
    col=c_;
      row=r_;
}

track::track(float x_, float y_, float dx_, float dy_):
    xPos(x_),yPos(y_),dxdz(dx_),dydz(dy_){}

