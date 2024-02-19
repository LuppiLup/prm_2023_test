#pragma once

#include <string>
#include <vector>

#include "TROOT.h"
#include "TrackData.h"

#include "TTreeReader.h"
#include "TTreeReaderValue.h"



struct PerSpill {
  std::vector<double> timeStamps;
  std::vector<double> tracks_x;
  std::vector<double> tracks_y;
  std::vector<double> tracks_z;
  Long64_t SpillNumber;
};

class AmberData {
public:
  AmberData(const std::string &DataFile);

  /*Make a plot where all time stamps of amber trigger data for a given spill is shown*/
  void PlotTimeStamps(Long64_t spill) const;

  /*Plot the difference of all time stamps of amber in a given time frame to vmm time stamps*/
  void PlotClusterTimeDiff(Long64_t spill, const std::vector<double> &VMMTimeStamps, double timeshift);


  double CalibrateAmberTrigger(Long64_t spill, const std::vector<double>& TriggerTimeStamps, double spill_pos);

  void Residuals(Long64_t spill, const std::vector<double>& VMMxTimeStamps, const std::vector<double>& VMMyTimeStamps, const std::vector<double>& VMM_xpos, const std::vector<double>& VMM_ypos, double timeshift);

private:
  std::vector<PerSpill> mPerSpillData;
  // clock correction
  double alpha = 3.218e-5;
  TTreeReaderValue<std::vector<TrackData>> *spectrometerTracks;
  TTreeReader *treeReader = nullptr;
};