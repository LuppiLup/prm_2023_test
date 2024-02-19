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
  //TString AMBER_DAQ_RECO_FILE = "/mnt/data/compass/phast.u7001.302117.root";
  TString AMBER_DAQ_RECO_FILE = "/localhome/henkel/Desktop/VMMData/prm_2023_B/test_data/phast.u7001.cdr11001-20-302117_WithoutGP.root";
  //TString VMM_DAQ_RECO_FILE = "/mnt/data/compass/run303925_20230921234249_woHits.root"; //"AMBER_2023_data/run314025_20230926155547_woHits.root";
  TString VMM_DAQ_RECO_FILE = "/mnt/data/Amber/2023/run304325_bc_44p444_tac_60_ccs_4_cs_2_dt_100_mst_1_spc_100_dp_80_cr_0p10-10p00_coin_center-of-mass_threshold=10.root";

  PutTime() << ": Get AMBER data..." << std::endl;
  AmberData AMBER(AMBER_DAQ_RECO_FILE.Data());
  PutTime() << ": Get VMM data..." << std::endl;
  VMMData VMM(VMM_DAQ_RECO_FILE.Data());

 

  PutTime() << ": VMM trigger times..." << std::endl;
  VMM.PlotTriggerTimes();
  PutTime() << ": VMM cluster times..." << std::endl;
  VMM.PlotClusterTimes();
  VMM.PlotCluster0Pos();
  VMM.PlotCluster1Pos();
  //Takes quite long
  PutTime() << ": VMM cluster trigger time diff..." << std::endl;

  //VMM.PlotClusterTriggerDiff();
  //VMM.PlotClusterTriggerDiffVMMs();
  //VMM.TimeResolution();

  PutTime() << ": VMM cluster trigger 2d..." << std::endl;
  //VMM.PlotClusterTrigger2D();



  PutTime() << ": Amber time stamps..." << std::endl;
  //AMBER.PlotTimeStamps(12);
  //PutTime() << ": Amber cluster time diff..." << std::endl;
  //double posA = 3189e09;    //spill 16
  //double posB = 3239e09;    //spill 18
  double posA = 4223e09;    //spill 58
  double posB = 4271e09;    //spill 60
  double peakposA = AMBER.CalibrateAmberTrigger(58, VMM.GetTriggerTimes(), posA);
  double peakposB = AMBER.CalibrateAmberTrigger(60, VMM.GetTriggerTimes(), posB);
  double shiftA = peakposA - posA + 1900;
  double shiftB = peakposB - posB + 1900;
  //AMBER.PlotClusterTimeDiff(16, VMM.GetCluster0Times(), shift16);
  // Takes a bit
  //AMBER.Residuals(58, VMM.GetCluster0Times(), VMM.GetCluster1Times(), VMM.GetCluster0Pos(), VMM.GetCluster1Pos(), shiftA);
  //AMBER.Residuals(60, VMM.GetCluster0Times(), VMM.GetCluster1Times(), VMM.GetCluster0Pos(), VMM.GetCluster1Pos(), shiftB);


  
  PutTime() << ": End of program!" << std::endl;
  app.Terminate(); // addem by Michael to properly clode TApplication in root 5 to prevent seg fault
  return 0;
}
