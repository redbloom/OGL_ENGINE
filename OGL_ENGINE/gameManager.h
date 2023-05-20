#pragma once
#include <random>
#include <engine/model.h>
#define TOTAL_DIGITS 4

// Juego
bool startMessage = false;
bool gameCompleted = false;
bool showFinalMsg = false;
// Objeto - interaccion
bool interactingWithObject = false;
bool stopGettingInfo = false;
bool collidedObject = false;
int lastObjectInt;
// Teclas 
bool clickOneTime = false;
bool oneObjAtTime = false;
// Digitos
bool digitFound = false;



// Aleatorio
std::random_device rd;
std::mt19937 gen(rd());

int random(int low, int high)
{
	std::uniform_int_distribution<> dist(low, high);
	return dist(gen);
}

// Digits
class Password
{public:
	int digit;
	bool isFound;
	int assignedPlace;

	Password() {
		this->isFound = false;
	}
} code[TOTAL_DIGITS];



enum DIGITS {
	NONE = 11
};

void generatePass(string* names, int size) {

	int randomPlace;
	int i = 0;
	int placesChoosen[TOTAL_DIGITS];

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
	for (i = 0; i < TOTAL_DIGITS; i++) {
		code[i].assignedPlace = placesChoosen[i];
		code[i].digit = random(0, 9);
	}
}

void digitTakenCode(int modelLocation) {
	for (int i = 0; i < TOTAL_DIGITS; i++)
	{
		if (code[i].assignedPlace == modelLocation) 
			code[i].isFound = true;
		
	}
}

string digitsToText() {
	string digitsCollected = "";
	int digitsNumber = 0;
	for (int i = 0; i < TOTAL_DIGITS; i++)
	{
		if (code[i].isFound) {
			digitsCollected += to_string(code[i].digit);
			digitsNumber++;

		}
		else
			digitsCollected += " ";

	}

	if (digitsNumber == TOTAL_DIGITS) {
		gameCompleted = true;
		showFinalMsg = true;
	}

	return digitsCollected;
}