// Creates the Server, hereby referenced as Fox
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <string.h>
#include <winsock2.h>
#include "../Animal/translate.h"
#include "fox.h" 

#pragma comment(lib, "Ws2_32.lib")

using namespace std;
// Function to create sockets
DWORD WINAPI Fox::animalCommunicate(LPVOID lpParam)
{
	WSADATA WSAData;

	// Create sockets for fox (server) and cat (client)
	SOCKET fox, foxFriend;

	// Socket addresses for fox and cat
	SOCKADDR_IN foxAddr, foxFriendAddr;

	WSAStartup(MAKEWORD(2, 0), &WSAData);

	// Making fox
	fox = socket(AF_INET, SOCK_STREAM, 0);

	// If invalid socket created, return -1
	if (fox == INVALID_SOCKET) {
		cout << "Socket creation failed with error:"
			<< WSAGetLastError() << endl;
		return -1;
	}
	foxAddr.sin_addr.s_addr = INADDR_ANY;
	foxAddr.sin_family = AF_INET;
	foxAddr.sin_port = htons(5555);

	// If socket error occurred, return -1
	if (bind(fox,
		(SOCKADDR*)&foxAddr,
		sizeof(foxAddr))
		== SOCKET_ERROR) {
		cout << "Bind function failed with error: "
			<< WSAGetLastError() << endl;
		return -1;
	}

	// Get the request from fox
	if (listen(fox, 0)
		== SOCKET_ERROR) {
		cout << "Listen function failed with error:"
			<< WSAGetLastError() << endl;
		return -1;
	}

	cout << "Listening for incoming connections...." << endl;

	// Intialise address for cat socket
	int catAddrSize = sizeof(foxFriendAddr);

	// If connection established
	if ((foxFriend = accept(fox,
		(SOCKADDR*)&foxFriendAddr,
		&catAddrSize))
		!= INVALID_SOCKET) {
		cout << "Ready to chat!" << endl
			<< "Enter \"exit\" to disconnect"
			<< endl;
		/*
		cout << "Cat connected!" << endl;
		cout << "Now you can chat with the Cat."
			<< "Enter \"exit\" to disconnect" << endl;*/


		DWORD threadID;

		// Create Thread to receive data by fox
		HANDLE threadReceive = CreateThread(NULL,
			0,
			animalReceive,
			&foxFriend,
			0,
			&threadID);

		// If created thread is not created
		if (threadReceive == NULL) {
			cout << "Thread Creation Error: "
				<< WSAGetLastError() << endl;
		}

		// Create Thread to send data from fox
		HANDLE threadSend = CreateThread(NULL,
			0,
			animalSend,
			&foxFriend,
			0,
			&threadID);

		// If created thread is not created
		if (threadSend == NULL) {
			cout << "Thread Creation Error: "
				<< WSAGetLastError() << endl;
		}

		// Wait for threads to finish
		WaitForSingleObject(threadReceive, INFINITE);
		WaitForSingleObject(threadSend, INFINITE);

		// Close the socket
		closesocket(foxFriend);

		// If socket closing failed.
		if (closesocket(fox)
			== SOCKET_ERROR) {
			cout << "Close socket failed with error: "
				<< WSAGetLastError() << endl;
			return -1;
		}
		WSACleanup();
	}
}


int main()
{
	// Instantiate object
	Fox fox; //<-- Client 

	HANDLE _fox = CreateThread(NULL, 0, fox.animalCommunicate, NULL, 0, NULL);

	// Wait for them to finish
	WaitForSingleObject(_fox, INFINITE);

	return 0;
}