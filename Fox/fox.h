#pragma once
#include "../Animal/animal.h"

class Fox : public Animal {
public:
	static DWORD WINAPI animalCommunicate(LPVOID);
};