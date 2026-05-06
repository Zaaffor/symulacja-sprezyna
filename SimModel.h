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
	float getSpringConstant() const;
	float getDamping() const { return damp; }
	float getMass() const { return m; }

	void setDamping(float c) { damp = c; }
	void setMass(float new_m);
	float getDamping() { return damp; }
	void setSpringConstant(float new_k);

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
