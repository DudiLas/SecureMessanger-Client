#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include "utilities.h"
#include "Contact.h"



class Message
{
	public:
		uuid id;
		unsigned char Version;
		unsigned short Code;
		unsigned int PayloadSize;
		std::string payload;

	
		Message(unsigned char Version, unsigned short Code);
		void setPayload(std::string data);
		void setID(uuid id);
		std::string to_string();



};




#endif