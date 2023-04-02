#include "Enemies.h"

#pragma region shyGuy
//Modelo
void shyGuy::setAnimation() {
	float scaleDefault = 0.2f;
	this->Animation.push_back(Model("shyGuy", "models/animModels/shyGuyWalk/shyGuyWalk01.obj", glm::vec3(7, -0.1, -4.0), glm::vec3(0, 0, 0), 0.0f, scaleDefault));
	this->Animation.push_back(Model("shyGuyWalk01", "models/animModels/shyGuyWalk/shyGuyWalk02.obj", glm::vec3(7, -0.1, -4.0), glm::vec3(0, 0, 0), 0.0f, scaleDefault));
	this->Animation.push_back(Model("shyGuyWalk02", "models/animModels/shyGuyWalk/shyGuyWalk03.obj", glm::vec3(7, -0.1, -4.0), glm::vec3(0, 0, 0), 0.0f, scaleDefault));
	this->Animation.push_back(Model("shyGuyWalk03", "models/animModels/shyGuyWalk/shyGuyWalk04.obj", glm::vec3(7, -0.1, -4.0), glm::vec3(0, 0, 0), 0.0f, scaleDefault));
	this->Animation.push_back(Model("shyGuyWalk04", "models/animModels/shyGuyWalk/shyGuyWalk05.obj", glm::vec3(7, -0.1, -4.0), glm::vec3(0, 0, 0), 0.0f, scaleDefault));
	this->Animation.push_back(Model("shyGuyWalk05", "models/animModels/shyGuyWalk/shyGuyWalk06.obj", glm::vec3(7, -0.1, -4.0), glm::vec3(0, 0, 0), 0.0f, scaleDefault));
	this->Animation.push_back(Model("shyGuyWalk06", "models/animModels/shyGuyWalk/shyGuyWalk07.obj", glm::vec3(7, -0.1, -4.0), glm::vec3(0, 0, 0), 0.0f, scaleDefault));
	this->Animation.push_back(Model("shyGuyWalk07", "models/animModels/shyGuyWalk/shyGuyWalk08.obj", glm::vec3(7, -0.1, -4.0), glm::vec3(0, 0, 0), 0.0f, scaleDefault));
	this->Animation.push_back(Model("shyGuyWalk08", "models/animModels/shyGuyWalk/shyGuyWalk09.obj", glm::vec3(7, -0.1, -4.0), glm::vec3(0, 0, 0), 0.0f, scaleDefault));
	this->Animation.push_back(Model("shyGuyWalk09", "models/animModels/shyGuyWalk/shyGuyWalk10.obj", glm::vec3(7, -0.1, -4.0), glm::vec3(0, 0, 0), 0.0f, scaleDefault));
	this->Animation.push_back(Model("shyGuyWalk10", "models/animModels/shyGuyWalk/shyGuyWalk11.obj", glm::vec3(7, -0.1, -4.0), glm::vec3(0, 0, 0), 0.0f, scaleDefault));
	this->Animation.push_back(Model("shyGuyWalk11", "models/animModels/shyGuyWalk/shyGuyWalk12.obj", glm::vec3(7, -0.1, -4.0), glm::vec3(0, 0, 0), 0.0f, scaleDefault));
	this->Animation.push_back(Model("shyGuyWalk12", "models/animModels/shyGuyWalk/shyGuyWalk13.obj", glm::vec3(7, -0.1, -4.0), glm::vec3(0, 0, 0), 0.0f, scaleDefault));
	this->Animation.push_back(Model("shyGuyWalk13", "models/animModels/shyGuyWalk/shyGuyWalk14.obj", glm::vec3(7, -0.1, -4.0), glm::vec3(0, 0, 0), 0.0f, scaleDefault));
	this->Animation.push_back(Model("shyGuyWalk14", "models/animModels/shyGuyWalk/shyGuyWalk15.obj", glm::vec3(7, -0.1, -4.0), glm::vec3(0, 0, 0), 0.0f, scaleDefault));
	
	this->cb = CollisionBox(glm::vec3(7, -0.1, -4.0), glm::vec3(0.6), glm::vec4(0.0, 1.0, 0.0, 1.0));
	this->name = "shyGuy";

}

//Animacion
void shyGuy::Walk(Shader enShader) {

	if (!this->isMoving or this->itArrived or this->isAttacking) {
		this->Animation[0].Draw(enShader);
		return;
	}

	//Dibujar el modelo
	this->Animation[this->frame].Draw(enShader);

	//Avanzar en la animacion
	if (this->iAnimation > this->iTimer) { // TIMER
		this->iAnimation = 0;
		this->frame++;
	}
	else
		this->iAnimation++;

	if (this->frame >= this->Animation.size()) // CANTIDAD DE ELEMENTOS EN EL ARREGLO
		this->frame = 0;
}

#pragma endregion


#pragma region billBalla

void billBalla::setInitModel() {
	float scaleDefault = 0.2f;
	this->Animation.push_back(Model("billBalla", "models/marioResources/Bullet.obj", glm::vec3(7, -0.1, -3.0), glm::vec3(0, 90, 0), 0.0f, scaleDefault));
	this->cb = CollisionBox(glm::vec3(7, -0.1, -3.0), glm::vec3(0.6), glm::vec4(0.0, 1.0, 0.0, 1.0));
	this->name = "billBalla";

}

void billBalla::Draw(Shader enShader) {
	this->Animation[0].Draw(enShader);
}


#pragma endregion

#pragma region Blooper

void Blooper::setInitModel() {
	float scaleDefault = 0.2f;
	this->Animation.push_back(Model("Blooper", "models/marioResources/Calamar.obj", glm::vec3(7, -0.1, -2.0), glm::vec3(0, 90, 0), 0.0f, scaleDefault));
	this->cb = CollisionBox(glm::vec3(7, -0.1, -2.0), glm::vec3(0.5), glm::vec4(0.0, 1.0, 0.0, 1.0));
	this->name = "Blooper";
}

void Blooper::Draw(Shader enShader) {
	this->Animation[0].Draw(enShader);
}


#pragma endregion