#include "Buff.h"


#pragma region Star
// Modelo
void Star::setInitModel() {
	float scaleDefault = 0.2f;
	Animation.push_back(Model("Star", "models/marioResources/Star.obj", glm::vec3(7, 0.1, 6.0), glm::vec3(0, 0, 0), 0.0f, scaleDefault));
	this->cb = CollisionBox(glm::vec3(7, 0.1, 6.0), glm::vec3(0.5), glm::vec4(0.0, 1.0, 0.0, 1.0));
	this->name = "Star";
}

// Animacion
void Star::Levitate(Shader enShader) {
	this->Animation[this->frame].Draw(enShader);
	glm::vec3 pModel;
	float mov = 0.01;
	if (this->maxCount > 4) {
		this->bReverse = true;
		this->maxCount = 0;
	}

	if (this->minCount > 4) {
		this->bReverse = false;
		this->minCount = 0;
	}

	if (this->iAnimation > this->iTimer) { // TIMER

		this->iAnimation = 0;

		if (!this->bReverse) {
			pModel = this->Animation[this->frame].getPosition();
			pModel.y += mov;
			this->Animation[this->frame].setPosition(pModel);
			this->maxCount++;
		}
		else {
			pModel = this->Animation[frame].getPosition();
			pModel.y -= mov;
			this->Animation[this->frame].setPosition(pModel);
			this->minCount++;
		}

		this->cb.setPosition(pModel);
		this->cb.defineCollisionBox();
	}
	else
		this->iAnimation++;

}

// Buff
int Star::getBuff(int ATK) {
	return ATK + 25;
}

#pragma endregion

#pragma region Heart
// Modelo
void Heart::setInitModel() {
	float scaleDefault = 0.2f;
	Animation.push_back(Model("Heart", "models/marioResources/Heart.obj", glm::vec3(7, 0.1, 7.0), glm::vec3(0, -90, 0), 0.0f, scaleDefault));
	this->cb = CollisionBox(glm::vec3(7, 0.1, 7.0), glm::vec3(0.5), glm::vec4(0.0, 1.0, 0.0, 1.0));
	this->name = "Heart";
}

// Animacion
void Heart::Beat(Shader enShader) {
	this->Animation[this->frame].Draw(enShader);
	glm::vec3 scaleModel;

	if (this->maxCount > 2) {
		this->bReverse = true;
		this->maxCount = 0;
	}

	if (this->minCount > 2) {
		this->bReverse = false;
		this->minCount = 0;
	}

	if (this->iAnimation > this->iTimer) { // TIMER

		this->iAnimation = 0;

		if (!this->bReverse) {
			scaleModel = this->Animation[this->frame].getScale();
			scaleModel.x += 0.02;
			scaleModel.y += 0.02;
			scaleModel.z += 0.02;
			this->Animation[this->frame].setScale(scaleModel);
			this->maxCount++;
		}
		else {
			scaleModel = this->Animation[this->frame].getScale();
			scaleModel.x -= 0.02;
			scaleModel.y -= 0.02;
			scaleModel.z -= 0.02;
			this->Animation[this->frame].setScale(scaleModel);
			this->minCount++;
		}
	}
	else
		this->iAnimation++;

}

// Buff
int Heart::getBuff(int HP) {
	return HP + 50;
}

#pragma endregion


#pragma region Coin
// Modelo
void Coin::setInitModel() {
	float scaleDefault = 0.2f;
	Animation.push_back(Model("Coin", "models/marioResources/Coin.obj", glm::vec3(7, 0.1, 5.0), glm::vec3(0, 45, 0), 0.0f, scaleDefault));
	this->cb = CollisionBox(glm::vec3(7, 0.1, 5.0), glm::vec3(0.5), glm::vec4(0.0, 1.0, 0.0, 1.0));
	this->name = "Coin";
}

// Animacion
void Coin::Spin(Shader enShader) {
	
	this->Animation[this->frame].Draw(enShader);
	glm::vec3 rotModel;

	if (this->iAnimation > 1) { // TIMER
		this->iAnimation = 0;

		rotModel = this->Animation[this->frame].getAngles();
		rotModel.y += 2.0;
		this->Animation[this->frame].setAngles(rotModel);
	}
	else
		this->iAnimation++;

}

// Buff
int Coin::getBuff(int SCORE) {
	return SCORE + 1;
}

#pragma endregion