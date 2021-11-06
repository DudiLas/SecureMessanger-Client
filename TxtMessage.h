#ifndef TEXT_MESSAGE 
#define TEXT_MESSAGE
#include "utilities.h"

class TxtMessage
{
public:
	uuid id;
	unsigned char msgType;
	unsigned int PayloadSize;
	std::string payload;


	TxtMessage(unsigned char msgType);
	void setPayload(std::string data);
	void setID(uuid id);
	std::string to_string();



};
#endif