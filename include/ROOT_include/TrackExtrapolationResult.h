
#pragma once
#include "TObject.h"
#include <iostream>
#include <string>

struct TrackExtrapolationResult : public TObject {
	TrackExtrapolationResult();
	ClassDef(TrackExtrapolationResult, 1);
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
};