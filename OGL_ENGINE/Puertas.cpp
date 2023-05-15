#include "Puertas.h"
#define MOV 0.01
#define COUNT 150
#define MAX 10

bool rLimitMin, lLimitMin;
bool rLimitMax, lLimitMax;


#pragma region Door
void Door::setInitModel() {
	float scaleDefault = 0.2f;
	Animation.push_back(Model("Door", "models/mcDonalds/Door.obj", glm::vec3(13.08, 0.126, 4.23), glm::vec3(0, -90, 0), 0.0f, scaleDefault));
	this->cb = CollisionBox(glm::vec3(12.64, -0.1, 2.68), glm::vec3(0.5), glm::vec4(0.0, 1.0, 0.0, 1.0));
	this->name = "Puerta";

	glm::vec3 scaleCollBox = this->cb.getScale();
	scaleCollBox.x += 0.2;
	this->cb.setScale(scaleCollBox);
	this->cb.defineCollisionBox();
}

void Door::Slide(Shader enShader) {

	this->Animation[this->frame].Draw(enShader);
	glm::vec3 pModel;
	float mov = MOV;
	pModel = this->Animation[this->frame].getPosition();
	bool continueAnimation;


	//Limites
	
	if (pModel.z >= 2.55 && pModel.z >= 4.22 && this->collDetected) { // Cerrado
		// Accion para abrirse triggereada
		pModel.z = 4.21;
		this->Animation[this->frame].setPosition(pModel);
		this->isClosing = false;
		this->isOpening = true;
	}

	if (pModel.z >= 2.55 && pModel.z >= 4.22) {
		this->isClosing = false;
	}

	if (pModel.z <= 2.55 && pModel.z <= 4.22) { // Abierto
		// Accion para cerrarse triggereada
		pModel.z = 2.56;
		this->Animation[this->frame].setPosition(pModel);
		this->isClosing = true;
		this->isOpening = false;
	}

	if (this->iAnimation > this->iTimer) { // TIMER

		this->iAnimation = 0;

		if (isOpening) {
			pModel = this->Animation[this->frame].getPosition();
			pModel.z -= mov;
			this->Animation[this->frame].setPosition(pModel);
			this->cb.setPosition(pModel);
			this->cb.defineCollisionBox();
			
		}
		else if (isClosing) {
			if (this->collDetected)
				return;
			// Cerrar
			pModel = this->Animation[frame].getPosition();
			pModel.z += mov;
			this->Animation[this->frame].setPosition(pModel);
			this->cb.setPosition(pModel);
			this->cb.defineCollisionBox();
			
		}

	}
	else
		this->iAnimation++;
}



#pragma endregion

