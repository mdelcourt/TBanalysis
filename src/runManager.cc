#include "runManager.h"
using namespace tbeam;

runProcessor::runProcessor(string fName, string treeName):eventId(0){
  f=new TFile(fName.c_str());
  t=(TTree *) f->Get(treeName.c_str());
  if (!t)
    cerr<<"Unable to find tree!"<<endl;
  nEntries = t->GetEntries();
  cout<<"Loaded tree "<<treeName<<" with "<<nEntries<<" entries."<<endl;
  COND = new condEvent; 
  tracks = new vector <track *>;
  fhits  = new vector <FEI4hit *>;
  isGood = new bool;
  
  
  TBranch * b = t->GetBranch("DUT");
  b->SetAutoDelete(1);
  b->SetStatus(0);

  b = t->GetBranch("Condition");
  b->SetAutoDelete(1);
  b->SetStatus(0);

  b = t->GetBranch("Fei4Event");
  b->SetAutoDelete(1);
  b->SetStatus(1);

  b = t->GetBranch("TelescopeEvent");
  b->SetAutoDelete(1);
  b->SetStatus(1);


  TEL  = 0;
  FEI4 = 0;
  DUT  = new dutEvent;

  t->SetBranchAddress("DUT"            ,&DUT   );
  t->SetBranchAddress("Condition"      ,&COND  );
  t->SetBranchAddress("TelescopeEvent" ,&TEL   );
  t->SetBranchAddress("Fei4Event"      ,&FEI4  );
  t->SetBranchAddress("goodEventFlag"  ,&isGood); 
}

runProcessor::~runProcessor(){
  f->Close();
  EmptyVectors();

  for (auto h :histos)
    delete h;
  delete DUT;
  delete COND;
  delete TEL;
  delete FEI4;
  delete isGood;
}

void runProcessor::RegisterHisto(TObject *h){
  histos.push_back(h);
}

void runProcessor::SaveToFile(string fName){
  TFile * f = new TFile(fName.c_str(),"RECREATE");
  f->cd();
  for (auto h:histos){
      h->Write();
  }
  f->Close();
}

void runProcessor::EmptyVectors(){
  for (unsigned int i =0; i<tracks->size(); i++)
    delete tracks->at(i);
  for (unsigned int i =0; i<fhits->size(); i++)
    delete fhits->at(i);

  vector<track *>().swap(*tracks);
  vector<FEI4hit *>().swap(*fhits);
}

void runProcessor::ProduceVectors(){
  EmptyVectors();
  for (unsigned int fId = 0; fId < FEI4->col->size(); fId++)
    fhits->push_back(new FEI4hit(FEI4->col->at(fId),FEI4->row->at(fId)));
  for (unsigned int tId = 0; tId < TEL->xPos->size(); tId++)
    tracks->push_back(new track(TEL->xPos->at(tId), TEL->yPos->at(tId), TEL->dxdz->at(tId), TEL->dydz->at(tId)));
}

bool runProcessor::GetEvent(){
  t->GetEvent(eventId);
  eventId++;
  ProduceVectors();
  if (eventId>nEntries){
    eventId=0;
    cout<<"Reached last event... Re-looping on the tree..."<<endl;
    return(false);
  }
  return(true);
}

bool runProcessor::GetCleanEvent(){

  bool status = GetEvent();
  int ntries = 0;
  while (ntries < 20 && (fhits->size()!=1 || tracks->size()!=1)){
    ntries++;
    status*=GetEvent();
  }
  return(status);
}

