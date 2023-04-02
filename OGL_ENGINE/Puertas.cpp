#include "Puertas.h"
#define MOV 0.01
#define COUNT 150
#define MAX 10

bool rLimitMin, lLimitMin;
bool rLimitMax, lLimitMax;

#pragma region PuertaL

void puertaL::setInitModel() {
	float scaleDefault = 0.2f;
	Animation.push_back(Model("PuertaL", "models/Puertas/puertaL.obj", glm::vec3(12.64, 0.0, 2.68), glm::vec3(0, -90, 0), 0.0f, scaleDefault));
	this->cb = CollisionBox(glm::vec3(12.64, -0.1, 2.68), glm::vec3(0.5), glm::vec4(0.0, 1.0, 0.0, 1.0));
	this->name = "PuertaL";
}

void puertaL::Slide(Shader enShader) {
	this->Animation[this->frame].Draw(enShader);
	glm::vec3 pModel;
	float mov = MOV;
	pModel = this->Animation[this->frame].getPosition();

	if (pModel.z >= 1.65 && pModel.z >= 2.68) { // IZQ LIMITE ->
		pModel.z = 2.67;
		this->Animation[this->frame].setPosition(pModel);
		this->bReverse = false;
		this->maxCount = 0;
		return;
	}
	if (pModel.z <= 1.65 && pModel.z <= 2.68) {
		pModel.z = 1.66;
		this->Animation[this->frame].setPosition(pModel);
		this->bReverse = true;
		this->minCount = 0;
		return;
	}

	if (this->iAnimation > this->iTimer) { // TIMER

		this->iAnimation = 0;

		if (!this->bReverse) {
			pModel = this->Animation[this->frame].getPosition();
			pModel.z -= mov;
			this->Animation[this->frame].setPosition(pModel);
			this->maxCount++;
		}
		else {
			pModel = this->Animation[frame].getPosition();
			pModel.z += mov;
			this->Animation[this->frame].setPosition(pModel);
			this->minCount++;
		}

		this->cb.setPosition(pModel);
		this->cb.defineCollisionBox();
	}
	else
		this->iAnimation++;
}


#pragma endregion

#pragma region PuertaR

void puertaR::setInitModel() {
	float scaleDefault = 0.2f;
	Animation.push_back(Model("PuertaR", "models/Puertas/puertaR.obj", glm::vec3(12.64, 0.0, 2.68), glm::vec3(0, -90, 0), 0.0f, scaleDefault));
	this->cb = CollisionBox(glm::vec3(12.64, -0.1, 2.68), glm::vec3(0.5), glm::vec4(0.0, 1.0, 0.0, 1.0));
	this->name = "PuertaR";
}

void puertaR::Slide(Shader enShader) {
	this->Animation[this->frame].Draw(enShader);
	glm::vec3 pModel;
	float mov = MOV;
	pModel = this->Animation[this->frame].getPosition();
	if (pModel.z >= 2.68 && pModel.z >= 3.71) { // DER LIMITE <-
		pModel.z = 3.70;
		this->Animation[this->frame].setPosition(pModel);
		this->bReverse = true;
		this->maxCount = 0;
		return;
	}
	if (pModel.z <= 2.68 && pModel.z <= 3.71) {
		pModel.z = 2.69;
		this->Animation[this->frame].setPosition(pModel);
		this->bReverse = false;
		this->minCount = 0;
		return;
	}

	if (this->iAnimation > this->iTimer) { // TIMER

		this->iAnimation = 0;

		if (!this->bReverse) {
			pModel = this->Animation[this->frame].getPosition();
			pModel.z += mov;
			this->Animation[this->frame].setPosition(pModel);
			this->maxCount++;
		}
		else {
			pModel = this->Animation[frame].getPosition();
			pModel.z -= mov;
			this->Animation[this->frame].setPosition(pModel);
			this->minCount++;
		}

		this->cb.setPosition(pModel);
		this->cb.defineCollisionBox();
	}
	else
		this->iAnimation++;
}


#pragma endregion