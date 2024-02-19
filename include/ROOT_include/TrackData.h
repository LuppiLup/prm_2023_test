#pragma once

#include "TObject.h"
#include "TrackExtrapolationResult.h"
#include <iostream>
#include <string>
#include <vector>

struct TrackData : public TObject {
  TrackData();
  ClassDef(TrackData, 2);
  void Print() const;

  int32_t nHits;
  double chi2;
  int32_t ndf;

  double x, y, z, gradX, gradY;

  bool hasGoodMomentum;
  double momentum;

  double zMin, zMax;
  double xx0;
  bool canBeMuon, isMuPrimCoral;
  double meanTime, sigmaTime, chi2Time;

  bool trackIsMatched;

  std::vector<TrackExtrapolationResult> extrapolationResults;
};