#pragma once

#include <string>
#include <vector>

class VMMData {
public:
  VMMData(const std::string &DataFile);

  /*Plot all vmm cluster positions*/
  void PlotCluster0Pos() const;
  void PlotCluster1Pos() const;

  /*Plot all vmm cluster times*/
  void PlotClusterTimes() const;

  /*Plot all amber triggers collected with the vmm daq*/
  void PlotTriggerTimes() const;

  /*Plot the difference of all amber triggers with all vmm cluster times*/
  void PlotClusterTriggerDiff() const;

  /*Get time resolution by comparing planes*/
  void TimeResolution() const;

  /*Make 4 plots to show the distribution of cluster times and amber triggers. Just in the spills*/
  void PlotClusterTrigger2D() const;

  const std::vector<double>& GetTriggerTimes() const { return mTriggerTimes; }

  const std::vector<double> &GetClusterTimes() const { return mCluster0Times; }

  const std::vector<double> &GetCluster0Pos() const { return mCluster0Pos; }

  const std::vector<double>& GetCluster1Pos() const { return mCluster1Pos; }

  const std::vector<int>& GetCluster0fec() const { return mCluster0fec; }

  const std::vector<int>& GetCluster1fec() const { return mCluster1fec; }



private:
  std::vector<double> mCluster0Times;
  std::vector<double> mCluster1Times;
  std::vector<double> mCluster0Pos;
  std::vector<double> mCluster1Pos;
  std::vector<int> mCluster0fec;
  std::vector<int> mCluster1fec;
  std::vector<double> mTriggerTimes;
};
