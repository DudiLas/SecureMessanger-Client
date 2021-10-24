#include "Contact.h"
#include <string>

Contact::Contact(uuid_t& uuid, std::string& name)
{
	strcpy_s(this->uuid, uuid);
	this->name = new std::string(name);
}

void Contact::setPublickey(int &  publicKey)
{
	this->publicKey = new int(publicKey);
}
void Contact::setSymetrickey(int & symetricKey)
{
	this->symetricKey = new int(symetricKey);
}



int Contact::getPublickey()
{
	return *publicKey;
}
int Contact::getSymetrickey()
{
	return *symetricKey;
}
std::string Contact::getName()
{
	return *name;
}
uuid_t & Contact::getID()
{
	return uuid;
}

Contact::~Contact()
{
	delete publicKey;
	delete symetricKey;
	delete name;
	delete uuid;
}