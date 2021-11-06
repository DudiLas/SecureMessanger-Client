#ifndef PULL_MESSAGE 
#define PULL_MESSAGE
#include "utilities.h"

class PullMessage
{
public:
	uuid id;
	unsigned int ID;
	unsigned char msgType;
	unsigned int PayloadSize;
	std::string payload;


	PullMessage(std::string msg);
	void setPayload(std::string payload);
	



};
#endif