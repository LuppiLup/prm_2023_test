#include "TrackData.h"

TrackData::TrackData() : TObject() {}

void TrackData::Print() const {
  std::cout << std::boolalpha;
  std::cout << "Print track parameters" << std::endl;
  std::cout << "  " << nHits << " hits (" << ndf << " NDF), Chi2 = " << chi2 << std::endl;
  std::cout << "  (X = " << x << ",Y = " << y << " ,Z = " << z << "), gradX = " << gradX
            << ", gradY = " << gradY << std::endl;
  if (hasGoodMomentum) {
    std::cout << "  Measured good momentum: " << momentum << std::endl;
  } else {
    std::cout << "  No good momentum measured" << std::endl;
  }
  std::cout << "  Z-Range (Min - Max): " << zMin << " " << zMax << std::endl;
  std::cout << "  Can be muon? " << canBeMuon << ", IsMuPrimCoral: " << isMuPrimCoral
            << ", XX0: " << xx0 << std::endl;
  std::cout << "  Time: Mean = " << meanTime << ", Sigma = " << sigmaTime << ", Chi2 = " << chi2Time
            << std::endl;
  for (const TrackExtrapolationResult &extrapolation : extrapolationResults) {
    extrapolation.Print();
  }
}

ClassImp(TrackData);