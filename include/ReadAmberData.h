#pragma once

#include <string>
#include <vector>

#include "TROOT.h"

struct PerSpill {
  std::vector<double> timeStamps;
  Long64_t SpillNumber;
};

class AmberData {
public:
  AmberData(const std::string &DataFile);

  /*Make a plot where all time stamps of amber trigger data for a given spill is shown*/
  void PlotTimeStamps(Long64_t spill) const;

  /*Plot the difference of all time stamps of amber in a given time frame to vmm time stamps*/
  void PlotClusterTimeDiff(Long64_t spill, const std::vector<double> &VMMTimeStamps);


  double CalibrateAmberTrigger(Long64_t spill, const std::vector<double>& TriggerTimeStamps, double spill_pos);

private:
  std::vector<PerSpill> mPerSpillData;
  // clock correction
  double alpha = 3.218e-5;
};