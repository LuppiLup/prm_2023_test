#pragma once

#include <string>
#include <vector>

#include "TROOT.h"
#include "TrackData.h"

#include "TTreeReader.h"
#include "TTreeReaderValue.h"



struct PerSpill {
  std::vector<double> timeStamps;
  std::vector<float> TCS;
  std::vector<double> meanTrackTime;;
  std::vector<double> sigmaTrackTime;
  std::vector<double> tracks_x;
  std::vector<double> tracks_y;
  std::vector<double> tracks_sigma_x;
  std::vector<double> tracks_sigma_y;
  std::vector<double> tracks_z;
  std::vector<bool> boundaries_cut;
  Long64_t SpillNumber;
};

class AmberData {
public:
  AmberData(const std::string &DataFile);

  /*Make a plot where all time stamps of amber trigger data for a given spill is shown*/
  void PlotTimeStamps(Long64_t spill) const;

  void TrackTime(Long64_t spill) const;

  /*Plot the difference of all time stamps of amber in a given time frame to vmm time stamps*/
  void PlotClusterTimeDiff(Long64_t spill, const std::vector<double> &VMMTimeStamps, double timeshift);


  std::tuple<double, std::vector<bool>, std::vector<double>, std::vector<double>, std::vector<double>> CalibrateAmberTrigger(Long64_t VMM_scan_number, Long64_t AMBER_run_number, Long64_t spill, const std::vector<double>& TriggerTimeStamps, double spill_pos);

  void Residuals(Long64_t VMM_scan_number, Long64_t AMBER_run_number, Long64_t spill, const std::vector<int>& VMMxfec, const std::vector<int>& VMMyfec, const std::vector<double>& VMMxTimeStamps, const std::vector<double>& VMMyTimeStamps, const std::vector<double>& VMM_xpos, const std::vector<double>& VMM_ypos, const std::vector<int>& x_adc, const std::vector<int>& y_adc, const std::vector<int>& x_size, const std::vector<int>& y_size, double timeshift, const std::vector<bool>& tracktime_matched, const std::vector<double>& tracktime, const std::vector<double>& trigger_TCS, const std::vector<double>& TimeInSpill_calibrated);

private:
  std::vector<PerSpill> mPerSpillData;
  // clock correction
  double alpha = 3.218e-5;// + 0.06e-5;
  TTreeReaderValue<std::vector<TrackData>> *spectrometerTracks;
  TTreeReader *treeReader = nullptr;
};
