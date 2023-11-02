#include <iomanip>
#include <iostream>

#include "ReadAmberData.h"
#include "ReadVmmData.h"
#include "TApplication.h"

std::ostream &PutTime() {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  std::cout << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
  return std::cout;
}

int main(int argc, char **argv) {
  TApplication app("app", &argc, argv); // added by Michael to resolve seg fault in root 5
  TString AMBER_DAQ_RECO_FILE = "/mnt/data/compass/phast.u7001.302117.root";
  TString VMM_DAQ_RECO_FILE = "/mnt/data/compass/run303925_20230921234249_woHits.root"; //"AMBER_2023_data/run314025_20230926155547_woHits.root";

  PutTime() << ": Get AMBER data..." << std::endl;
  AmberData AMBER(AMBER_DAQ_RECO_FILE.Data());
  PutTime() << ": Get VMM data..." << std::endl;
  VMMData VMM(VMM_DAQ_RECO_FILE.Data());

  PutTime() << ": Amber time stamps..." << std::endl;
  AMBER.PlotTimeStamps(12);
  double x = AMBER.CalibrateAmberTrigger(16, VMM.GetTriggerTimes(), 3189e09);
  //AMBER.PlotTimeStamps(13);
// Takes a bit
//   PutTime() << ": Amber cluster time diff..." << std::endl;
//   AMBER.PlotClusterTimeDiff(12, VMM.GetClusterTimes());

  PutTime() << ": VMM trigger times..." << std::endl;
  VMM.PlotTriggerTimes();
  PutTime() << ": VMM cluster times..." << std::endl;
  VMM.PlotClusterTimes();
// Takes quite long
   PutTime() << ": VMM cluster trigger time diff..." << std::endl;
   //VMM.PlotClusterTriggerDiff();
  PutTime() << ": VMM cluster trigger 2d..." << std::endl;
  //VMM.PlotClusterTrigger2D();
  PutTime() << "  " << x << std::endl;
  PutTime() << ": End of program!" << std::endl;
  app.Terminate(); // addem by Michael to properly clode TApplication in root 5 to prevent seg fault
  return 0;
}
