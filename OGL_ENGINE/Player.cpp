#include "Player.h"
#include <fstream>

fstream file;


// Cargar modelos
void Player::setInitModel() {
	this->model = Model("Yoshi", "models/Yoshi_Standard/Yoshi_Standard.obj", glm::vec3(3, -0.1, 2.5), glm::vec3(0, 90, 0), 0.0f, 0.2f);
	this->cb = CollisionBox(glm::vec3(3, -0.1, 2.5), glm::vec3(0.25), glm::vec4(0.0, 1.0, 0.0, 1.0));
	this->name = "Yoshi";

}

void Player::setAnimation() {
	glm::vec3 posModel = this->model.getPosition();
	glm::vec3 angModel = this->model.getAngles();
	angModel.y += 45;

	this->Animation.push_back(Model("YoshiPunch00", "models/animModels/YoshiPunch/YoshiPunch00.obj", posModel, angModel, 0.0f, 0.2f));
	this->Animation.push_back(Model("YoshiPunch01", "models/animModels/YoshiPunch/YoshiPunch01.obj", posModel, angModel, 0.0f, 0.2f));
	this->Animation.push_back(Model("YoshiPunch02", "models/animModels/YoshiPunch/YoshiPunch02.obj", posModel, angModel, 0.0f, 0.2f));
	this->Animation.push_back(Model("YoshiPunch03", "models/animModels/YoshiPunch/YoshiPunch03.obj", posModel, angModel, 0.0f, 0.2f));
	this->Animation.push_back(Model("YoshiPunch04", "models/animModels/YoshiPunch/YoshiPunch04.obj", posModel, angModel, 0.0f, 0.2f));
	this->Animation.push_back(Model("YoshiPunch05", "models/animModels/YoshiPunch/YoshiPunch05.obj", posModel, angModel, 0.0f, 0.2f));
	this->Animation.push_back(Model("YoshiPunch06", "models/animModels/YoshiPunch/YoshiPunch06.obj", posModel, angModel, 0.0f, 0.2f));
	this->Animation.push_back(Model("YoshiPunch07", "models/animModels/YoshiPunch/YoshiPunch07.obj", posModel, angModel, 0.0f, 0.2f));
	this->Animation.push_back(Model("YoshiPunch08", "models/animModels/YoshiPunch/YoshiPunch08.obj", posModel, angModel, 0.0f, 0.2f));
	this->Animation.push_back(Model("YoshiPunch09", "models/animModels/YoshiPunch/YoshiPunch09.obj", posModel, angModel, 0.0f, 0.2f));
	this->Animation.push_back(Model("YoshiPunch10", "models/animModels/YoshiPunch/YoshiPunch10.obj", posModel, angModel, 0.0f, 0.2f));
	this->Animation.push_back(Model("YoshiPunch11", "models/animModels/YoshiPunch/YoshiPunch11.obj", posModel, angModel, 0.0f, 0.2f));
	this->Animation.push_back(Model("YoshiPunch12", "models/animModels/YoshiPunch/YoshiPunch12.obj", posModel, angModel, 0.0f, 0.2f));
	this->Animation.push_back(Model("YoshiPunch13", "models/animModels/YoshiPunch/YoshiPunch13.obj", posModel, angModel, 0.0f, 0.2f));
	this->Animation.push_back(Model("YoshiPunch14", "models/animModels/YoshiPunch/YoshiPunch14.obj", posModel, angModel, 0.0f, 0.2f));
}

// Posicionar modelos
void Player::updatePosition(glm::vec3 position) {
	//Modelo standard
	this->model.setPosition(position);
	this->cb.setPosition(position);
	this->cb.defineCollisionBox();
	//Animado
	for (int i = 0; i < this->Animation.size(); i++) // Esto para cambiar la posicion de cada uno de los elementos en el arreglo.
		this->Animation[i].setPosition(position);
}

// Cambiar angulos
void Player::updateAngles(glm::vec3 angles) {
	//Modelo standard
	this->model.setAngles(angles);
	angles.y += 45;
	//Animado
	for (int i = 0; i < this->Animation.size(); i++) // Esto para cambiar la posicion de cada uno de los elementos en el arreglo.
		this->Animation[i].setAngles(angles);
}

// Animacion
void Player::Punch(Shader enShader) {
	if (!this->isPunching)
		return;

	//Dibujar el modelo
	this->Animation[frame].Draw(enShader);

	//Avanzar en la animacion
	if (this->iAnimation > 10) { // TIMER
		this->iAnimation = 0;
		this->frame++;
	}
	else
		this->iAnimation++;

	if (this->frame >= this->Animation.size()) { // CANTIDAD DE ELEMENTOS EN EL ARREGLO
		this->frame = 0;
		this->isPunching = false;
	}
}

// TODO: Cargar / Guardar el maximo Score (O sea ponerlo en el codigo)

// ::::::::::: SCORE :::::::::::

void Player::setScore(int score) {
	this->iScore = score;
}

int Player::getScore() {
	return this->iScore;
}
int Player::getMaxScore() {
	return this->maxScore;
}


// ::::::::::: POWER UPS :::::::::::

void Player::setBuffs(int buffID, int powerUp) {
	switch (buffID) {
	case HP: {
		setHP(powerUp);
	} break;
	case ATK: {
		setAtk(powerUp);
	} break;
	case SCORE: {
		setScore(powerUp);
	} break;
	}
}

int Player::attackEnemy() {

	if (this->iCooldown > 150) { // TIMER
		this->iCooldown = 0;
		return this->getAtk();
	}
	else {
		this->iCooldown++;
		return 0;
	}
}

// ::::::::::: CONSTRUCTOR Y DESTRUCTOR :::::::::::

Player::Player() {

	this->setAtk(25);
	this->setHP(150);
	this->iScore = 0;
	this->maxScore = 0;
	this->iCooldown = 0;
	this->isPunching = false;
}

Player::~Player() {
	// Deletear la animacion
	for (int i = 0; i < this->Animation.size(); i++)
		this->Animation[i].Release();

	// Deletear el modelo inicial
	this->model.Release();
	this->cb.Release();

}

