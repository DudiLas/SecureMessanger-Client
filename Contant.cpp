#include "Contact.h"
#include <string>

Contact::Contact(uuid & id, std::string& name)
{
	memcpy(this->id.uuid, id.uuid, ID_LENGTH );
	this->name = new std::string(name);
	this->symetricKey = new std::string("");
	this->publicKey = new std::string("");
}
Contact::Contact(Contact& ct)
{
	memcpy(this->id.uuid, ct.getID().uuid, ID_LENGTH);
	this->name = new std::string(ct.getName());
	this->symetricKey = new std::string(ct.getPublickey());
	this->publicKey = new std::string(ct.getSymetrickey());
}

void Contact::setPublickey(std::string &  publicKey)
{
	this->publicKey = new std::string(publicKey);
}
void Contact::setSymetrickey(std::string & symetricKey)
{
	this->symetricKey = new std::string(symetricKey);
}

int Contact::isSymetricKey()
{
	return *symetricKey != "";
}

int Contact::isPublicKey()
{
	return *publicKey != "";
}



std::string Contact::getPublickey()
{
	return *publicKey;
}
std::string Contact::getSymetrickey()
{
	return *symetricKey;
}
std::string Contact::getName()
{
	return *name;
}
uuid & Contact::getID()
{
	return id;
}

Contact::~Contact()
{
	delete publicKey;
	delete symetricKey;
	delete name;
}