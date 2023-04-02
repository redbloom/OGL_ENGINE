#pragma once
#include "Base.h"
#include "buffType.h"

class Player : public Base {

private:
	int iScore;
	int maxScore;

protected:
	int iCooldown;

public:

	bool isPunching;

	// Constructor - destructor
	Player();
	~Player();


	// Animaciones
	void setInitModel();
	void setAnimation();

	// Posiciones
	void updatePosition(glm::vec3);
	void updateAngles(glm::vec3);

	void Punch(Shader enShader);
	int attackEnemy();

	// Power-Ups
	void setBuffs(int, int);

	// Set / get
	int getMaxScore();
	void setScore(int);
	int getScore();

};