#include "Message.h"


Message::Message( unsigned char Version, unsigned short Code)
{
	this->Version = Version;
	this->Code = Code;
	memset(id.uuid,  0, UUID_SIZE);
	
	
}

void Message::setID(uuid id)
{
	memcpy((this->id).uuid,  id.uuid, UUID_SIZE);
}


std::string Message::to_string()
{
	std::string verStr, codeStr, pSizeStr, idStr;
	std::string finaleMsg;
	verStr = unsigned char(this->Version);
	codeStr = sto2s(this->Code);
	pSizeStr = ito4s(this->PayloadSize);
	idStr = uuidToStr(this->id);

	finaleMsg = idStr + verStr + codeStr +  pSizeStr +  payload;
	return finaleMsg;


}

void  Message::setPayload(std::string data)
{
	this->PayloadSize = data.length();
	this->payload = data;


}