#pragma once

#include <string>
#include <vector>

class VMMData {
public:
  VMMData(const std::string &DataFile);

  /*Plot all vmm cluster times*/
  void PlotClusterTimes() const;

  /*Plot all amber triggers collected with the vmm daq*/
  void PlotTriggerTimes() const;

  /*Plot the difference of all amber triggers with all vmm cluster times*/
  void PlotClusterTriggerDiff() const;

  /*Make 4 plots to show the distribution of cluster times and amber triggers. Just in the spills*/
  void PlotClusterTrigger2D() const;

  const std::vector<double> &GetClusterTimes() const { return mClusterTimes; }

private:
  std::vector<double> mClusterTimes;
  std::vector<double> mTriggerTimes;
};