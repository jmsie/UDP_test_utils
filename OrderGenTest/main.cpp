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

	symbol.push_back("TXF_1");
	symbol.push_back("MXF_1");

	signal_name.push_back("KD_L2");
	signal_name.push_back("2020_1");
	signal_name.push_back("ST71");
	position.push_back(-1);
	position.push_back(0);
	position.push_back(1);
	position.push_back(-1);
	position.push_back(1);
	position.push_back(0);


	//start communication
	for (auto symbol_it = symbol.begin(); symbol_it != symbol.end(); symbol_it++)
	{
		for (auto position_it = position.begin(); position_it != position.end(); position_it++) {
			for (auto signal_name_it = signal_name.begin(); signal_name_it != signal_name.end(); signal_name_it++) {
				sprintf_s(message, BUFLEN, "%s,%s,%d", symbol_it->c_str(), signal_name_it->c_str(), *position_it);
				cout << "Sending..." << message << endl;
				//send the message
				if (sendto(s, message, strlen(message), 0, (struct sockaddr*)&si_other, slen) == SOCKET_ERROR)
				{
					printf("sendto() failed with error code : %d", WSAGetLastError());
					exit(EXIT_FAILURE);
				}
			}
		}
	}

	closesocket(s);
	WSACleanup();

	return 0;
}