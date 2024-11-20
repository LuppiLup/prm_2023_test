#pragma once

#include <string>
#include <vector>

class VMMData {
public:
  VMMData(const std::string &DataFile_cluster, const std::string &DataFile_trigger, int spill);

  /*Plot all vmm cluster positions*/
  void PlotCluster0Pos() const;
  void PlotCluster1Pos() const;

  /*Plot all vmm cluster times*/
  void PlotClusterTimes() const;

  /*Plot all amber triggers collected with the vmm daq*/
  void PlotTriggerTimes() const;

  /*Plot the difference of all amber triggers with all vmm cluster times*/
  void PlotClusterTriggerDiff() const;
  void Synch_fecs(double shift_1, double shift_2, double shift_4, double shift_5);


  /*Make 4 plots to show the distribution of cluster times and amber triggers. Just in the spills*/
  void PlotClusterTrigger2D() const;

  const std::vector<double>& GetTriggerTimes() const { return mTriggerTimes; }

  const std::vector<double> &GetCluster0Times() const { return mCluster0Times; }

  const std::vector<double>& GetCluster1Times() const { return mCluster1Times; }

  const std::vector<double> &GetCluster0Pos() const { return mCluster0Pos; }

  const std::vector<double>& GetCluster1Pos() const { return mCluster1Pos; }

  const std::vector<int>& GetCluster0fec() const { return mCluster0fec; }

  const std::vector<int>& GetCluster1fec() const { return mCluster1fec; }

  const std::vector<int>& GetCluster0adc() const { return mCluster0adc; }

  const std::vector<int>& GetCluster1adc() const { return mCluster1adc; }

  const std::vector<int>& GetCluster0size() const { return mCluster0size; }

  const std::vector<int>& GetCluster1size() const { return mCluster1size; }

  const std::vector<double>& GetHitTime() const { return mHitTime; }

  const std::vector<int>& GetHitFEC() const { return mHitFEC; }

  const std::vector<int>& GetHitVMM() const { return mHitVMM; }



private:
  std::vector<double> mCluster0Times;
  std::vector<double> mCluster1Times;
  std::vector<double> mCluster0Pos;
  std::vector<double> mCluster1Pos;
  std::vector<int> mCluster0fec;
  std::vector<int> mCluster1fec;
  std::vector<int> mCluster0adc;
  std::vector<int> mCluster1adc;
  std::vector<int> mCluster0size;
  std::vector<int> mCluster1size;
  std::vector<double> mTriggerTimes;
  std::vector<double> mHitTime;
  std::vector<int> mHitFEC;
  std::vector<int> mHitVMM;
};
