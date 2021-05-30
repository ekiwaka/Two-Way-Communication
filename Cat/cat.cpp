// C++ program to create cat
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include "../Animal/translate.h"
#include "cat.h"

#pragma comment(lib, "Ws2_32.lib")


int main()
{
	// Instantiate object
	Cat cat; //<-- Client

	HANDLE _cat = CreateThread(NULL, 0, cat.animalCommunicate, NULL, 0, NULL);

	// Wait for thread to finish
	WaitForSingleObject(_cat, INFINITE);

	return 0;
}