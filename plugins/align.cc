

#include "MyHeader.h"
#include "runManager.h"


float fitMin(TGraph * g, float min = 0, float step = 0.04){
  TF1 * myFunc = new TF1("myFunc","pol2");
  float bound_minus = min - 6*step;
  float bound_plus  = min + 6*step;
  g->Fit(myFunc, "", "", bound_minus, bound_plus);

  cout<<" Min     = "<<min<<endl;
  float fMin = -myFunc->GetParameter(1)/(2.*myFunc->GetParameter(2));
  cout<<" Fit min = "<<fMin<<endl;
  return(fMin);
}

void processPar(runProcessor * r, alignPar * p, float & val, string name){
  TGraph * g = p->optimisePar(val , r, name);
  r->registerHisto(g);
  val = fitMin(g,val);

  p->Print();
}



void iteration(runProcessor * r, alignPar * p, alignPar * dp){
  dp->alpha = processPar(r,p,p->alpha,"alpha", dp->alpha);
  dp->beta  = processPar(r,p,p->beta ,"beta" , dp->beta );
  dp->U0    = processPar(r,p,p->U0   ,"U0"   , dp->U0   );
  dp->V0    = processPar(r,p,p->V0   ,"V0"   , dp->V0   );
  dp->D     = processPar(r,p,p->D    ,"D"    , dp->D    );



}


int main(){
  alignPar * p = new alignPar("Align_Work.root");
  p->Print();

  runProcessor * r = new runProcessor("/home/ucl/cp3/delcourt/storage/tbFullTree/1611/AnalysisTree_0926.root");
  processPar(r,p,p->alpha,"alpha");
  processPar(r,p,p->alpha,"alpha");
  processPar(r,p,p->alpha,"alpha");
  processPar(r,p,p->alpha,"alpha");
  processPar(r,p,p->beta,"beta");
  processPar(r,p,p->U0,"U0");
  processPar(r,p,p->V0,"V0");
  processPar(r,p,p->D,"D");

  r->saveToFile("results.root");
  p->SaveFile("Align_Work.root");
}
