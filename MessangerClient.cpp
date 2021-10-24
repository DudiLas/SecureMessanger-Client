#include <iostream>
#include <string>
#include <fstream>
#include <WS2tcpip.h>
#include "ClientSocket.h"
#pragma comment(lib, "ws2_32.lib")


int main()
{

	/*
	reading ip:port from server.info file
	*/

	std::string ipAddress; //ip Address of the server
	int port; // Listening port on the server
	std::string serverInfo;//buffer for the ip:port info
	std::ifstream serverInfoFile("server.info");//server.info file opening

	std::getline(serverInfoFile, serverInfo);

	//casting data into appropiate ip:port form
	try
	{
		int pos = serverInfo.find(":");
		ipAddress = serverInfo.substr(0, pos);
		port = std::stoi(serverInfo.substr(pos+1));
	}
	catch(...)
	{
		std::cerr << "cant resolve ip:port from server.info file" << std::endl;
		exit(1);
	}

	ClientSocket* client = new ClientSocket(ipAddress, port);



	
	/*
	//Do-while loop to send and recieve data
	char buf[4096];
	std::string userInput;
	do
	{
		//Prompt from user
		std::cout << "> ";
		std::getline(std::cin, userInput);

		//Send  the text
		int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
		if (sendResult != SOCKET_ERROR)
		{
			//wait for resonse
			ZeroMemory(buf, 4096);
			int bytesReceived = recv(sock, buf, 4096, 0);
			std::cout << std::string(buf, 0, bytesReceived) << std::endl;

		}


	} while (userInput.size() > 0);

	//Gracefully close down everything
	closesocket(sock);
	WSACleanup();
	
	*/
	return 0;


}



