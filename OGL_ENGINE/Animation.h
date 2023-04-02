#pragma once
#define _Personaje
#include <engine/model.h>
//#include "animValues.h"


class Animacion {

public:

	vector<Model> Animation;

	int frame;
	int iAnimation;
	int iMovement;
	int iTimer;
	int minCount;
	int maxCount;

	bool bReverse;

	// Set - get
	void setTimer(int timer) {
		this->iTimer = timer;
	}
	int getFrame() {
		return this->frame;
	}


};

