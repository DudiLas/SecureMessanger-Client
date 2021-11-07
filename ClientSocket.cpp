#include "ClientSocket.h"


ClientSocket::ClientSocket(std::string ip, int port)
{
	//Initiallize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Cant start winsock, Err #" << wsResult << std::endl;
		std::exit(1);

	}
	//Create socket
	this->sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		std::exit(1);
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
		std::exit(1);
	}


	memset(this->id.uuid, 0, UUID_SIZE);

}

ClientSocket::~ClientSocket()
{
	for (auto it = Contacts.begin(); it != Contacts.end(); ++it)
	{
		Contact * tempCon = *it;
		delete tempCon;
	}

}


void ClientSocket::ClientSend(std::string msg, int* err)
{
	int sendResult = send(this->sock, msg.c_str(), msg.size() + 1, 0);
	*err = sendResult;
}

Message* ClientSocket::ClientRecv(int* err)
{

	char header[HEADER_LEN + 1];
	char* msgStr;
	unsigned char Version;
	unsigned short Code;
	unsigned int PayloadSize;
	std::string header_s;
	std::string msgStr_s = "";

	memset(header, 0, HEADER_LEN);

	*err = recv(this->sock, header, HEADER_LEN, 0);
	if (*err == SOCKET_ERROR)
	{
		return new Message(1, 1);
	}

	header[HEADER_LEN] = '\0';
	header_s = chTostr(header, HEADER_LEN);
	Version = header_s[0];
	Code = twoBytesToInteger(header_s.substr(1, 2));
	PayloadSize = fourBytesToInteger(header_s.substr(3, 4));
	Message* msg = new Message(Version, Code);

	if (PayloadSize > 0)
	{
	
		msgStr = (char*)malloc((PayloadSize + 1) * sizeof(char));
		*err = recv(this->sock, msgStr, (PayloadSize + 1) * sizeof(char), 0);
		if (*err == SOCKET_ERROR)
		{
			return new Message(1, 1);
		}
		msgStr[PayloadSize] = 0;
		msgStr_s = chTostr(msgStr, PayloadSize);
	}
	msg->setPayload(msgStr_s);
	return msg;
	

}



void ClientSocket::addContact(Contact& ct)
{
	Contact* temp = new Contact(ct);
	if(!this->Exists(temp->getID()))
		this->Contacts.push_back(temp);
}



void ClientSocket::remContact(uuid id)
{
	for (auto it = Contacts.begin(); it != Contacts.end(); ++it)
	{
		Contact tempCon = **it;
		if (!memcmp(id.uuid, tempCon.getID().uuid, UUID_SIZE))
		{
			Contacts.erase(it);
		}
	}
}

int ClientSocket::Exists(uuid id)
{
	for (auto it = Contacts.begin(); it != Contacts.end(); ++it)
	{
		Contact tempCon = **it;
		if (!memcmp(id.uuid, tempCon.getID().uuid, UUID_SIZE))
		{
			return 1;
		}
	}
	return 0;
}

Contact * ClientSocket::findContact(std::string name)
{
	for (auto it = Contacts.begin(); it != Contacts.end(); ++it)
	{
		Contact * tempCon = *it;
		if (name == tempCon->getName())
		{
			return tempCon;
		}
	}
	return nullptr;
}

Contact* ClientSocket::findContact(uuid id)
{
	for (auto it = Contacts.begin(); it != Contacts.end(); ++it)
	{
		Contact * tempCon = *it;
		if (!memcmp(id.uuid, tempCon->getID().uuid, UUID_SIZE))
		{
			return tempCon;
		}
	}
	return nullptr;
}


void ClientSocket::setPrivateKey(std::string privateKey)
{
	this->privateKey = privateKey;
}


void ClientSocket::setName(std::string name)
{
	this->name = name;
}

void ClientSocket::setID(uuid id)
{
	memcpy(this->id.uuid, id.uuid, UUID_SIZE);
}



int ClientSocket::HandleMessageID(int msgId)
{
	//route to spesific message type
	if (msgId != 10 && msgId != 0 && !fileExists("me.info"))
	{
		std::cout << "ERROR: Unregisted client" << std::endl;
		return 0;

	}
	switch (msgId)
	{
	case 10:
		return Register();
		break;
	case 20:
		return ReClist();
		break;
	case 30:
		return RePkey();
		break;
	case 40:
		return ReWmsg();
		break;
	case 50:
		return Smsg();
		break;
	case 51:
		return SreqSkey();
		break;
	case 52:
		return SSkey();
		break;
	case 0:
		return 1;



	};

	return 0;


}

int ClientSocket::Register()
{
	std::string fileName = "me.info";
	std::string name;
	std::string myName;
	std::string payload;
	int err;
	//check if me.info exists
	if (fileExists(fileName))
	{
		std::cout << "Error: already registed" << std::endl;
		return 0;
	}
	
	//input the name and copy to me.info
	
	std::cout << "Enter Username: " << std::endl;
	std::getline(std::cin, name);
	myName = name;

	if (name.length() < MAX_WORD_LEN)
	{
		fillWithBytes(name, MAX_WORD_LEN);
	}


	//creates message for the server
	Message* msg = new Message(VERSION, REGISTER_CODE);

	//get public key
	RSAPrivateWrapper rsapriv(Base64Wrapper::decode(privateKey));
	std::string pubkey = rsapriv.getPublicKey();

	//define message payload
	payload = name + pubkey;
	msg->setPayload(payload);

	//send message to server & handle error
	this->ClientSend(msg->to_string(), &err);
	if (err == SOCKET_ERROR)
	{
		std::cout << "could not send to server" << std::endl;
		delete msg;
		return 1;
	}

	/*
	*****************
	Handle server response
	*****************
	*/

	Message* RecMsg = this->ClientRecv(&err);
	if (err == SOCKET_ERROR)
	{
		std::cout << "could not receive from server" << std::endl;
		delete msg;
		return 1;
	}

	if (RecMsg->Code == ERROR)
	{
		std::cout << "ERROR: registration failed, name already exists" << std::endl;
		delete msg;
		return 0;
	}

	else if (RecMsg->Code == SUC_REGISTER)
	{
		std::ofstream serverInfoFile(fileName);
		memcpy(this->id.uuid, RecMsg->payload.c_str(), UUID_SIZE);
		this->setName(myName);

		serverInfoFile << myName << std::endl;

		serverInfoFile << string_to_hex(uuidToStr(id)) << std::endl;
		serverInfoFile << this->privateKey << std::endl;
		serverInfoFile.close();
	}


	//handle closed data
	
	delete msg;
	delete RecMsg;
	return 0;

}



int ClientSocket::ReClist()
{
	//creates message for the server
	Message* msg = new Message(VERSION, RECEIVE_LIST_CODE);
	int err;
	msg->setPayload("");
	msg->setID(this->id);

	//send message to server & handle error
	this->ClientSend(msg->to_string(), &err);
	if (err == SOCKET_ERROR)
	{
		std::cout << "could not send to server";
		delete msg;
		return 1;
	}

	/*
	*****************
	Handle server response
	*****************
	*/

	Message* RecMsg = this->ClientRecv(&err);
	if (err == SOCKET_ERROR)
	{
		std::cout << "could not recv from serverP";
		delete msg;
		return 1;
	}

	if (RecMsg->Code == CLIENT_LIST)
	{
		int numClients = RecMsg->PayloadSize / (MAX_WORD_LEN + UUID_SIZE);
		std::cout << "Client List: " << std::endl;
		for (int i = 0; i < numClients; i++)
		{
			
			int curPos = i * (MAX_WORD_LEN + UUID_SIZE);
			std::string curName = std::string(RecMsg->payload.substr(curPos + UUID_SIZE, UUID_SIZE + MAX_WORD_LEN).c_str());
			int fin = curPos + UUID_SIZE;
			std::string ID = RecMsg->payload.substr(curPos, UUID_SIZE);
			std::string hexID = string_to_hex(ID);

			uuid id;
			memcpy(id.uuid, ID.c_str(), UUID_SIZE);
			Contact* ct = new Contact(id, curName);
			if (memcmp(this->id.uuid, ct->getID().uuid, UUID_SIZE))
			{
				this->addContact(*ct);
			}

			std::cout << curName << " ";
			std::cout << hexID << std::endl;

		}
	}


	//handle closed data
	delete RecMsg;
	delete msg;
	return 0;

}


int ClientSocket::RePkey()
{
	//creates message for the server
	Message* msg = new Message(VERSION, PUBLIC_KEY_CODE);
	int err;
	std::string name;

	std::cout << "Enter Name: ";
	getline(std::cin, name);

	Contact * ct = this->findContact(name);
	if (ct == nullptr)
	{
		std::cout << "ERROR: contact not found";
		return 0;
	}
	msg->setPayload(uuidToStr(ct->getID()));
	msg->setID(this->id);

	//send message to server & handle error
	this->ClientSend(msg->to_string(), &err);
	if (err == SOCKET_ERROR)
	{
		std::cout << "could not send to server";
		delete msg;
		return 1;
	}

	Message* RecMsg = this->ClientRecv(&err);
	if (err == SOCKET_ERROR)
	{
		std::cout << "could not recv from server";
		delete msg;
		delete RecMsg;
		return 1;
	}

	if (RecMsg->Code == PUBLIC_REQ)
	{
		std::string id_s = RecMsg->payload.substr(0, UUID_SIZE);
		std::string publicKey = RecMsg->payload.substr(UUID_SIZE, PUBLIC_KEY_SIZE);
		uuid id;
		memcpy(id.uuid, id_s.c_str(), UUID_SIZE);
		Contact* ct = findContact(id);
		ct->setPublickey(publicKey);

		std::cout << publicKey << std::endl;

	}

	//handle closed data
	delete RecMsg;
	delete msg;
	return 0;
}



int ClientSocket::SreqSkey()
{
	//creates message for the server
	Message* msg = new Message(VERSION, MSG_CODE);
	int err;
	std::string name;

	std::cout << "Enter Name: ";
	getline(std::cin, name);

	Contact* ct = this->findContact(name);
	if (ct == nullptr)
	{
		std::cout << "ERROR: contact not found";
		delete msg;
		return 0;
	}

	TxtMessage* txtmsg = new TxtMessage(SYM_REQ);
	txtmsg->setID(ct->getID());
	txtmsg->setPayload("");

	msg->setPayload(txtmsg->to_string());
	msg->setID(this->id);

	//send message to server & handle error
	this->ClientSend(msg->to_string(), &err);
	if (err == SOCKET_ERROR)
	{
		std::cout << "could not send to server";
		delete txtmsg;
		delete msg;

		return 1;
	}

	Message* RecMsg = this->ClientRecv(&err);
	if (err == SOCKET_ERROR)
	{
		std::cout << "could not recv from server";
		delete RecMsg;
		delete txtmsg;
		delete msg;
		return 1;
	}

	if (RecMsg->Code == MSG_REC)
	{
		std::cout << "Message succesfully recieved" << std::endl;

	}


	//handle closed data
	delete RecMsg;
	delete txtmsg;
	delete msg;
	return 0;


}


int ClientSocket::SSkey()
{
	//creates message for the server
	Message* msg = new Message(VERSION, MSG_CODE);
	int err;
	std::string name;

	std::cout << "Enter Name: ";
	getline(std::cin, name);

	Contact* ct = this->findContact(name);
	if (ct == nullptr)
	{
		std::cout << "ERROR: contact not found" << std::endl;
		return 0;
	}

	TxtMessage* txtmsg = new TxtMessage(SEND_SYM);
	txtmsg->setID(ct->getID());

	if (!ct->isPublicKey())
	{
		std::cout << "ERROR: public key not found" << std::endl;
		delete msg;
		delete txtmsg;
		return 0;
	}
	std::string publicKey = ct->getPublickey();

	std::string symKey;
	if (!ct->isSymetricKey())
	{
		// 1. Generate a key and initialize an AESWrapper. You can also create AESWrapper with default constructor which will automatically generates a random key.
		unsigned char key[AESWrapper::DEFAULT_KEYLENGTH];
		AESWrapper::GenerateKey(key, AESWrapper::DEFAULT_KEYLENGTH);
		symKey = chTostr((const char *)key, AESWrapper::DEFAULT_KEYLENGTH);
		ct->setSymetrickey(symKey);
	}
	else
	{
		symKey = ct->getSymetrickey();
	}


	RSAPublicWrapper rsapub(ct->getPublickey());
	std::string cipher = rsapub.encrypt(symKey.c_str(), symKey.length());	// you can encrypt a const char* or an std::string


	txtmsg->setPayload(cipher);

	msg->setPayload(txtmsg->to_string());
	msg->setID(this->id);

	//send message to server & handle error
	this->ClientSend(msg->to_string(), &err);
	if (err == SOCKET_ERROR)
	{
		std::cout << "could not send to server";
		delete msg;
		delete txtmsg;
		return 1;
	}

	Message* RecMsg = this->ClientRecv(&err);
	if (err == SOCKET_ERROR)
	{
		std::cout << "could not recv from server";
		delete msg;
		delete txtmsg;
		delete RecMsg;
		return 1;
	}

	if (RecMsg->Code == MSG_REC)
	{
		std::cout << "Message succesfully recieved" << std::endl;

	}


	//handle closed data
	delete RecMsg;
	delete msg;
	delete txtmsg;
	return 0;


}





int ClientSocket::Smsg()
{
	
	//creates message for the server
	Message* msg = new Message(VERSION, MSG_CODE);
	int err;
	std::string name;

	std::cout << "Enter Name: ";
	getline(std::cin, name);

	Contact* ct = this->findContact(name);
	if (ct == nullptr)
	{
		std::cout << "ERROR: contact not found" << std::endl;
		return 0;
	}

	TxtMessage* txtmsg = new TxtMessage(SEND_MSG);
	txtmsg->setID(ct->getID());

	if (!ct->isSymetricKey())
	{
		std::cout << "ERROR: symetric key not found" << std::endl;
		return 0;
	}
	std::string symKey = ct->getSymetrickey();

	std::cout << "Enter message: " << std::endl;

	std::string txt = "", buffer = "";
	getline(std::cin, buffer);
	while (buffer != "")
	{
		txt += buffer + '\n';
		getline(std::cin, buffer);
	}


	


	AESWrapper aes((unsigned char *)symKey.c_str(), AESWrapper::DEFAULT_KEYLENGTH);

	// 2. encrypt a message (plain text)
	std::string cipher = aes.encrypt(txt.c_str(), txt.length());


	txtmsg->setPayload(cipher);

	msg->setPayload(txtmsg->to_string());
	msg->setID(this->id);

	//send message to server & handle error
	this->ClientSend(msg->to_string(), &err);
	if (err == SOCKET_ERROR)
	{
		std::cout << "could not send to server";
		delete msg;
		return 1;
	}

	Message* RecMsg = this->ClientRecv(&err);
	if (err == SOCKET_ERROR)
	{
		std::cout << "could not recv from server";
		delete msg;
		return 1;
	}

	if (RecMsg->Code == MSG_REC)
	{
		std::cout << "Message succesfully recieved" << std::endl;

	}


	//handle closed data
	delete RecMsg;
	delete msg;
	delete txtmsg;
	return 0;


}






int ClientSocket::ReWmsg()
{
	//creates message for the server
	Message* msg = new Message(VERSION, PULL_CODE);
	int err;
	std::string name;

	

	msg->setPayload("");
	msg->setID(this->id);

	//send message to server & handle error
	this->ClientSend(msg->to_string(), &err);
	if (err == SOCKET_ERROR)
	{
		std::cout << "could not send to server";
		delete msg;
		return 1;
	}



	Message* RecMsg = this->ClientRecv(&err);
	if (err == SOCKET_ERROR)
	{
		std::cout << "could not recv from server";
		delete msg;
		return 1;
	}

	int curPos = 0;
	int len = RecMsg->payload.length();
	if (RecMsg->Code == PULL_REC)
	{
		while (curPos < len)
		{
			std::string header = RecMsg->payload.substr(curPos, PULL_MSG_HEADER_LEN);
			curPos += PULL_MSG_HEADER_LEN;
			PullMessage pullMsg = PullMessage(header);
			std::string content = RecMsg->payload.substr(curPos, pullMsg.PayloadSize);
			pullMsg.setPayload(content);
			switch (pullMsg.msgType)
			{
				case SYM_REQ:
					this->handleReqSym(pullMsg);
					break;
				case SEND_SYM:
					this->handleSendSym(pullMsg);
					break;
				case SEND_MSG:
					this->handleSendTxt(pullMsg);
					break;


			}
			curPos += pullMsg.PayloadSize;
		}


	}


	//handle closed data
	delete RecMsg;
	delete msg;
	return 0;





}


void ClientSocket::handleReqSym(PullMessage pullMsg)
{
	Contact* ct = this->findContact(pullMsg.id);
	if (ct == nullptr)
	{
		std::cout << "From: Unknowm User, Message didnt get Recieved" << std::endl;
		return;
	}
	std::cout << "From: " << ct->getName() << std::endl;
	std::cout << "Content: "  << std::endl;
	std::cout << "Request for symetric key" << std::endl;
	std::cout << "-----------<EOM>---------"  << std::endl;

}

void ClientSocket::handleSendSym(PullMessage pullMsg)
{
	Contact* ct = this->findContact(pullMsg.id);
	if (ct == nullptr)
	{
		std::cout << "From: Unknowm User, Message didnt get Recieved" << std::endl;
		return;
	}

	std::string cipher = pullMsg.payload;

	RSAPrivateWrapper rsapriv_other(Base64Wrapper::decode(this->privateKey));
	std::string symKey = rsapriv_other.decrypt(cipher);
	ct->setSymetrickey(symKey);



	std::cout << "From: " << ct->getName() << std::endl;
	std::cout << "Content: " << std::endl;
	std::cout << "symetric key received" << std::endl;
	std::cout << "-----------<EOM>---------" << std::endl;


}

void ClientSocket::handleSendTxt(PullMessage pullMsg)
{
	Contact* ct = this->findContact(pullMsg.id);
	if (ct == nullptr)
	{
		std::cout << "From: Unknowm User, Message didnt get Recieved" << std::endl;
		return;
	}

	std::string cipher = pullMsg.payload;
	std::string symKey;
	

	


	std::cout << "From: " << ct->getName() << std::endl;
	std::cout << "Content: " << std::endl;

	if (!ct->isSymetricKey())
	{
		std::cout << "cant decrypr message" << std::endl;
		return;
	}

	symKey = ct->getSymetrickey();
	AESWrapper aes((unsigned char*)symKey.c_str(), AESWrapper::DEFAULT_KEYLENGTH);

	// 2. encrypt a message (plain text)
	std::string decrypt = aes.decrypt(cipher.c_str(), cipher.length());

	std::cout << decrypt;
	std::cout << "-----------<EOM>---------" << std::endl;


}













