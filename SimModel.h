#ifndef SIM_MODEL_H
#define SIM_MODEL_H

class SimModel {
public:
	SimModel(float k, float m, float c, float amp_eq);

	void update(float dt);
	void reset(float start_amp);
	void resetX(float start_x);
	void resetZ(float start_z);

	float getAcceleration() const { return acc; }
	float getVelocity() const { return vel; }
	float getAmplitude() const { return amp; }
	float getEquilibrium() const { return equi; }
	float getDisplacement() const { return equi - amp; }
	float getSpringConstant() const;
	float getDamping() const { return damp; }
	float getMass() const { return m; }

	float getX() const { return x; }
	float getVelX() const { return velX; }
	float getAccX() const { return accX; }

	float getZ() const { return z; }
	float getVelZ() const { return velZ; }
	float getAccZ() const { return accZ; }

	void setDamping(float c) { damp = c; }
	void setMass(float new_m);
	float getDamping() { return damp; }
	void setSpringConstant(float new_k);

	void setSpringConstantX(float new_kx) { kx = new_kx; }
	float getSpringConstantX() const { return kx; }

	void setSpringConstantZ(float new_kz) { kz = new_kz; }
	float getSpringConstantZ() const { return kz; }

private:
	float acc, vel, amp, equi;

	float damp, k, m;

	float accX, velX, x, equiX, kx;

	float accZ, velZ, z, equiZ, kz;
};

#endif
