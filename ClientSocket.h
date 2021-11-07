#ifndef CLIENT_SOCKET
#define CLIENT_SOCKET
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <vector>
#include <iterator>
#pragma comment(lib, "ws2_32.lib")
#include "Contact.h"
#include <iostream>
#include "Message.h"
#include "utilities.h"
#include "TxtMessage.h"
#include "PullMessage.h"
#define MAX_LEN 256

class ClientSocket
{
	public:
		//socket related
		ClientSocket(std::string ip, int port);
		~ClientSocket();
		void ClientSend(std::string msg, int* err);
		Message* ClientRecv(int* err);

		void setPrivateKey(std::string privateKey);
		void setName(std::string name);
		void setID(uuid id);
		Contact * findContact(std::string name);
		Contact* findContact(uuid id);


		//contacts related
		void addContact(Contact& ct);
		void remContact(uuid id);
		int Exists(uuid id);
		//request related
		int HandleMessageID(int id);

		void handleReqSym(PullMessage pullMsg);
		void handleSendSym(PullMessage pullMsg);
		void handleSendTxt(PullMessage pullMsg);

		//code 10
		int Register();
		//code 20
		int ReClist();
		//code 30
		int RePkey();
		//code 40
		int ReWmsg();
		//code 50
		int Smsg();
		//code 51
		int SreqSkey();
		//code 52
		int SSkey();
		//code 0



	private:
		SOCKET sock;
		std::vector<Contact *> Contacts;
		std::string name;
		uuid id;
		std::string privateKey;




};










#endif
