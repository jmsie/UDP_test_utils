/*
	Simple UDP Server
*/

#include<stdio.h>
#include<winsock2.h>

#include <string>
//#include "minwinbase.h"
#include <windows.h>


#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFLEN 512	//Max length of buffer


std::string get_current_time()
{
	std::string Today;
	char buffer[128];

	SYSTEMTIME SystemTime;

	GetLocalTime(&SystemTime);

	sprintf_s(buffer, sizeof(buffer), "%04d_%02d_%02d %02d:%02d:%02d:%03d",
		SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,
		SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds);

	Today.append(buffer);

	return Today;
}


int main()
{
	char inputStr[128];
	printf(" Input port to listen : ");
	scanf_s("%s", inputStr, sizeof(inputStr));
	printf("Listen to port: %s", inputStr);

	SOCKET s;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	char buf[BUFLEN];
	WSADATA wsa;

	slen = sizeof(si_other);

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(atoi(inputStr));

	//Bind
	if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	puts("Bind done");

	//keep listening for data
	while (1)
	{
		printf("Waiting for data...");
		fflush(stdout);

		//clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);

		//try to receive some data, this is a blockaing call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		printf("Data: %s %s\n", get_current_time().c_str(), buf);

	}

	closesocket(s);
	WSACleanup();

	return 0;
}