#include "MyHeader.h"

void testAnalyser(){
  TFile * f = new TFile( "/eos/cms/store/group/upgrade/Tracker/TestBeam/BTOct2016/AnalysisTuple/AnalysisTree_0857.root" );
  TTree * t = (TTree *)f->Get("analysisTree");
  bool * isGood = new bool;
  dutEvent       * DUT  = new dutEvent;
  condEvent      * COND = new condEvent;
  TelescopeEvent * TEL  = new TelescopeEvent;
  FeIFourEvent   * FEI4 = new FeIFourEvent;


  t->SetBranchAddress("DUT"            ,&DUT   );
  t->SetBranchAddress("Condition"      ,&COND  );
  t->SetBranchAddress("TelescopeEvent" ,&TEL   );
  t->SetBranchAddress("Fei4Event"      ,&FEI4  );
  t->SetBranchAddress("goodEventFlag"  ,&isGood);

  for (long unsigned int eventId = 0; eventId<t->GetEntries(); eventId++){
     t->GetEntry(eventId);
  }
}
