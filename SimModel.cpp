#include "SimModel.h"

SimModel::SimModel(float k, float m, float c, float amp_eq) : 
	k(k), m(m), damp(c), equi(amp_eq), amp(amp_eq), vel(0.0f), acc(0.0f) { }

void SimModel::update(float dt) {
	float displacement = amp - equi;
	acc = (-k * displacement - damp * vel) / m;
	vel += acc * dt;
	amp += vel * dt;
}

void SimModel::reset(float start_amp) {
	amp = start_amp;
	vel = 0.0f;
	acc = 0.0f;
}

void SimModel::setMass(float new_m) {
	m = new_m;
}

void SimModel::setSpringConstant(float new_k) {
	k = new_k;
}

float SimModel::getSpringConstant() const {
	return k;
}