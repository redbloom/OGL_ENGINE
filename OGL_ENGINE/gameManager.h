#pragma once
#include <random>
#include <engine/model.h>
#define TOTAL_DIGITS 4
#define TIME_LIMIT 25

// Juego
bool gameStarted = false;
bool gameCompleted = false;
int digitsFound[TOTAL_DIGITS];
bool interactingWithObject = false;
bool stopGettingInfo = false;
bool collidedObject = false;
bool clickOneTime = false;
bool oneObjAtTime = false;
bool digitFound = false;
int lastObjectInt;

int cooldown = TIME_LIMIT;

int digitsCounter = 0;

// Aleatorio
std::random_device rd;
std::mt19937 gen(rd());

int random(int low, int high)
{
	std::uniform_int_distribution<> dist(low, high);
	return dist(gen);
}

// Digits
int generatedPassword[TOTAL_DIGITS];
int placesChoosen[TOTAL_DIGITS];

enum DIGITS {
	NONE = 11
};

void generatePass(string* names, int size) {

	int randomPlace;
	int i = 0;

	size -= 1;

	// Escogiendo que modelo tendra un digito 
	int placeCount = 0;
	do { 

		bool sameNumber = false;
		randomPlace = random(0, size);
		i = randomPlace;

		if (names[i] == "mcDonalds" || names[i] == "Seat01" || names[i] == "Seat02"
			|| names[i] == "Seat03" || names[i] == "Seat04" || names[i] == "Seat05"
			|| names[i] == "Table01" || names[i] == "Table02" || names[i] == "Table03"
			|| names[i] == "Counter" || names[i] == "Arcade") {
			continue;
		}
		
		for (int i = 0; i < placeCount; i++)
		{
			if (randomPlace == placesChoosen[i])
				sameNumber = true;
		}

		if(!sameNumber)
		{
			placesChoosen[placeCount] = randomPlace;
			placeCount++;
		}
		

	} while (placeCount < TOTAL_DIGITS);

	int temp;

	// Ordenando los lugares asignados de menor a mayor, 
	// esto para que sea mas fácil ubicarlos en el for (que se usara 
	// para poner el digito correspondiente)
	for (i = 0; i < TOTAL_DIGITS; i++) {
		for (int j = i + 1; j < TOTAL_DIGITS; j++)
		{
			if (placesChoosen[j] < placesChoosen[i]) {
				temp = placesChoosen[i];
				placesChoosen[i] = placesChoosen[j];
				placesChoosen[j] = temp;
			}
		};
	}


	// Generando los digitos
	for (size_t i = 0; i < TOTAL_DIGITS; i++)
		generatedPassword[i] = random(0, 9);
}