/*
	Simple udp client
*/
#include<stdio.h>
#include<winsock2.h>
#include<ws2tcpip.h>
#include<tchar.h>
#include<iostream>
#include<list>
#include<string>

using namespace std;

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define SERVER "127.0.0.1"	//ip address of udp server
#define BUFLEN 512	//Max length of buffer

int main(void)
{
	char inputStr[128];
	printf(" Input port to send : ");
	scanf_s("%s", inputStr, sizeof(inputStr));
	printf("Send to port: %s", inputStr);


	struct sockaddr_in si_other;
	int s, slen = sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN];
	list<string> symbol;
	list<string> signal_name;
	list<int> position;
	WSADATA wsa;

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//create socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//setup address structure
	memset((char*)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(atoi(inputStr));

	InetPton(AF_INET, _T(SERVER), &si_other.sin_addr.s_addr);



	while (true)
	{

		printf("-----------------------------------------------\n");
		printf(" Input String or x to quit : \n");
		scanf_s("%s", inputStr, sizeof(inputStr));
		if (inputStr[0] == 'x' || inputStr[0] == 'X')
			exit(0);

		printf("Sending: %s\n", inputStr);
		//send the message
		if (sendto(s, inputStr, strlen(inputStr), 0, (struct sockaddr*)&si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

	}//end while



	closesocket(s);
	WSACleanup();

	return 0;
}