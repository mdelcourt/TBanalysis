#include "MyHeader.h"
#include "alignToolbox.h"
#include "runManager.h"


runProcessor * r;
int Npoints = 5000;
double maxDist = 5;


double getFOM(const double *xx){
  alignPar * p = new alignPar(xx);
  double FOM=0;
  for (int evtId =0; evtId<Npoints; evtId++){
    double dist=maxDist+1;
    int ntries=0;
    while (dist > maxDist && ntries < 20){
      ntries++;
      r->getCleanEvent();
      if (r->fhits->size()==0 || r->tracks->size()==0)
        dist = maxDist+1;
      else
        dist = p->distance(r->fhits->at(0),r->tracks->at(0));
    }
    FOM+=dist;
  }
  cout<<"."<<endl;
  delete p;
  return(FOM);

}





void tester(){
  r = new runProcessor("/home/ucl/cp3/delcourt/storage/tbFullTree/1611/AnalysisTree_0926.root");
  cout<<"h created"<<endl;
  alignPar * p = new alignPar();
  p->alpha = -3.98135;
  p->beta  = 19.9168;
  p->U0    = 39.0609;
  p->V0    = 168.11;
  p->D     = 0;

  double * x = new double[100];
  double * y = new double[100];
  for (int i=0; i<100; i++){
    cout<<i<<"/100 done"<<endl;
    x[i] = -2+0.02*2*i;
    double * param = p->doubleVec();
    param[1]+=x[i];
    y[i] = getFOM(param);
    delete param;
  }
  TGraph * g = new TGraph(100,x,y);
  g->Draw("ALP*");    
}


int main(){
  r = new runProcessor("/home/ucl/cp3/delcourt/storage/tbFullTree/1611/AnalysisTree_0926.root");
  ROOT::Math::Minimizer* min =
          ROOT::Math::Factory::CreateMinimizer("Minuit2", "");
   // set tolerance , etc...
   min->SetMaxFunctionCalls(1000000); // for Minuit/Minuit2
   min->SetMaxIterations(10000);  // for GSL
   min->SetTolerance(1);
   min->SetPrintLevel(1);

   ROOT::Math::Functor f(&getFOM,5);
   double step[5] = {0.1,0.1,0.1,0.1,0.1};

   alignPar * p = new alignPar();
   p->alpha = -4.98135;
   p->beta  = 19.9168;
   p->U0    = 39.0609;
   p->V0    = 168.11;
   p->D     = 0;
   
   double * variable = new double [5];
   variable =  p->doubleVec();

   min->SetFunction(f);
   min->SetVariable(0,"alpha",variable[0], step[0]);
   min->SetVariable(1,"beta" ,variable[1], step[1]);
   min->SetVariable(2,"U0"   ,variable[2], step[2]);
   min->SetVariable(3,"V0"   ,variable[3], step[3]);
   min->SetVariable(4,"D"    ,variable[4], step[4]);

   min->Minimize();

   const double *xs = min->X();
   cout << "Minimum: f(" << xs[0] << "," << xs[1] << ","<<xs[2]<<","<<xs[3]<<","<<xs[4]<<")"<< endl;

   return(1);
}
