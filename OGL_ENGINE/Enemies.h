#pragma once
#include "Base.h"

enum atkDir {
	IZQ, DER
};

class Enemy : public Base {

public: 
	bool isMoving;
	bool itArrived;
	bool isAttacking;
	bool enemCollPlayer;
	int atkDirection;
	float moveSpeed;
	int reverseDir;
	int iCooldown;
	bool bDraw;
	bool isDefeated;

	//void set
	int attackObjective() {
		//if (!this->itArrived and !this->isAttacking)
		//	return 0;

		if (this->iCooldown > 250) { // TIMER
			this->isAttacking = true;
			this->iCooldown = 0;
			return this->getAtk();
		}
		else {
			this->iCooldown++;
			return 0;
		}

	}

	void moveForward() {
		if (!this->isMoving or this->itArrived)
			return;

		glm::vec3 posModel;
		if (this->atkDirection == IZQ) {
			// Z ++
			for (int i = 0; i < this->Animation.size(); i++) {
				posModel = this->Animation[i].getPosition();
				posModel.z += this->moveSpeed;
				this->Animation[i].setPosition(posModel);
			}
			this->cb.setPosition(posModel);
			this->cb.defineCollisionBox();
		}

		if (this->atkDirection == DER) {
			// Z--
			for (int i = 0; i < this->Animation.size(); i++) {
				posModel = this->Animation[i].getPosition();
				posModel.z -= this->moveSpeed;
				this->Animation[i].setPosition(posModel);
			
			}
			this->cb.setPosition(posModel);
			this->cb.defineCollisionBox();
		}
	}

	void setDirection(int direction) {
		this->atkDirection = direction;
		glm::vec3 posModel;
		glm::vec3 angModel;

		for (int i = 0; i < this->Animation.size(); i++)
			angModel = this->Animation[i].getAngles();

		if (direction == IZQ) 
			posModel = glm::vec3(7, -0.1, -26.5);
		if (direction == DER) {
			posModel = glm::vec3(7, -0.1, 26.5);
			angModel.y = this->reverseDir;
		}

		this->cb.setPosition(posModel);
		this->cb.defineCollisionBox();
		this->cb.setAngles(angModel);

		for (int i = 0; i < this->Animation.size(); i++) {
			this->Animation[i].setPosition(posModel);
			this->Animation[i].setAngles(angModel);
		}
	}

};

class shyGuy : public Enemy {
public:
	// Constructor - destructor
	shyGuy() {
		// Juego
		this->setAtk(25);
		this->setHP(100);
		this->itArrived = false;
		this->isMoving = true;
		this->isAttacking = false;
		this->iCooldown = 0;
		this->enemCollPlayer = false;

		// Animacion
		this->frame = 0;
		this->iAnimation = 0;
		this->minCount = 0;
		this->maxCount = 0;
		this->bReverse = false;
		this->iTimer = 10;
		this->reverseDir = 180;
		this->isDefeated = false;
		this->bDraw = false;
		
		// Velocidad
		this->moveSpeed = 0.007f; // velocidad del Enemigo

	}
	~shyGuy() {
		this->cb.Release();
		for (int i = 0; i < this->Animation.size(); i++)
			this->Animation[i].Release();
	}

	// Animaciones
	void setInitModel() {}
	void setAnimation();
	void Walk(Shader enShader);
};


class billBalla : public Enemy {
public:
	// Constructor - destructor
	billBalla() {
		// Juego
		this->setAtk(30);
		this->setHP(80);
		this->itArrived = false;
		this->isMoving = true;
		this->isAttacking = false;
		this->iCooldown = 0;
		this->enemCollPlayer = false;

		// Animacion
		this->frame = 0;
		this->iAnimation = 0;
		this->minCount = 0;
		this->maxCount = 0;
		this->bReverse = false;
		this->iTimer = 10;
		this->reverseDir = -90;

		this->isDefeated = false;
		this->bDraw = false;

		// Velocidad
		this->moveSpeed = 0.005f; // (timer)

	}
	~billBalla() {
		this->cb.Release();
		for (int i = 0; i < this->Animation.size(); i++)
			this->Animation[i].Release();
	}

	// Animaciones
	void setInitModel();
	void setAnimation() { }
	
	void Draw(Shader enShader);
};


class Blooper : public Enemy {
public:
	// Constructor - destructor
	Blooper() {
		// Juego
		this->setAtk(25);
		this->setHP(100);
		this->itArrived = false;
		this->isMoving = true;
		this->isAttacking = false;
		this->iCooldown = 0;
		this->enemCollPlayer = false;

		// Animacion
		this->frame = 0;
		this->iAnimation = 0;
		this->minCount = 0;
		this->maxCount = 0;
		this->bReverse = false;
		this->iTimer = 10;
		this->reverseDir = -90;

		this->isDefeated = false;
		this->bDraw = false;

		// Velocidad
		this->moveSpeed = 0.005f; // (timer)

	}
	~Blooper() {
		this->cb.Release();
		for (int i = 0; i < this->Animation.size(); i++)
			this->Animation[i].Release();
	}

	// Animaciones
	void setInitModel();
	void setAnimation() { }

	void Draw(Shader enShader);
};