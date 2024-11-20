#pragma once
#include "TObject.h"
#include <iostream>
#include <string>
#include <TMatrix.h>

struct TrackExtrapolationResult : public TObject {
  TrackExtrapolationResult();
  ClassDef(TrackExtrapolationResult, 3);
  void Print() const;

  double z = 0;
  std::string name = "Default";
  bool useMaterialMap = false;
  bool usePropagationMatrix = false;

  bool successful = false;
  double x = 0;
  double y = 0;
  double dxdz = 0;
  double dydz = 0;

  bool covMatrixIsSet = false;
  double covMatrix[5][5]; // Only saved for straight line extrapolation
};
