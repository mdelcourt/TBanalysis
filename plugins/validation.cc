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

  TH1 * yDist = new TH1F(name.c_str(), name.c_str(),500,-20,20);
  for (int i=0; i<1e5; i++){
    r->GetEvent();
    if (matched){
      if(r->eClusters->size()-r->matchedTracks->size() !=0)
        cout<<"MISMATCH !!!"<<endl;
      for (unsigned int trkId = 0; trkId<r->matchedTracks->size(); trkId++){
        for (auto c : r->DUT->clusters["det0"]){
          if(abs(r->eClusters->at(trkId)->fx - c->fx) <2){
            yDist->Fill(r->GetDutAlign()->GetV(r->matchedTracks->at(trkId)->xPos,r->matchedTracks->at(trkId)->dxdz));
            break;
          }
        }
      }
    }
    else{
      for (auto t : *r->matchedTracks)
        yDist->Fill(r->GetDutAlign()->GetV(t->xPos,t->dxdz));
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
    if (n>d)
      cout<<"Error, n>d !!!"<<endl;
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




int main(int narg, char * argv []){
  cout<<"Received "<<narg<<" arguments :"<<endl;
  for (int i=0; i<narg; i++)
    cout<<argv[i]<<"  ";
  cout<<endl;


  runProcessor * r = new runProcessor("/home/ucl/cp3/delcourt/storage/tbFullTree/1611/AnalysisTree_0926.root");
  r->LoadAlign("fsc.root");
  r->GetDutAlign()->beta = 1;
  
  r->RegisterHisto(getResiduals(r));
  r->SetEvtId(0);
  TH1 * h1 = getYdist(r);
  r->SetEvtId(0);
  TH1 * h2 = getYdist(r,true);
  TH1 * h3 = computeEff(h2,h1);
  r->RegisterHisto(h1);
  r->RegisterHisto(h2);
  r->RegisterHisto(h3);
  r->SaveToFile("fsc_validation.root");
  //delete r; 
}
