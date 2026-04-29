#include "SimData.h"

void SimData::record(float time, float displacement, float acceleration) {
	t.push_back(time);
	disp.push_back(displacement);
	acc.push_back(acceleration);
}

void SimData::clear() {
	t.clear();
	disp.clear();
	acc.clear();
}