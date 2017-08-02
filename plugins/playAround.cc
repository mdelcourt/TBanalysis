

#include "MyHeader.h"
#include "runManager.h"


int main(){
  alignPar * p = new alignPar("Align_0.root");
  p->Print();

  runProcessor * r = new runProcessor("/home/ucl/cp3/delcourt/storage/tbFullTree/1611/AnalysisTree_0926.root");
  r->registerHisto(p->GetTVector());
  TH2 * hist = new TH2I ("hitMap", "hitMap",1000,-5,5,1000,-10,10);
  r->registerHisto(hist);


  while(r->getEvent()){
    for (auto t :(*r->tracks))
      for (auto h:(*r->fhits))
        if (p->distance(h,t)<0.1)
          hist->Fill(t->xPos,t->yPos);
  }


  r->saveToFile("results.root");
}
