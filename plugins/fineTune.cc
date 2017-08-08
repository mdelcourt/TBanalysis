#include "MyHeader.h"
#include "runManager.h"

float fitMin(TGraph * g, float min = 0, float step = 0.04){
  TF1 * myFunc = new TF1("myFunc","pol2");
  float bound_minus = min - 30*step;
  float bound_plus  = min + 30*step;
  g->Fit(myFunc, "", "", bound_minus, bound_plus);

  cout<<" Min     = "<<min<<endl;
  float fMin = -myFunc->GetParameter(1)/(2.*myFunc->GetParameter(2));
  cout<<" Fit min = "<<fMin<<endl;
  return(fMin);
}

void processPar(runProcessor * r, alignPar * p, float & val, string name, int DUT = -1, float range = 1., bool fine = false){
  int Nevt = 5000;
  if (fine)
    Nevt=50000;
  cout<<"Processing parameter "<<name<<endl;
  TGraph * g = p->OptimisePar(val , r, name, DUT, range,50,Nevt);
  r->RegisterHisto(g);
  val = fitMin(g,val, range*1./50);

  p->Print();
}


TH2 * getY (runProcessor * r, int nEvent = 50000, bool clean = false){
  cout<<"Filling Y scatter plot"<<endl;
  TH2 * Scatter_plot = new TH2I("Y_axis_correlation", "Y_axis_correlation",1000,-10,10, 1000,0,500);
  for (int evtId = 0; evtId<nEvent; evtId++){
    if (clean)
      r->GetCleanEvent();
    else
      r->GetEvent();
    for (auto tr : *(r->tracks))
      for (auto c: *(r->fhits))
        Scatter_plot->Fill(tr->yPos,c->row);
  } 
  return(Scatter_plot);

}


TH2 * getX (runProcessor * r, int nEvent = 50000, bool clean = false){
  cout<<"Filling X scatter plot"<<endl;
  TH2 * Scatter_plot = new TH2I("X_axis_correlation", "X_axis_correlation",1000,-10,10, 1000,0,100);
  for (int evtId = 0; evtId<nEvent; evtId++){
    if (clean)
      r->GetCleanEvent();
    else
      r->GetEvent();
    for (auto tr : *(r->tracks))
      for (auto c: *(r->fhits))
        Scatter_plot->Fill(tr->xPos,c->col);
  } 
  return(Scatter_plot);

}

TH2 * getX_DUT(runProcessor * r, string detId = "det0", int nEvent = 50000, bool clean = false){
  cout<<"Filling X scatter plot"<<endl;
  TH2 * Scatter_plot = new TH2I("DUT_X_axis_correlation", "DUT_X_axis_correlation",1000,-10,10, 1000,0,300);
  for (int evtId = 0; evtId<nEvent; evtId++){
    if (clean)
      r->GetCleanEvent();
    else
      r->GetEvent();
    for (auto tr : *(r->matchedTracks))
      for (auto c: (r->DUT->clusters[detId.c_str()]))
        Scatter_plot->Fill(tr->yPos,c->x);
  }
  return(Scatter_plot);

}



TH1 * flatten(TH2 * h , int minEntries=100){
  stringstream ss;
  ss<<h->GetTitle()<<"_flat";
  TH1 * flatHist = (TH1 * ) h->ProjectionX(ss.str().c_str(),0,0);

  for (int binId = 0; binId < flatHist->GetXaxis()->GetNbins(); binId++){
    TH1 * pr = h->ProjectionY("pr",binId,binId);
    double max = 0;
    double stdDev = 0;
    if (pr->GetEntries()>=minEntries){
      max    = pr->GetXaxis()->GetBinCenter(pr->GetMaximumBin());
      stdDev = pr->GetStdDev();
    }
    flatHist->SetBinContent(binId,max);
    flatHist->SetBinError(binId,stdDev);
  }
  return(flatHist);

}

TH1 * getResiduals(runProcessor * r){
  TH1 * res = new TH1I("residuals","residuals", 1000,-5,5);
  for (int evtId=0; evtId<1e4; evtId++){
    r->GetEvent();
    if (r->eClusters->size()==1 && r->DUT->clusters["det0"].size() ==1){
      res->Fill(r->eClusters->at(0)->fx-r->DUT->clusters["det0"].at(0)->fx);
    }

  }
  return res;
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
  cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
  cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;



}



int main(int narg, char * argv []){
  init(narg,argv,"fineTune");    
  cout<<"Data file = "<<dataFile<<endl;
  runProcessor * r = new runProcessor(dataFile);
  r->LoadAlign(fileIn);
  alignPar p0 = *(r->GetFei4Align());
  alignPar p1 = *(r->GetDutAlign());
  cout<<"FEI4 align parameters : "<<endl;
  p0.Print();
  cout<<"DUT align parameters : "<<endl;
  p1.Print();

  cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
  cout<<"@@@@                        @@@@@"<<endl;
  cout<<"@@@@        ALIGNING        @@@@@"<<endl;
  cout<<"@@@@          FEI4          @@@@@"<<endl;
  cout<<"@@@@                        @@@@@"<<endl;
  cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
  processPar(r, &p0, p0.alpha, "FEI4_alpha_1",-1,2);
  processPar(r, &p0, p0.beta , "FEI4_beta_1" ,-1,3);
  processPar(r, &p0, p0.U0   , "FEI4_U0_1"   ,-1,20);
  processPar(r, &p0, p0.V0   , "FEI4_V0_1"   ,-1,20);
  processPar(r, &p0, p0.D    , "FEI4_D_1"    ,-1,3000);


/*  processPar(r, &p0, p0.alpha, "FEI4_alpha_2",-1,0.5);
  processPar(r, &p0, p0.beta , "FEI4_beta_2" ,-1,0.8);
  processPar(r, &p0, p0.U0   , "FEI4_U0_2"   ,-1,2);
  processPar(r, &p0, p0.V0   , "FEI4_V0_2"   ,-1,2);
  processPar(r, &p0, p0.D    , "FEI4_D_2"    ,-1,500);


  processPar(r, &p0, p0.alpha, "FEI4_alpha_3",-1,0.15,true);
  processPar(r, &p0, p0.beta , "FEI4_beta_3" ,-1,0.3 ,true);
  processPar(r, &p0, p0.U0   , "FEI4_U0_3"   ,-1,0.2 ,true);
  processPar(r, &p0, p0.V0   , "FEI4_V0_3"   ,-1,0.5 ,true);
  processPar(r, &p0, p0.D    , "FEI4_D_3"    ,-1,200 ,true);*/


  cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
  cout<<"@@@@                        @@@@@"<<endl;
  cout<<"@@@@        ALIGNING        @@@@@"<<endl;
  cout<<"@@@@          DUT           @@@@@"<<endl;
  cout<<"@@@@                        @@@@@"<<endl;
  cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
  r->SetFei4Align(&p0);

  processPar(r, &p1, p1.alpha, "DUT_alpha_1",0,3);
  processPar(r, &p1, p1.U0   , "DUT_U0_1"   ,0,5);
  processPar(r, &p1, p1.D    , "DUT_D_1"    ,0,5000);
  r->SetDutAlign(&p1);
  r->SaveToFile(fileOut);

/*  processPar(r, &p1, p1.alpha, "DUT_alpha_2",0,0.8);
  processPar(r, &p1, p1.U0   , "DUT_U0_2"   ,0,1.5);
  processPar(r, &p1, p1.D    , "DUT_D_2"    ,0,1000);
  r->SaveToFile(fileOut);

  processPar(r, &p1, p1.alpha, "DUT_alpha_3",0,0.3, true);
  processPar(r, &p1, p1.U0   , "DUT_U0_3"   ,0,0.6, true);
  processPar(r, &p1, p1.D    , "DUT_D_3"    ,0,350, true);*/

  r->RegisterHisto(getResiduals(r));


  r->SaveToFile(fileOut);
  //delete r; 
}
