// Client.cpp : Defines the entry point for the console application.
//
//CLIENT

#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <stdio.h>
#include <thread>
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib")

void recieve(SOCKET Connection, int deleteTag) {
	char messageToRecieve[256];
	memset(messageToRecieve, 0, sizeof(messageToRecieve));
	int exitCode;
	while (true) {
		recv(Connection, messageToRecieve, sizeof(messageToRecieve), NULL);
		if (deleteTag != 0) {
			std::cout << '\b' << '\b' << '\b' << '\b' << '\b' << '\b' << '\b' << '\b';
			deleteTag--;
		}
		if (messageToRecieve[0] == '/' && messageToRecieve[1] == 'e' && messageToRecieve[2] == 'x' && messageToRecieve[3] == 'i' && messageToRecieve[4] == 't') {
			std::cout << "Server has disconnected." << std::endl;
			std::cin >> exitCode;
			exit(1);
		}
		std::cout << "server: " << messageToRecieve << std::endl;
		memset(messageToRecieve, 0, sizeof(messageToRecieve));
		std::cout << "client: ";
	}
}
void sendMessage(SOCKET Connection, int deleteTag) {
	std::string theMessage;
	char messageToSend[256];
	while (true) {
		memset(messageToSend, 0, sizeof(messageToSend));
		if (deleteTag == 0) {
			deleteTag++;
		}
		std::cout << "client: ";
		std::getline(std::cin, theMessage);
		for (int i = 0; i < theMessage.length(); i++) {
			if (theMessage[i] == ' ') {
				theMessage[i] = '_';
			}
		}
		for (int i = 0; i < theMessage.length(); i++) {
			messageToSend[i] = theMessage[i];
		}
		send(Connection, messageToSend, sizeof(messageToSend), NULL);
		if (messageToSend[0] == '/' && messageToSend[1] == 'e' && messageToSend[2] == 'x' && messageToSend[3] == 'i' && messageToSend[4] == 't') {
			std::cout << "Exiting...";
			exit(1);
		}
	}
}

int main()
{
	WSADATA wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		std::cout << "Connection Failed." << std::endl;
		exit(1);
	}
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Put the address in the brackets
	addr.sin_port = htons(1111); //Put port in brackets
	addr.sin_family = AF_INET; //IPV4 Socket

	std::cout << "Connecting..." << std::endl;

	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) {
		std::cout << "Failed to connect." << std::endl;
	}
	else {
		std::cout << "Connection Successful!" << std::endl;
	}

	int deleteTag;

	std::thread t1(recieve, Connection, deleteTag);
	std::thread t2(sendMessage, Connection, deleteTag);

	t1.join();
	t2.join();

	return 0;
}