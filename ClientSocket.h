#ifndef CLIENT_SOCKET
#define CLIENT_SOCKET
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <list>
#include <iterator>
#pragma comment(lib, "ws2_32.lib")
#include "Contact.h"

class ClientSocket
{
	public:
		ClientSocket(std::string ip, int port);

	private:
		SOCKET sock;
		std::list<Contact> Contacts;




};










#endif
