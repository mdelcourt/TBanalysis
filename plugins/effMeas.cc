#include "MyHeader.h"
#include "runManager.h"

TH1 * getResiduals(runProcessor * r){
  TH1 * res = new TH1F("residuals","residuals", 1000,-5,5);
  for (int evtId=0; evtId<1e4; evtId++){
    r->GetEvent();
    if (r->eClusters->size()==1 && r->DUT->clusters["det0"].size() ==1){
      res->Fill(r->eClusters->at(0)->fx-r->DUT->clusters["det0"].at(0)->fx);
    }

  }
  return res;
}

TH1 * getYdist(runProcessor * r, bool matched = false){
  string name = "Y_distribution";
  if (matched)
    name += "_matched";

  TH1 * yDist = new TH1F(name.c_str(), name.c_str(),50,-5,10);
  for (int i=0; i<1e5; i++){
    r->GetEvent();
    if (matched){
      for (auto t : *r->matchedTracks)
        yDist->Fill(t->yPos);
    }
    else{
      for (auto t : *r->tracks)
        yDist->Fill(t->yPos);
    }
  }
  return(yDist);
}

TH1 * computeEff(TH1 * h_n, TH1 * h_d){
  string name="";
  name +=h_n->GetTitle();
  name +="_eff";
  TH1 * h_eff = (TH1D *) h_n->Clone(name.c_str());
  
  for (int binId = 0; binId < h_n->GetXaxis()->GetNbins(); binId++){
    double n = h_n->GetBinContent(binId);
    double d = h_d->GetBinContent(binId);
    
    double e = 0;
    double de= 0;
    if (d>0){
      e=n*1./d;
      de=sqrt(e*(1-e)*1./d);
    }
    h_eff->SetBinContent(binId,e);
    h_eff->SetBinError(binId,de);
  }
  return(h_eff);
}

void getEff(runProcessor * r){
  r->SetEvtId(0);
  TH1 * h_ext  = new TH1D("extrapolated","extrapolated",256,0,256);
  TH1 * h_det0 = new TH1D("det0"     ,"det0"     ,256,0,256);
  TH1 * h_det1 = new TH1D("det1"     ,"det1"     ,256,0,256);
  while (r->GetEvent()){
    for (auto c : *r->cleanClusters){
      h_ext->Fill(c->fx);
      for (auto c0 : r->DUT->clusters["det0"]){
        if (abs(c->fx-c0->fx)<3){
         h_det0->Fill(c->fx); 
         break;
        }
      }
      for(auto c1 : r->DUT->clusters["det1"]){
        if (abs(c->fx-c1->fx)<3){
          h_det1->Fill(c->fx);
          break;
        }
      }
    }
  }

  TH1 * hf_1 = computeEff(h_det0,h_ext);
  TH1 * hf_2 = computeEff(h_det1,h_ext);

  r->RegisterHisto(h_ext);
  r->RegisterHisto(h_det0);
  r->RegisterHisto(h_det1);
  r->RegisterHisto(hf_1);
  r->RegisterHisto(hf_2);
}
string fileIn   = "";
string fileOut  = "";
string dataFile = "";

void init(int narg, char * argv[], string processName){
  cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
  cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
  cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
  cout<<endl<<"Starting process "<<processName<<endl;


  cout<<endl<<"Received "<<narg<<" arguments :"<<endl;
  for (int i=0; i<narg; i++)
    cout<<argv[i]<<"  ";
  cout<<endl<<endl;

  if (narg==1){
    cout<<"No arguments given, will process default files !!!"<<endl;
    dataFile = "/home/ucl/cp3/delcourt/storage/tbFullTree/1611/AnalysisTree_0926.root";
    fileIn   = "default.root";
    fileOut  = processName+"_926.root";
  }
  else if (narg==2){
    cout<<"Missing 2 arguments, will consume and produce default files !!!"<<endl;
    dataFile= argv[1];
    fileIn  = "default.root";
    fileOut = processName+".root";
  }
  else if (narg==3){
    cout<<"Missing 1 argument, will produce default file !!!"<<endl;
    dataFile = argv[1];
    fileIn   = argv[2];
    fileOut  = processName+".root";
  }
  else{
    dataFile = argv[1];
    fileIn   = argv[2];
    fileOut  = argv[3];
    if (narg>4)
      cout<<"Too many arguments !!! Ignoring after first two."<<endl;
  }
  cout<<endl<<"Data file : "<<dataFile<<endl;
  cout<<endl<<"File in   : "<<fileIn<<endl;
  cout<<endl<<"File out  : "<<fileOut<<endl<<endl;
  cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
}


int main(int narg, char * argv []){

  init(narg,argv,"effMeas");


  runProcessor * r = new runProcessor(dataFile);
  r->LoadAlign(fileIn);
  r->GetDutAlign()->Vl = -2;
  r->GetDutAlign()->Vh = 6;

  r->GetDutAlign()->Ul = -999;
  r->GetDutAlign()->Uh = 999;

  r->RegisterHisto(getResiduals(r));
  TH1 * h1 = getYdist(r);
  TH1 * h2 = getYdist(r,true);
  TH1 * h3 = computeEff(h2,h1);
  r->RegisterHisto(h1);
  r->RegisterHisto(h2);
  r->RegisterHisto(h3);

  getEff(r);
  r->SaveToFile(fileOut);
  //delete r; 
}
