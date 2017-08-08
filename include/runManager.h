#ifndef __RUN_MANAGER__
#define __RUN_MANAGER__
#include "MyHeader.h"
#include "DataFormats.h"
#include "alignToolbox.h"

class alignpar;

class runProcessor{
  private:
    long int eventId;
    long int nEntries;
    TFile * f;
    TTree * t;
    string fName, treeName;
    void ProduceVectors();
    void EmptyVectors();
    alignPar * fei4_align;
    alignPar * dut_align;
    vector <TObject *> histos;

  public:
    ~runProcessor();
    runProcessor(string fName, string treeName="analysisTree");
    dutEvent       * DUT ;
    condEvent      * COND; 
    TelescopeEvent * TEL ; 
    FeIFourEvent   * FEI4;
    vector <track *>    * tracks;
    vector <track *>    * matchedTracks;
    vector <FEI4hit *>  * fhits;
    vector <cluster *>  * eClusters;
    vector <cluster *>  * cleanClusters;
    bool * isGood;
    bool GetEvent();
    bool GetCleanEvent();
    void RegisterHisto(TObject *);
    void SaveToFile(string fName);
    void LoadAlign(string fName,string fei4Name="fei4Align",string dutName ="dutAlign");
    
    void     SetEvtId(long int evtId);
    long int GetEvtId();

    void SetFei4Align(alignPar * p);
    void SetDutAlign(alignPar * p);

    alignPar * GetFei4Align();
    alignPar * GetDutAlign();
};


#endif
