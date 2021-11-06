#include "PullMessage.h"

PullMessage::PullMessage(std::string msg)
{
	std::string id_s = msg.substr(0, 16);
	memcpy(this->id.uuid, id_s.c_str(), UUID_SIZE);
	this->ID = fourBytesToInteger(msg.substr(16, 4));
	this->msgType = msg[20];
	this->PayloadSize = fourBytesToInteger(msg.substr(21, 4));
	this->payload = msg.substr(25);


}

void PullMessage::setPayload(std::string payload)
{
	this->payload = payload;

}




