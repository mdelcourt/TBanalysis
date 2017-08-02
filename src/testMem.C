#include "alignToolbox.C"

void testMem(){

  TH1 * h = new TH1I("hist","hist",10,0,10);
  TH1 * h2 = new TH1I("hist2","hist2",256,0,256);
    runProcessor * r = new runProcessor("/home/ucl/cp3/delcourt/storage/tbFullTree/1611/AnalysisTree_0926.root");
    for (int i=0; i<3e6; i++){
      r->getEvent();
      h->Fill(r->tracks->size());

//      for (auto c : r->DUT->clusters["det0"])
//        h2->Fill(c->x);
    }
    cout<<"End of event processor."<<endl;
 h->Draw(); 
 new TCanvas;
 h2->Draw();
}
