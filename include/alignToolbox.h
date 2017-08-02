#ifndef __ALIGNTOOLBOX__
#define __ALIGNTOOLBOX__
#include "MyHeader.h"
#include "DataFormats.h"
#include "runManager.h"


using namespace tbeam;

class alignPar{
private:
  float * xv;
  float * yv;

public:
  float alpha,beta;
  float U0,V0;
  float D;

  alignPar();
  alignPar(const double * xx);
  alignPar(string fName, string objName="alignVec");
  ~alignPar();
  void Print();
  float GetX(int u, float dxdz=0);
  float GetY(int v, float dydz=0);
  float GetU(float x, float dxdz=0);
  float GetV(float y, float dydz=0);
  float Distance (FEI4hit * h, track * t);
  double *  DoubleVec();
  TVector * GetTVector();
  void LoadFile(string fName = "alignSave.root",string objName = "alignVec");
  void SaveFile(string fName = "alignSave.root",string objName = "alignVec");
  
  TGraph * OptimisePar( float & par, runProcessor * r, string parName = "parameter", float maxDiff = 1, int nStep=50, int nEvent = 5000);


};

#endif
