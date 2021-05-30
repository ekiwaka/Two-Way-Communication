//Base class for Fox and Cat
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma once
#include <iostream>
#include <string.h>
#include <winsock2.h>

using namespace std;

class Animal {

public:

	static DWORD WINAPI animalReceive(LPVOID lpParam)
	{
		// Created buffer[] to receive message
		char buffer[1024] = { 0 };

		// Create socket for animal
		SOCKET animal = *(SOCKET*)lpParam;

		// Pointer to the first element of the reversed array returned by translate()
		char* reversed;

		// animal executes continuously
		while (true) {

			// If received buffer gives error then return -1
			if (recv(animal, buffer,
				sizeof(buffer), 0)
				== SOCKET_ERROR) {
				cout << "recv function failed with error: "
					<< WSAGetLastError()
					<< endl;
				return -1;
			}

			// If fox exits
			if (strcmp(buffer, "exit") == 0) {
				cout << "Conversation has ended."
					<< endl;

				//exit(1);
				return 1;
			}

			// Print the message that was stored in buffer
			cout << "\nMessage received: " << buffer; // << endl;

			cout << "\nTranslated message: ";
			reversed = translate(buffer);

			for (int i = 0; i < strlen(buffer); i++) {
				cout << *(reversed + i);
			}

			cout << "\n\n";


			// Clear buffer message
			memset(buffer, 0, sizeof(buffer));
		}
		return 1;
	};


	static DWORD WINAPI animalSend(LPVOID lpParam)
	{
		// Created buffer[] to receive message
		char buffer[1024] = { 0 };

		// Created socket
		SOCKET animal = *(SOCKET*)lpParam;

		// cat executes continuously
		while (true) {

			// Input message cat wants to send to fox
			fgets(buffer, sizeof(buffer), stdin);

			// If sending failed
			// return -1
			if (send(animal,
				buffer,
				sizeof(buffer), 0)
				== SOCKET_ERROR) {
				cout << "send failed with error: "
					<< WSAGetLastError() << endl;
				return -1;
			}

			// If animal exits
			if (strcmp(buffer, "exit\n") == 0) {
				cout << "Thank you!"
					<< endl;
				exit(0);
				break;
			}

			// Clear buffers
			memset(buffer, 0, sizeof(buffer));
		}
		return 1;
	};


	static DWORD WINAPI animalCommunicate(LPVOID lpParam)
	{
		WSADATA WSAData;

		// Create socket for server
		SOCKET animalFriend;
		SOCKADDR_IN addr;

		WSAStartup(MAKEWORD(2, 0), &WSAData);

		// If invalid socket created, return -1
		if ((animalFriend = socket(AF_INET,
			SOCK_STREAM, 0))
			== INVALID_SOCKET) {
			cout << "Socket creation failed with error: "
				<< WSAGetLastError() << endl;
			return -1;
		}

		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_family = AF_INET;
		addr.sin_port = htons(5555);

		// If connection failed
		if (connect(animalFriend,
			(SOCKADDR*)&addr,
			sizeof(addr))
			== SOCKET_ERROR) {
			cout << "Connection failed with error: "
				<< WSAGetLastError() << endl;
			return -1;
		}

		// If connection established
		cout << "Ready to chat!" << endl
			<< "Enter \"exit\" to disconnect"
			<< endl;
		DWORD threadID;

		// Create Thread to receive data from fox
		HANDLE threadReceive = CreateThread(NULL,
			0,
			animalReceive,
			&animalFriend,
			0, &threadID);

		// If created thread is not created
		if (threadReceive == NULL)
			cout << "Thread creation error: "
			<< GetLastError();


		// Create Thread to send data to fox
		HANDLE threadSend = CreateThread(NULL,
			0,
			animalSend,
			&animalFriend,
			0, &threadID);

		// If created thread is not created
		if (threadSend == NULL)
			cout << "Thread creation error: "
			<< GetLastError();

		// Wait for threads to finish
		WaitForSingleObject(threadReceive, INFINITE);
		WaitForSingleObject(threadSend, INFINITE);

		// Close the socket
		closesocket(animalFriend);
		WSACleanup();
	};
};