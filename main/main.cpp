#include <iomanip>
#include <iostream>
#include <cstdlib>

#include "ReadAmberData.h"
#include "ReadVmmData.h"
#include "TApplication.h"


std::ostream &PutTime() {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  std::cout << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
  return std::cout;
}

int main(int argc, char* argv[]) {
  TApplication app("app", &argc, argv);
  if (argc != 5) {
        std::cerr << "Usage" << std::endl;
        return 1;
  }
  int scan_number = std::atoi(argv[1]);
  int run_number = std::atoi(argv[2]);
  int spill_number = std::atoi(argv[3]);
  double spill_time = std::atof(argv[4]);

  std::cout << "scan   " << scan_number << std::endl;
  std::cout << "run   " << run_number << std::endl;
  std::cout << "spill   " << spill_number << std::endl;
  std::cout << "time_pos   " << spill_time << std::endl;
  

  //test 58-60
  //TString AMBER_DAQ_RECO_FILE = "/home/henkel/Desktop/VMMData/prm_2023_B/files/test_new_phast/phast.u7001.test_new_phast_05_29.root"; //spill 60
  TString AMBER_DAQ_RECO_FILE = Form("/mnt/data/Amber/2023/phast_data/phast_run_%d.root", run_number);

  //TString VMM_DAQ_RECO_FILE_cluster = Form ("/mnt/data/Amber/2023/run3543002/new_clustering_3543001-2/VMM_plane_clusters_spill_%d.root", arg2);
  //TString VMM_DAQ_RECO_FILE_trigger = Form ("/mnt/data/Amber/2023/run3543002/new_clustering_3543001-2/VMM_plane_clusters_spill_%d.root", arg2);

  TString VMM_DAQ_RECO_FILE_cluster = Form("/mnt/data/Amber/2023/voltage_scans/%d/%d_%d.root", scan_number, run_number, spill_number);
  //TString VMM_DAQ_RECO_FILE_cluster = Form("/mnt/data/Amber/2023/voltage_scans/%d/302117_58000.root", scan_number, run_number, spill_number);
  TString VMM_DAQ_RECO_FILE_trigger = Form("/mnt/data/Amber/2023/voltage_scans/%d/%d_%d.root", scan_number, run_number, spill_number);


  PutTime() << ": Get AMBER data..." << std::endl;
  AmberData AMBER(AMBER_DAQ_RECO_FILE.Data());
  PutTime() << ": Get VMM data..." << std::endl;

  
  VMMData VMM(VMM_DAQ_RECO_FILE_cluster.Data(), VMM_DAQ_RECO_FILE_trigger.Data(), spill_time);

 
  PutTime() << ": VMM trigger times..." << std::endl;
  VMM.PlotTriggerTimes();
  PutTime() << ": VMM cluster times..." << std::endl;
  VMM.PlotClusterTimes();
  VMM.PlotCluster0Pos();
  VMM.PlotCluster1Pos();
  //Takes quite long
  PutTime() << ": VMM cluster trigger time diff..." << std::endl;

  
  
  //VMM.PlotClusterTriggerDiff();
  //VMM.Synch_fecs(-1812.2, -1848.8, -1879.9, -1858.0); //302117
  //VMM.Synch_fecs(-1807.1, -1867.1, -1876.9, -1874.4); //302030
  //VMM.Synch_fecs(-1812.8, -1781.8, -1768.0, -1859.1);   //scan29 trigger ch 60
  //VMM.Synch_fecs(-1887.0, -1878.0, -1887.0, -1864.0);   //scan28 trigger ch 63

  if (scan_number == 21 && run_number == 302046) {
    VMM.Synch_fecs(-1911.0, -1880.0, -1889.0, -1912.0);
  }

  if (scan_number == 21 && run_number == 302047) {
    VMM.Synch_fecs(-1888.0, -1857.0, -1864.0, -1888.0);
  }
  
  if (scan_number == 22) {
    VMM.Synch_fecs(-1888.0, -1924.0, -1891.0, -1910.0);
  }

  if (scan_number == 23) {
    VMM.Synch_fecs(-1887.0, -1923.0, -1864.0, -1864.0);
  }

  if (scan_number == 26) {
    VMM.Synch_fecs(-1886.0, -1878.0, -1932.0, -1864.0);
  }

  if (scan_number == 27) {
    VMM.Synch_fecs(-1886.0, -1923.0, -1930.0, -1862.0);
  }

  if (scan_number == 28) {
    VMM.Synch_fecs(-1887.0, -1878.0, -1887.0, -1864.0);
  }

  if (scan_number == 29) {
    VMM.Synch_fecs(-1885, -1855, -1841, -1932);
  }

  if (scan_number == 30) {
    VMM.Synch_fecs(-1889, -1925, -1955, -1932);
  }

  if (scan_number == 31) {
    VMM.Synch_fecs(-1886, -1924, -1910, -1934);
  }

  if (scan_number == 32) {
    VMM.Synch_fecs(-1885, -1855, -1886, -1840);
  }

  if (scan_number == 33) {
    VMM.Synch_fecs(-1882, -1897, -1839, -1861);
  }


  std::cout << "check" << std::endl;

  PutTime() << ": VMM cluster trigger 2d..." << std::endl;

  
  
  PutTime() << ": Amber time stamps..." << std::endl;

 
  //int spillA = arg2;
  //double posA = arg1;


  auto resultA = AMBER.CalibrateAmberTrigger(scan_number, run_number, spill_number, VMM.GetTriggerTimes(), spill_time);
  double peakposA = std::get<0>(resultA);
  std::vector<bool> tracktimeA_matched = std::get<1>(resultA);
  std::vector<double> tracktimeA = std::get<2>(resultA);
  std::vector<double> trigger_TCS_A = std::get<3>(resultA);
  std::vector<double> TimeInSpill_calibrated_A = std::get<4>(resultA);
  double shiftA = peakposA - spill_time;
  AMBER.Residuals(scan_number, run_number, spill_number, VMM.GetCluster0fec(), VMM.GetCluster1fec(), VMM.GetCluster0Times(), VMM.GetCluster1Times(), VMM.GetCluster0Pos(), VMM.GetCluster1Pos(), VMM.GetCluster0adc(), VMM.GetCluster1adc(), VMM.GetCluster0size(), VMM.GetCluster1size(), shiftA, tracktimeA_matched, tracktimeA, trigger_TCS_A, TimeInSpill_calibrated_A);
  
  
  PutTime() << ": End of program!" << std::endl;
  app.Terminate(); // addem by Michael to properly clode TApplication in root 5 to prevent seg fault
  return 0;
}
