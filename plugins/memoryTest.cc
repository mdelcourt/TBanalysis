

#include "MyHeader.h"
#include "runManager.h"

int main(){

  runProcessor * r = new runProcessor("/home/ucl/cp3/delcourt/storage/tbFullTree/1611/AnalysisTree_0926.root");

  while(1)
    r->GetEvent();


}
