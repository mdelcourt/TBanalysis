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
    vector <track *>   * tracks;
    vector <FEI4hit *> * fhits;
    bool * isGood;
    bool GetEvent();
    bool GetCleanEvent();
    void RegisterHisto(TObject *);
    void SaveToFile(string fName);
};


#endif
