#include "runManager.h"
using namespace tbeam;



float tolerance = 5; 

runProcessor::runProcessor(string fName, string treeName):eventId(0){
  f=new TFile(fName.c_str());
  t=(TTree *) f->Get(treeName.c_str());
  if (!t)
    cerr<<"Unable to find tree!"<<endl;
  nEntries = t->GetEntries();
  cout<<"Loaded tree "<<treeName<<" with "<<nEntries<<" entries."<<endl;
  COND = new condEvent; 
  isGood = new bool;
  
  dut_align  = new alignPar;
  fei4_align = new alignPar;

  tracks        = new vector <track *>;
  matchedTracks = new vector <track *>;
  fhits         = new vector <FEI4hit *>;
  eClusters     = new vector <cluster *>;
  cleanClusters = new vector <cluster * >;

  DUT  = 0;//new dutEvent;
  COND = 0;//new condEvent;
  TEL  = 0;//new TelescopeEvent;
  FEI4 = 0;//new FeIFourEvent;
  
  TBranch * b = t->GetBranch("DUT");
  b->SetAddress(&DUT);
  b->SetAutoDelete(1);

  b = t->GetBranch("Condition");
  b->SetAddress(&COND);
//  b->SetStatus(1);
  b->SetAutoDelete(1);

  b = t->GetBranch("Fei4Event");
  b->SetAddress(&FEI4);
  b->SetAutoDelete(1);

  b = t->GetBranch("TelescopeEvent");
  b->SetAddress(&TEL);
  b->SetAutoDelete(1);


  b = t->GetBranch("goodEventFlag");
  b->SetAddress(&isGood);
  b->SetAutoDelete(1);

//  t->SetBranchAddress("DUT"            ,&DUT   );
//  t->SetBranchAddress("Condition"      ,&COND  );
//  t->SetBranchAddress("TelescopeEvent" ,&TEL   );
//  t->SetBranchAddress("Fei4Event"      ,&FEI4  );
//  t->SetBranchAddress("goodEventFlag"  ,&isGood); 
}

runProcessor::~runProcessor(){
  EmptyVectors();

  for (auto h :histos)
    delete h;
  delete DUT;
  delete COND;
  delete TEL;
  delete FEI4;
  f->Close();
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

  TVector * v = dut_align->GetTVector();
  v->Write("dutAlign");
  delete v;
  v = fei4_align->GetTVector();
  v->Write("fei4Align");
  delete v;
  f->Close();
}

void runProcessor::EmptyVectors(){
  for (unsigned int i =0; i<tracks->size(); i++){
    delete tracks->at(i);
    tracks->at(i)=0;
  }
  for (unsigned int i =0; i<fhits->size(); i++){
    delete fhits->at(i);
    fhits->at(i)=0;
  }
  for (auto cl : *eClusters){
    delete cl;
    cl=0;
  }
  
  tracks->clear();
  fhits->clear();
  matchedTracks->clear();
  eClusters ->clear();
  cleanClusters->clear();
}

void runProcessor::ProduceVectors(){
  EmptyVectors();
  for (unsigned int fId = 0; fId < FEI4->col->size(); fId++)
    fhits->push_back(new FEI4hit(FEI4->col->at(fId),FEI4->row->at(fId)));
  for (unsigned int tId = 0; tId < TEL->xPos->size(); tId++)
    tracks->push_back(new track(TEL->xPos->at(tId), TEL->yPos->at(tId), TEL->dxdz->at(tId), TEL->dydz->at(tId)));
  for (auto t : *tracks)
    for (auto f : *fhits)
      if (fei4_align->Distance( f , t ) < 1) {
        matchedTracks->push_back(t);
        break;
      }
  for (auto t : *matchedTracks){
    cluster * c = new cluster;
    c->x=dut_align->GetU(t->yPos,t->dydz);
    c->fx=dut_align->GetU(t->yPos,t->dydz);
    c->size=1;
    eClusters->push_back(c);
    if (dut_align->isGood(t))
      cleanClusters->push_back(c);
  }

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

void runProcessor::LoadAlign(string fName, string fei4Name, string dutName){
  TFile * f = new TFile(fName.c_str());
  TVector * v = (TVector * ) f->Get(fei4Name.c_str());
  if (v)
    fei4_align->LoadVector(v);
  else
    cout<<"Error!!! Cannot find object "<<fei4Name<<" in file "<<fName<<"!"<<endl;
  v = (TVector * ) f->Get(dutName.c_str());
  if (v)
    dut_align->LoadVector(v);
  else
    cout<<"Error!!! Cannot find object "<<dutName<<" in file "<<fName<<"!"<<endl;
  
}

void runProcessor::SetFei4Align(alignPar * p){
  delete fei4_align;
  fei4_align = p;
}
void runProcessor::SetDutAlign(alignPar * p){
  delete dut_align;
  dut_align = p;
}

alignPar * runProcessor::GetFei4Align(){
  return(fei4_align);
}
alignPar * runProcessor::GetDutAlign(){
  return(dut_align);
}


void runProcessor::SetEvtId(long int evtId){
  eventId = evtId;
}
long int runProcessor::GetEvtId(){
  return(eventId);
}
