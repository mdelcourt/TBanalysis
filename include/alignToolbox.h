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
  // Module to align unit
  float alpha,beta;

  // Module to align offset
  float U0,V0;
  // Distance btw track and module to align
  float D;
  // Limits on U coordinates
  float Ul,Uh;
  //Limits on V coordinates
  float Vl,Vh;
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
  float Distance (cluster * c, track * t);
  double *  DoubleVec();
  TVector * GetTVector();
  void LoadFile(string fName = "alignSave.root",string objName = "alignVec");
  void LoadVector(TVector * v);
  void SaveFile(string fName = "alignSave.root",string objName = "alignVec");
  bool isGood(track * t);


  TGraph * OptimisePar( float & par, runProcessor * r, string parName = "parameter", int dut = -1, float maxDiff = 1, int nStep=50, int nEvent = 5000);


};

#endif
