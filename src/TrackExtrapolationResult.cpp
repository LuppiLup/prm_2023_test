#include "TrackExtrapolationResult.h"

TrackExtrapolationResult::TrackExtrapolationResult() : TObject() {}

void TrackExtrapolationResult::Print() const {
  std::cout << "  Track extrapolation to " << name << " (Z = " << z << ")" << std::flush;
  if (usePropagationMatrix) {
    std::cout << " using transportation matrix (over M2 beam line)" << std::flush;
  }
  if (useMaterialMap) {
    std::cout << " using material map (slower, more precise)" << std::flush;
  }
  std::cout << std::endl;
  if (successful) {
    std::cout << "    x = " << x << ", y = " << y << ", dxdz = " << dxdz << ", dydz = " << dydz
              << std::endl;
  } else {
    std::cout << "    Extrapolation was not successful" << std::endl;
  }
  std::cout << std::endl;
}

ClassImp(TrackExtrapolationResult);