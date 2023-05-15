#pragma once
#include "Animation.h"
#define TIMER 2

class Puerta : public Animacion {
protected:
	int iType;
public:
	Model model;
	CollisionBox cb;
	string name;
	bool isOpen;
	bool isOpening;
	bool isClosing;

	bool collDetected;

	virtual void setInitModel() = 0; // Modelo standard
	virtual void setAnimation() = 0;
};

class Door : public Puerta {
public:

	Door() {
		this->isOpen = false;
		this->isOpening = false;
		this->isClosing = false;
		this->collDetected = false;

		// Animacion
		this->frame = 0;
		this->iAnimation = 0;
		this->minCount = 0;
		this->maxCount = 0;
		this->bReverse = false;
		this->iTimer = TIMER;
	}
	~Door() {
		this->model.Release();
		this->cb.Release();
		for (int i = 0; i < this->Animation.size(); i++)
			this->Animation[i].Release();
	}

	void setAnimation() {}
	void Slide(Shader);
	void setInitModel();
	
};

//class puertaR : public Puerta {
//public:
//
//	puertaR() {
//		// Animacion
//		this->frame = 0;
//		this->iAnimation = 0;
//		this->minCount = 0;
//		this->maxCount = 0;
//		this->bReverse = false;
//		this->iTimer = TIMER;
//	}
//	~puertaR() {
//		this->model.Release();
//		this->cb.Release();
//		for (int i = 0; i < this->Animation.size(); i++)
//			this->Animation[i].Release();
//	}
//
//	void setAnimation() {}
//	void Slide(Shader);
//	void setInitModel();
//};
//
//class puertaL : public Puerta {
//public:
//
//	puertaL() {
//		// Animacion
//		this->frame = 0;
//		this->iAnimation = 0;
//		this->minCount = 0;
//		this->maxCount = 0;
//		this->bReverse = false;
//		this->iTimer = TIMER;
//	}
//	~puertaL() {
//		this->model.Release();
//		this->cb.Release();
//		for (int i = 0; i < this->Animation.size(); i++)
//			this->Animation[i].Release();
//	}
//
//	void setAnimation() {}
//	void Slide(Shader);
//	void setInitModel();
//};