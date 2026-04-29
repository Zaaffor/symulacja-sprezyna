#ifndef SIM_MODEL_H
#define SIM_MODEL_H

class SimModel {
public:
	SimModel(float k, float m, float c, float amp_eq);

	void update(float dt);
	void reset(float start_amp);

	float getAcceleration() const { return acc; }
	float getVelocity() const { return vel; }
	float getAmplitude() const { return amp; }
	float getEquilibrium() const { return equi; }
	float getDisplacement() const { return equi - amp; }

	void setDamping(float c) { damp = c; }
	void setMass(float new_m);
	float getDamping() { return damp; }

private:
	float acc;
	float vel;
	float amp;
	float equi;
	float damp;
	float k;
	float m;
};

#endif
