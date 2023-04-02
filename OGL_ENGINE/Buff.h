#pragma once
#include "Animation.h"
#include "buffType.h"

enum posDir {
	UP, DOWN
};

class Buff : public Animacion {
protected:
	int iType;
public:
	Model model;
	CollisionBox cb;
	string name;

	bool isPicked;
	bool bDraw;

	virtual void setInitModel() = 0; // Modelo standard
	virtual void setAnimation() = 0;
	virtual int getBuff(int) = 0;

	int getType() {
		return this->iType;
	}

	void setDirection(int direction, int value) {
		glm::vec3 posModel;

		for (int i = 0; i < this->Animation.size(); i++)
			posModel = this->Animation[i].getPosition();

		if (direction == UP) {
			posModel.x = 10.6;
			posModel.z = (float)value;
		}
		if (direction == DOWN) {
			posModel.x = 2.81;
			posModel.z = (float)value;
		}

		this->cb.setPosition(posModel);
		this->cb.defineCollisionBox();

		for (int i = 0; i < this->Animation.size(); i++) {
			this->Animation[i].setPosition(posModel);
		}
	}
};

class Star : public Buff {
public:
	Star() {
		// Buff
		this->iType = ATK;
		
		// Animacion
		this->frame = 0;
		this->iAnimation = 0;
		this->minCount = 0;
		this->maxCount = 0;
		this->bReverse = false;
		this->iTimer = 20;

		this->isPicked = false;
		this->bDraw = false;
	}
	~Star() {
		this->model.Release();
		this->cb.Release();
		for (int i = 0; i < this->Animation.size(); i++)
			this->Animation[i].Release();
	}

	void setAnimation() {}
	void Levitate(Shader);
	void setInitModel();
	int getBuff(int);
};

class Heart : public Buff {
public:
	Heart() {
		// Buff
		this->iType = HP;

		// Animacion
		this->frame = 0;
		this->iAnimation = 0;
		this->minCount = 0;
		this->maxCount = 0;
		this->bReverse = false;
		this->iTimer = 15;

		this->isPicked = false;
		this->bDraw = false;
	}
	~Heart() {
		this->model.Release();
		this->cb.Release();
		for (int i = 0; i < this->Animation.size(); i++)
			this->Animation[i].Release();
	}

	void setAnimation() {}
	void Beat(Shader);
	void setInitModel();
	int getBuff(int);
};


class Coin : public Buff {
public:
	Coin() {
		// Buff
		this->iType = SCORE;

		// Animacion
		this->frame = 0;
		this->iAnimation = 0;
		this->minCount = 0;
		this->maxCount = 0;
		this->bReverse = false;
		this->iTimer = 1;

		this->isPicked = false;
		this->bDraw = false;
	}
	~Coin() {
		this->model.Release();
		this->cb.Release();
		for (int i = 0; i < this->Animation.size(); i++)
			this->Animation[i].Release();
	}

	void setAnimation() {}
	void Spin(Shader);
	void setInitModel();
	int getBuff(int);
};