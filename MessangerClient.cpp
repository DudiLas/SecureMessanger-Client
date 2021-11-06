#include "MessangerClient.h"







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
		port = std::stoi(serverInfo.substr(pos + 1));
	}
	catch (...)
	{
		std::cerr << "cant resolve ip:port from server.info file" << std::endl;
		std::exit(1);
	}

	std::string datafileName = "me.info";
	ClientSocket* client = new ClientSocket(ipAddress, port);
	if (!fileExists(datafileName))
	{
		//Create an RSA decryptor. this is done here to generate a new private/public key pair
		RSAPrivateWrapper rsapriv;

		//get the private key and encode it as base64 (base64 in not necessary for an RSA decryptor.)
		std::string base64key = Base64Wrapper::encode(rsapriv.getPrivateKey());
		client->setPrivateKey(base64key);

	}
	else
	{
		std::ifstream serverInfoFile(datafileName);
		std::string name, privateKey = "", id_s;
		char buffer[PRIVATE_KEY_LEN + 1];
		char id_c[HEX_UUID_SIZE];
		uuid id;

		getline(serverInfoFile, name);
		serverInfoFile.read(id_c, HEX_UUID_SIZE);
		id_s = hex_to_string(chTostr(id_c, HEX_UUID_SIZE));
		


		serverInfoFile.read(buffer, PRIVATE_KEY_LEN);
		buffer[PRIVATE_KEY_LEN] = '\0';
		privateKey = std::string(buffer);
		
		memcpy(id.uuid, id_s.c_str(), UUID_SIZE);
		client->setID(id);
		client->setName(name);
		client->setPrivateKey(privateKey);
		serverInfoFile.close();
			



	}






	//used variables
	std::string cm;
	int cmID;






	//Main loop
	while(true)
	{
		std::cout << MESSAGE;
		std::getline(std::cin, cm);
		
		

		//resolve command id 
		cmID = std::stoi(cm);
		if (!cmID)
		{
			std::cout << "ERROR: cant resolve command id, try again\n" << std::endl;
			continue;
		}
		client->HandleMessageID(cmID);

		//route to specific massage type and handle it
		

		
		
		

	}



















	
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



