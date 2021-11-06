#ifndef CONTACT
#define CONTACT
#include <stdio.h>
#include <string>
#include "utilities.h"

const int ID_LENGTH = 16;
class Contact
{
	public:
		void setPublickey(std::string & publicKey);
		void setSymetrickey(std::string & symetricKey);
		

		std::string getPublickey();
		std::string getSymetrickey();
		std::string getName();
		uuid & getID();
		int isGood();

		~Contact();
		Contact(uuid&, std::string & name);
		Contact(Contact& ct);

	private:
		std::string * publicKey;
		std::string * symetricKey;
		std::string * name;

		//remember to look over(should be at ram)
		uuid id;
	

};


#endif CONTACT
