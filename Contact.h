#ifndef CONTACT
#define CONTACT
#include <stdio.h>
#include <string>
typedef char uuid_t[16];
class Contact
{
	public:
		void setPublickey(int & publicKey);
		void setSymetrickey(int & symetricKey);
		

		int getPublickey();
		int getSymetrickey();
		std::string getName();
		uuid_t & getID();

		~Contact();
		Contact(uuid_t&, std::string & name);

	private:
		int * publicKey;
		int * symetricKey;
		std::string * name;
		uuid_t uuid;
	

};


#endif CONTACT
