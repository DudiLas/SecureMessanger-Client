#include "ClientSocket.h"
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

ClientSocket::ClientSocket(std::string ip, int port)
{
	//Initiallize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Cant start winsock, Err #" << wsResult << std::endl;
		exit(1);

	}
	//Create socket
	this->sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		exit(1);
	}

	//Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &hint.sin_addr);
	//connect to the server
	int connResult = connect(this->sock, (sockaddr*)& hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "cant connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(this->sock);
		WSACleanup();
		exit(1);
	}

}