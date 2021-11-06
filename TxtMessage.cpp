#include "TxtMessage.h"

TxtMessage::TxtMessage(unsigned char msgType)
{
	this->msgType = msgType;
	memset(id.uuid, 0, UUID_SIZE);


}

void TxtMessage::setID(uuid id)
{
	memcpy((this->id).uuid, id.uuid, UUID_SIZE);
}


std::string TxtMessage::to_string()
{
	std::string msgTypeStr, pSizeStr, idStr;
	std::string finaleMsg;
	msgTypeStr = unsigned char(this->msgType);
	pSizeStr = ito4s(this->PayloadSize);
	idStr = uuidToStr(this->id);

	finaleMsg = idStr + msgTypeStr + pSizeStr + payload;
	return finaleMsg;


}

void  TxtMessage::setPayload(std::string data)
{
	this->PayloadSize = data.length();
	this->payload = data;


}