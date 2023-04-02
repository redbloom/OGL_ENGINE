#pragma once
#include "Animation.h"


// Base para cada PERSONAJE

class Base : public Animacion {
protected:
	int iHP;
	int iAtk;

public:
	string name;
	Model model;
	CollisionBox cb;
	virtual void setInitModel() = 0; // Modelo standard
	virtual void setAnimation() = 0;

	// Attack / Receive
	void getDMG(int Atk) {
		this->iHP -= Atk;
	}

	int getHP() {
		return this->iHP;
	}
	int getAtk() {
		return this->iAtk;
	}

	void setHP(int HP) {
		this->iHP = HP;
	}
	void setAtk(int ATK) {
		this->iAtk = ATK;
	}


};

