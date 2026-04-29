#ifndef SIM_DATA_H
#define SIM_DATA_H

#include <vector>

class SimData {
public:
	void record(float time, float displacement, float acceleration);
	void clear();

	const std::vector<float>& getTime() const { return t; }
	const std::vector<float>& getDisplacement() const { return disp; }
	const std::vector<float>& getAcceleration() const { return acc; }

private:
	std::vector<float> t;
	std::vector<float> disp;
	std::vector<float> acc;
};

#endif 