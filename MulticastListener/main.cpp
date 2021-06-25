//#include <Windows.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <tchar.h>
#pragma comment(lib, "ws2_32.lib")



//static const auto LOCAL_IP = _T("10.159.1.9");
static const auto LOCAL_IP = _T("127.0.0.1");


static const auto MULTICAST_GROUP_ADDRESS = _T("225.0.140.140");
static const unsigned short MULTICAST_GROUP_PORT = 14000;
static const int MSGBUFSIZE = 1024;

int init_winsock()
{
	int iResult;
	WSADATA wsaData;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	return 0;
}


int test_multicast()
{
	int fd;
	/* create what looks like an ordinary UDP socket */
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return(1);
	}

	/**** MODIFICATION TO ORIGINAL */
	/* allow multiple sockets to use the same PORT number */
	u_int yes = 1;            /*** MODIFICATION TO ORIGINAL */
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes)) < 0) {
		perror("Reusing ADDR failed");
		return(1);
	}
	/*** END OF MODIFICATION TO ORIGINAL */

	/* set up destination address */
	struct sockaddr_in addr = {};
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); /* N.B.: differs from sender */
	//InetPton(AF_INET, LOCAL_IP, &addr.sin_addr.s_addr);
	addr.sin_port = htons(MULTICAST_GROUP_PORT);

	/* bind to receive address */
	if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		perror("bind");
		return(1);
	}

	/* use setsockopt() to request that the kernel join a multicast group */
	struct ip_mreq mreq = {};
	InetPton(AF_INET, LOCAL_IP, &mreq.imr_interface.s_addr);
	InetPton(AF_INET, MULTICAST_GROUP_ADDRESS, &mreq.imr_multiaddr.s_addr);
	if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&mreq, sizeof(mreq)) < 0) {
		printf("set error IP_ADD_MEMBERSHIP %d\n", WSAGetLastError());

		//return(1);
	}

	/* now just enter a read-print loop */
	int nbytes = 0;
	int addrlen = sizeof(addr);
	char msgbuf[MSGBUFSIZE] = {};
	while (1) {
		if ((nbytes = recvfrom(fd, msgbuf, MSGBUFSIZE, 0, (struct sockaddr*)&addr, &addrlen)) < 0) {
			perror("recvfrom");
			return(1);
		}
		msgbuf[nbytes] = 0;
		printf("%s\n", msgbuf);
	}

	return 0;
}

int main()
{
	int ret = init_winsock();
	if (0 != ret) { return ret; }


	ret = test_multicast();

	getchar();
}