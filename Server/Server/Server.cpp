// Server.cpp : Defines the entry point for the console application.
//
//SERVER

#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <stdio.h>
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib")

void recieve(SOCKET newConnection, int deleteTag) {
	char messageToRecieve[256];
	memset(messageToRecieve, 0, sizeof(messageToRecieve));
	int exitCode;
	while (true) {
		recv(newConnection, messageToRecieve, sizeof(messageToRecieve), NULL);
		if (deleteTag != 0) {
			std::cout << '\b' << '\b' << '\b' << '\b' << '\b' << '\b' << '\b' << '\b';
			deleteTag--;
		}
		if (messageToRecieve[0] == '/' && messageToRecieve[1] == 'e' && messageToRecieve[2] == 'x' && messageToRecieve[3] == 'i' && messageToRecieve[4] == 't') {
			std::cout << "Client has disconnected." << std::endl;
			std::cin >> exitCode;
			exit(1);
		}
		std::cout << "client: " << messageToRecieve << std::endl;
		memset(messageToRecieve, 0, sizeof(messageToRecieve));
		std::cout << "server: ";
	}
}
void sendMessage(SOCKET newConnection, int deleteTag) {
	std::string theMessage;
	char messageToSend[256];
	memset(messageToSend, 0, sizeof(messageToSend));
	while (true) {
		if (deleteTag == 0) {
			++deleteTag;
		}
		std::cout << "server: ";
		std::getline(std::cin, theMessage);
		for (int i = 0; i < theMessage.length(); i++) {
			if (theMessage[i] == ' ') {
				theMessage[i] = '_';
			}
		}
		for (int i = 0; i < theMessage.length(); i++) {
			messageToSend[i] = theMessage[i];
		}
		send(newConnection, messageToSend, sizeof(messageToSend), NULL);
		if (messageToSend[0] == '/' && messageToSend[1] == 'e' && messageToSend[2] == 'x' && messageToSend[3] == 'i' && messageToSend[4] == 't') {
			std::cout << "Exiting...";
			exit(1);
		}
		memset(messageToSend, 0, sizeof(messageToSend));
	}
}

int main()
{

	WSADATA wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		std::cout << "Winsock Failed." << std::endl;
	}

	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Put the address in the brackets
	addr.sin_port = htons(1111); //Put port in brackets
	addr.sin_family = AF_INET; //IPV4 Socket

	std::cout << "Connecting..." << std::endl;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);

	int deleteTag;

	if (newConnection != 0) {
		std::cout << "Connection Successful!" << std::endl;
	}

	if (newConnection == 0) {
		std::cout << "Connection Failed." << std::endl;
		return 0;
	}

	std::thread t1(recieve, newConnection, deleteTag);
	std::thread t2(sendMessage, newConnection, deleteTag);

	t1.join();
	t2.join();

	return 0;
}