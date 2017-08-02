#include "DataFormats.h"
#include <iostream>
ClassImp(tbeam::cbc)
ClassImp(tbeam::cluster)
ClassImp(tbeam::stub)
ClassImp(tbeam::dutEvent)
ClassImp(tbeam::condEvent)
ClassImp(tbeam::TelescopeEvent)

namespace tbeam {
tbeam::alignmentPars::alignmentPars() 
{
  FEI4_z = 0.;
  d0_chi2_min_z = 0.;
  d1_chi2_min_z = 0.;
  d0_Offset_aligned = 0.;
  d1_Offset_aligned = 0.;
  deltaz = 0.;
  angle = 0.;
  resSigmaFEI4X = 0.;
  resSigmaFEI4Y = 0.;
  offFEI4X = 0.;
  offFEI4Y = 0.;
}
tbeam::alignmentPars::alignmentPars(const double fei4Z, const double resfei4X, const double resfei4Y, const double offxfei4, const double offyfei4, 
                                    const double off_d0, const double zDUT_d0, const double dZ, const double t)
{
  FEI4_z = fei4Z;
  d0_chi2_min_z = zDUT_d0;
  d0_Offset_aligned = off_d0;
  deltaz = dZ;
  angle = TMath::Pi()*t/180.;
  d1_chi2_min_z = d0_chi2_min_z + deltaz*TMath::Cos(angle);
  d1_Offset_aligned = d0_Offset_aligned + TMath::Sin(angle)*deltaz;
  resSigmaFEI4X = resfei4X;
  resSigmaFEI4Y = resfei4Y;
  offFEI4X = offxfei4;
  offFEI4Y = offyfei4;
}

 void tbeam::alignmentPars::setD1parametersfromD0() 
{
  d1_chi2_min_z = d0_chi2_min_z + deltaz*TMath::Cos(angle);
  d1_Offset_aligned = d0_Offset_aligned + TMath::Sin(angle)*deltaz;

}
std::ostream& operator<< ( std::ostream& out, const tbeam::alignmentPars& a )
{
  out << "----Alignment Parameters----\n";
  out << "Fei4Z=" << a.FEI4z()
      << "\nd0_chi2_min_z=" << a.d0Z()
      << "\nd0_Offset_aligned=" << a.d0Offset()
      << "\nd1_chi2_min_z=" << a.d1Z()
      << "\nd1_Offset_aligned=" << a.d1Offset()
      << "\ndeltaZ=" << a.deltaZ()
      << "\nangle(rad)=" << a.theta()
      << "\nresidualSigmaFEI4x=" << a.residualSigmaFEI4x()
      << "\nresidualSigmaFEI4y=" << a.residualSigmaFEI4y()
      << "\noffsetFEI4x=" << a.offsetFEI4x()
      << "\noffsetFEI4y=" << a.offsetFEI4y()
      << std::endl;
  return out;
}

tbeam::cbc::cbc():
   pipelineAdd(0),
   status(0),
   error(0)
{
}

tbeam::cluster::cluster():
   x(0),
   fx(0.),
   size(0)
{
 // cout<<"Create Clust"<<endl;
}

tbeam::cluster::~cluster(){
 // cout<<"del Clust"<<endl;
}

tbeam::stub::stub():
   x(0),
   fx(0.),
   direction(0)
{
 //  cout<<"cstub"<<endl;
   seeding = 0;//new tbeam::cluster();
   matched = 0;//new tbeam::cluster();
}

tbeam::stub::~stub(){
}

tbeam::dutEvent::dutEvent():
   stubWord(0),
   stubWordReco(0)
{
  //cout<<"c"<<endl;
   //isGood(1)
   //stubs=std::vector<tbeam::stub*>();
}

tbeam::dutEvent::~dutEvent(){
//   cout<<"@@@@ DELETING EVENT @@@@ "<<endl; 
   //std::cout << "Entering dutEvent destructor!" << std::endl;   
//   cout<<"---> Looping on stubs..."<<endl;
   for (unsigned int i=0; i<stubs.size(); i++)
     if(stubs.at(i))
       delete stubs.at(i);
   stubs.clear();

//   cout<<"---> Looping on stubs DONE !"<<endl;
//   cout<<"---> Looping on clusters.. "<<endl;
   for(std::map<std::string,std::vector<tbeam::cluster *> >::iterator it = clusters.begin(); it != clusters.end(); ++it) {
      //for(std::vector<tbeam::cluster *>::iterator cl = it->second.begin(); cl!=it->second.end(); ++cl){
      for(unsigned int i =0; i<it->second.size(); i++){
         if(it->second.at(i)) {
           delete it->second.at(i);
           it->second.at(i)=0;
         }
      }
      //      clusters.clear();
   }
//   cout<<"@@@@ DONE DELETING EVENT @@@@"<<endl;
   //std::cout << "Leaving dutEvent destructor!" << std::endl;   
}

tbeam::condEvent::condEvent() :
   run(999999), 
   lumiSection(999999), 
   event(999999),  
   time(999999), 
   unixtime(999999), 
   tdcPhase(999999),
   HVsettings(999999),
   DUTangle(999999),
   window(999999),
   offset(999999),
   cwd(999999),
   tilt(999999),
   vcth(999999),
   stubLatency(999999),
   triggerLatency(999999),
   condData(999),
   glibStatus(9999)
{
  //cbcs = std::vector<tbeam::cbc>();
}

tbeam::TelescopeEvent::TelescopeEvent() 
{
   xPos = new vector<double>();
   yPos = new vector<double>();
   dxdz = new vector<double>();
   dydz = new vector<double>();
   trackNum = new vector<int>();
   iden = new vector<int>();
   chi2 = new vector<double>();
   ndof = new vector<double>();
}

tbeam::TelescopeEvent::~TelescopeEvent() {
   delete xPos;
   delete yPos;
   delete dxdz;
   delete dydz;
   delete trackNum;
   delete iden;
   delete chi2;
   delete ndof;
}

tbeam::FeIFourEvent::FeIFourEvent() {
  col = new vector<int>();
  row =new vector<int>();
  tot = new vector<int>();
  lv1 = new vector<int>();
  iden = new vector<int>();
  hitTime = new vector<int>();
  frameTime = new vector<double>();  
}
tbeam::FeIFourEvent::~FeIFourEvent() {
  delete     col;
  delete     row;
  delete     tot;
  delete     lv1;
  delete     iden;
  delete     hitTime;
  delete     frameTime;
}

tbeam::Track::Track() {
  trkIndex = 0.;
  xPos = 0.;
  yPos = 0.;
  dxdz = 0.;
  dydz = 0.;
  chi2 = 0.;
  ndof = 0.;
  xtkDut0 = 0.;
  xtkDut1 = 0.;
}
tbeam::Track::Track(int i, double x, double y, double sx, double sy, double c2, double ndf) {
  trkIndex = i;
  xPos = x;
  yPos = y;
  dxdz = sx;
  dydz = sy;
  chi2 = c2;
  ndof = ndf;
  xtkDut0 = 0.;
  xtkDut1 = 0.;
  ytkDut0 = 0.;
  ytkDut1 = 0.;

}

tbeam::Track::Track(int i, double x, double y, double sx, double sy, double c2, double ndf, double xtk0, double xtk1, double ytk0, double ytk1) {
  trkIndex = i;
  xPos = x;
  yPos = y;
  dxdz = sx;
  dydz = sy;
  chi2 = c2;
  ndof = ndf;
  xtkDut0 = xtk0;
  xtkDut1 = xtk0;
  ytkDut0 = ytk0;
  ytkDut1 = ytk0;
}

}
