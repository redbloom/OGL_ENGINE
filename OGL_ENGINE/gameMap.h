#pragma once
#include <random>
#include "Player.h"
#include "Buff.h"
#include "Enemies.h"
#include "yoshiEgg.h"
#define AMOUNT_ENEMIES 2
#define AMOUNT_BUFFS 2

// Player - Egg
//Player Yoshi;
yoshiEgg Egg;
void hpCheck();

// Variables
bool gameStarted = false;
bool gameOver = false;
bool gameFinished = false;
bool firstTime = true;

int enemyAtk;
int playerAtk;

// Aleatorio
std::random_device rd;
std::mt19937 gen(rd());

int random(int low, int high)
{
	std::uniform_int_distribution<> dist(low, high);
	return dist(gen);
}

#pragma region enemyMap
enum eID
{
	SHYGUY, BILLBALA, BLOOPER
};

// Arreglos de cada enemigo
shyGuy sgList[2];
int sizeSG = sizeof(sgList) / sizeof(shyGuy);
billBalla bbList[2];
int sizeBB = sizeof(bbList) / sizeof(billBalla);
Blooper blList[2];
int sizeBL = sizeof(blList) / sizeof(Blooper);

// Contador de cada anemigo
int sgCount = 0, bbCount = 0, blCount = 0;

// Bool de que si ya se termino
bool eMapFinished = false;

// Puntero de cada enemigo
shyGuy* actualSG = nullptr;
billBalla* actualBB = nullptr;
Blooper* actualBL = nullptr;

// Contador total de enemigos actuales
int maxEnemy = 0;

// Patron de enemigos
int enemyPattern[] = { SHYGUY, BLOOPER, BILLBALA, BLOOPER, SHYGUY, BILLBALA };
int sizeEn_Arr = sizeof(enemyPattern) / sizeof(int);

// Contador del patron
int enemyList_Counter = 0;

// Timer del mapa
int eTimer;

// Ultima direccion registrada
int lastDir;

// Funciones
void enemiesMap();
void loadEnemyMap();
void loadEnemyAnim(Shader*);
void enemyColls();

void enemiesMap() {

	if (firstTime) {
		loadEnemyMap();
		return;
	}

	if (!gameStarted or gameOver or eMapFinished)
		return;

	if (eTimer > 3000 and maxEnemy < 2) { // TIMER
		eTimer = 0;
		loadEnemyMap();
	}
	else {
		eTimer++;
	}
}

void loadEnemyMap() {

	if (eMapFinished)
		return;

	int Dir;
	if (firstTime) {
		Dir = random(IZQ, DER);
		lastDir = Dir;
	}
	else {

		if (lastDir == IZQ)
			Dir = DER;
		else
			Dir = IZQ;

		lastDir = Dir;
	}

	switch (enemyPattern[enemyList_Counter]) {
	case SHYGUY: {
		sgList[sgCount].setAnimation();
		sgList[sgCount].setDirection(Dir);
		sgList[sgCount].bDraw = true;
		sgCount++;

	} break;			
	case BILLBALA: {
		bbList[bbCount].setInitModel();
		bbList[bbCount].setDirection(Dir);
		bbList[bbCount].bDraw = true;
		bbCount++;
	} break;
	case BLOOPER: {
		blList[blCount].setInitModel();
		blList[blCount].setDirection(Dir);
		blList[blCount].bDraw = true;
		blCount++;
	} break;
	}

	if (enemyList_Counter < sizeEn_Arr) {
		enemyList_Counter++;
		maxEnemy++;
	}
	else
		eMapFinished = true;
	
}

void loadEnemyAnim(Shader* enShader) {
	if (!gameStarted or gameOver or gameFinished)
		return;


	// shyGuy
	for (int i = 0; i < sizeSG; i++)
	{
		if (sgList[i].isDefeated or !sgList[i].bDraw)
			continue;

		sgList[i].Walk(*enShader);
		sgList[i].moveForward();
	}

	// billBala
	for (int i = 0; i < sizeBB; i++)
	{
		if (bbList[i].isDefeated or !bbList[i].bDraw)
			continue;

		bbList[i].Draw(*enShader);
		bbList[i].moveForward();
	}

	// Blooper
	for (int i = 0; i < sizeBL; i++)
	{
		if (blList[i].isDefeated or !blList[i].bDraw)
			continue;

		blList[i].Draw(*enShader);
		blList[i].moveForward();
	}

}

#pragma endregion



#pragma region buffMap
enum bID
{
	STAR, HEART, COIN
};

// Arreglos de cada enemigo
Star starList[2];
int sizeStar = sizeof(starList) / sizeof(Star);
Heart heartList[2];
int sizeHeart = sizeof(heartList) / sizeof(Heart);
Coin coinList[2];
int sizeCoin = sizeof(coinList) / sizeof(Coin);

// Contador de cada anemigo
int stCount = 0, heCount = 0, coCount = 0;

// Puntero de cada enemigo
Star* actualStar = nullptr;
Heart* actualHeart = nullptr;
Coin* actualCoin = nullptr;

// Bool de que si ya se termino
bool bMapFinished = false;

// Contador total de enemigos actuales
int maxBuff = 0;

// Patron de enemigos
int buffPattern[] = { COIN, HEART, STAR, HEART, COIN, STAR};
int sizeBu_Arr = sizeof(buffPattern) / sizeof(int);

// Contador del patron
int buffList_Counter = 0;

// Timer del mapa
int bTimer;

// Ultima direccion registrada
int lastDir_;

// Funciones
void buffsMap();
void loadBuffMap();
void loadBuffAnim(Shader);
void buffColls();

void buffsMap() {

	if (firstTime) {
		loadBuffMap();
		return;
	}

	if (!gameStarted or gameOver or bMapFinished)
		return;

	if (bTimer > 3000 and maxBuff < 2) { // TIMER
		bTimer = 0;
		loadBuffMap();
	}
	else {
		bTimer++;
	}
}

void loadBuffMap() {

	if (bMapFinished)
		return;

	int Dir;
	if (firstTime) {
		Dir = random(UP, DOWN);
		lastDir_ = Dir;
	}
	else {

		if (lastDir_ == UP)
			Dir = DOWN;
		else
			Dir = UP;

		lastDir_ = Dir;
	}

	int posMod;
	if (Dir == UP) {
		posMod = random(-6, 9);
	}
	if (Dir == DOWN) {
		posMod = random(1, 6);
	}

	switch (buffPattern[buffList_Counter]) {
	case STAR: {
		starList[stCount].setInitModel();
		starList[stCount].setDirection(Dir, posMod);
		starList[stCount].bDraw = true;
		stCount++;

	} break;
	case HEART: {
		heartList[heCount].setInitModel();
		heartList[heCount].setDirection(Dir, posMod);
		heartList[heCount].bDraw = true;
		heCount++;
	} break;
	case COIN: {
		coinList[coCount].setInitModel();
		coinList[coCount].setDirection(Dir, posMod);
		coinList[coCount].bDraw = true;
		coCount++;
	} break;
	}

	if (buffList_Counter < sizeBu_Arr) {
		buffList_Counter++;
		maxBuff++;
	}
	else
		bMapFinished = true;

}

void loadBuffAnim(Shader* enShader) {
	if (!gameStarted or gameOver or gameFinished)
		return;


	// Star
	for (int i = 0; i < sizeStar; i++)
	{
		if (starList[i].isPicked or !starList[i].bDraw)
			continue;

		starList[i].Levitate(*enShader);
	}

	// Heart
	for (int i = 0; i < sizeHeart; i++)
	{
		if (heartList[i].isPicked or !heartList[i].bDraw)
			continue;

		heartList[i].Beat(*enShader);
	}

	// Coin
	for (int i = 0; i < sizeCoin; i++)
	{
		if (coinList[i].isPicked or !coinList[i].bDraw)
			continue;

		coinList[i].Spin(*enShader);
	}
}

#pragma endregion