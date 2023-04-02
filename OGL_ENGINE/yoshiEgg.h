#pragma once
#include <engine/model.h>

class yoshiEgg {
private: 
	int iHP;
public:
	Model model;

	yoshiEgg() {
		this->setHp(200);
	}

	~yoshiEgg() {
		this->model.Release();
	}

	void setInitModel() {
		this->model = Model("yoshiEgg", "models/Scene/yoshiEgg.obj", glm::vec3(7, 0.3, 2.9), glm::vec3(0, 0, 0), 0.0f, 0.2f);
	}
	
	void setHp(int HP) {
		this->iHP = HP;
	}

	int getHp() {
		return this->iHP;
	}

	void getDMG(int Atk) {
		this->iHP -= Atk;
	}
};