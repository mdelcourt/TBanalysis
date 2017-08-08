#include "alignToolbox.h"
using namespace tbeam;
alignPar::alignPar():xv(NULL),yv(NULL),alpha(0),beta(0),U0(0),V0(0),D(0),Ul(0),Uh(0),Vl(0),Vh(0) { }
alignPar::alignPar(const double * xx):xv(NULL),yv(NULL),alpha(xx[0]),beta(xx[1])
                                ,U0(xx[2]),V0(xx[3]),D(xx[4]),Ul(0),Uh(0),Vl(0),Vh(0)
                                { }
alignPar::alignPar(string fName,string ObjName):xv(NULL),yv(NULL){
  LoadFile(fName,ObjName);
}
double * alignPar::DoubleVec(){
  double * pSet = new double [9];
  pSet[0]=alpha;
  pSet[1]=beta;
  pSet[2]=U0;
  pSet[3]=V0;
  pSet[4]=D;
  pSet[5]=Ul;
  pSet[6]=Uh;
  pSet[7]=Vl;
  pSet[8]=Vh;
  return(pSet);
}

void alignPar::Print(){
  cout<<"**** Alignment parameters : "<<endl;
  cout<<"* Alpha = "<<alpha<<endl;
  cout<<"* Beta  = "<<beta<<endl;
  cout<<"* U0    = "<<U0<<endl;
  cout<<"* V0    = "<<V0<<endl;
  cout<<"* D     = "<<D<<endl;
  cout<<"* Ul    = "<<Ul<<endl;
  cout<<"* Uh    = "<<Uh<<endl;
  cout<<"* Vl    = "<<Vl<<endl;
  cout<<"* Vh    = "<<Vh<<endl;
  cout<<"****************************"<<endl;
}

float alignPar::GetX(int u, float dxdz){
  return((u-U0)*1./alpha +dxdz*D);
}

float alignPar::GetY(int v, float dydz){
  return((v-V0)*1./beta + dydz*D);
}

float alignPar::GetU(float x, float dxdz){
  return(alpha*(x+dxdz*D)+U0);
}

float alignPar::GetV(float y, float dydz){
  return(beta*(y+dydz*D)+V0);
}

float alignPar::Distance(FEI4hit * h , track * t){
  double dx = GetX(h->col,t->dxdz)-t->xPos;
  double dy = GetY(h->row,t->dydz)-t->yPos;
  return(sqrt(dx*dx+dy*dy));
}

float alignPar::Distance(cluster * cl , track * t){
  double dx = GetX(cl->x,t->dxdz)-t->yPos;
  return(abs(dx));
}


void alignPar::LoadFile(string fName,string objName){
  TFile * f = new TFile(fName.c_str());
  TVector * v=(TVector *) f->Get(objName.c_str());
  if (v)
    LoadVector(v);
  f->Close();
  delete f;
};

void alignPar::LoadVector(TVector * v){
  alpha = (*v)[0];
  beta  = (*v)[1];
  U0    = (*v)[2];
  V0    = (*v)[3];
  D     = (*v)[4];
  Ul    = (*v)[5];
  Uh    = (*v)[6];
  Vl    = (*v)[7];
  Vh    = (*v)[8];
}


TVector * alignPar::GetTVector(){
  double * p = DoubleVec();
  TVector * v = new TVector(9);
  for (int i=0; i<9; i++)
    (*v)[i]=p[i];
  delete p;
  return(v);
}

void alignPar::SaveFile(string fName,string objName){
  TFile * f = new TFile(fName.c_str(),"RECREATE");
  TVector * v=GetTVector();
  v->Write(objName.c_str());
  f->Close();
  delete f;
  delete v;
};

alignPar::~alignPar(){
  if (xv!=NULL)
    delete xv;
  if (yv!=NULL)
    delete yv;
}


float GetFOM(runProcessor * r,alignPar * p, int nTries=5, float tolerance=1){
    float  dist=tolerance+1;
    int nt=0;
    while (dist > tolerance && nt < nTries){
      nt++;
      r->GetCleanEvent();
      if (r->fhits->size()==0 || r->tracks->size()==0)
        dist = tolerance+1;
      else
        dist = p->Distance(r->fhits->at(0),r->tracks->at(0));
    }
    return dist;
}


float GetFOM_DUT(runProcessor * r,alignPar * p, int dut, int nTries=5, float tolerance=1){
    float  dist=tolerance+1;
    int nt=0;
    stringstream ss;
    ss<<"det"<<dut;
    string detName = ss.str();
    while (dist > tolerance && nt < nTries){
      nt++;
      r->GetEvent();
      if (r->DUT->clusters[detName.c_str()].size()==0 || r->matchedTracks->size()==0)
        dist = tolerance+1;
      else{
        float minD = tolerance+1;
        for (auto c : r->DUT->clusters[detName]){
          float dd= p->Distance(c,r->tracks->at(0));
          if (dd<minD)
            minD = dd;
        }
        dist=minD;
      }
    }
    return dist;
}


TGraph * alignPar::OptimisePar(float & par, runProcessor * r, string parName, int dut, float maxDiff, int nStep, int nEvent){
  cout<<"DUT = "<<dut<<" Nevents = "<<nEvent<<endl;
  if (xv!=NULL)
    delete xv;
  if (yv!=NULL)
    delete yv;
  xv = new float [nStep];
  yv = new float [nStep];
  float par0 = par;
  float minPar = par;
  float minFOM = 1e9;
  for (int iSt = 0; iSt < nStep; iSt++){
    par = par0 - maxDiff + maxDiff*(iSt*2./nStep);
    xv[iSt] = par;
    yv[iSt] = 0;
    for (int evtId = 0; evtId<nEvent; evtId++){
      if (dut>-1)
        yv[iSt]+=GetFOM_DUT(r,this,dut);
      else
        yv[iSt]+=GetFOM(r,this);
    }
    if (yv[iSt]<minFOM){
      minPar = xv[iSt];
      minFOM = yv[iSt];
    }
  }
  par = minPar;
  TGraph * g = new TGraph(nStep,xv,yv);
  g->SetTitle(parName.c_str());
  g->SetName(parName.c_str());
  return(g);

}

bool alignPar::isGood(track * t){
  double U = GetU(t->xPos,t->dxdz);
  if (U>Uh || U<Ul) 
    return(false);
  double V = GetV(t->yPos,t->dydz);
  if (V>Vh || V<Vl)
    return(false);
  return(true);
}

