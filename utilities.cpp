#include "utilities.h"


int fileExists(std::string name)
{

	std::ifstream f(name.c_str());
	int i = f.good();
	f.close();
	return i;
	/*
	FILE* file;
	fopen_s(&file, name.c_str(), "r");
	if(!file) {
		fclose(file);
		return 1;
	}
	else {
		return 0;
	}*/

}
// this function turns four byte number writen in a string to an integer
int fourBytesToInteger(std::string bytesNum)
{
	int a = static_cast<int>(static_cast<unsigned char>(bytesNum[3]) << (3 * SIZEOFBYTE) |
		static_cast<unsigned char>(bytesNum[2]) << (2 * SIZEOFBYTE) |
		static_cast<unsigned char>(bytesNum[1]) << SIZEOFBYTE |
		static_cast<unsigned char>(bytesNum[0]));
	return a;
}

// this function turns two byte number writen in a string to an integer
int twoBytesToInteger(std::string bytesNum)
{
	int a = static_cast<int>(static_cast<unsigned char>(bytesNum[1]) << SIZEOFBYTE |
		static_cast<unsigned char>(bytesNum[0]));
	return a;
}
std::string ito4s(unsigned int num)
{
	char byte1, byte2, byte3, byte4;
	std::string strNum;

	byte1 = (unsigned char)(num >> (SIZEOFBYTE * 3));
	byte2 = (unsigned char)(num >> (SIZEOFBYTE * 2));
	byte3 = (unsigned char)(num >> (SIZEOFBYTE));
	byte4 = (unsigned char)num;
	strNum = "xxxx";

	strNum[3] = byte1;
	strNum[2] = byte2;
	strNum[1] = byte3;
	strNum[0] = byte4;

	return strNum;
}

std::string sto2s(unsigned short num)
{
	char byte1, byte2;
	std::string strNum;

	byte1 = (unsigned char)(num >> (SIZEOFBYTE));
	byte2 = (unsigned char)num;
	strNum = "xx";


	strNum[1] = byte1;
	strNum[0] = byte2;

	return strNum;
}

 void fillWithBytes(std::string & data, int nBytes)
{
	 std::string temp = data;
	 temp += '\0';

	for (int i = data.length() + 1 ; i < nBytes ; i++)
	{
		temp += " ";

	}
	
	data = temp;




}


std::string uuidToStr(uuid id)
{
	std::string strID = "";
	for (int i = 0; i < UUID_SIZE; i++)
	{
		strID += id.uuid[i];
	}
	return strID;




}


std::string chTostr(const char* ch, int len)
{
	std::string str = "";
	for (int i = 0; i < len; i++)
	{
		str += ch[i];
	}
	return str;

}

std::string string_to_hex(const std::string& input)
{
	static const char hex_digits[] = "0123456789ABCDEF";

	std::string output;
	output.reserve(input.length() * 2);
	for (unsigned char c : input)
	{
		output.push_back(hex_digits[c >> 4]);
		output.push_back(hex_digits[c & 15]);
	}
	return output;
}
int hex_value(unsigned char hex_digit)
{
	static const signed char hex_values[256] = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
		-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	};
	int value = hex_values[hex_digit];
	if (value == -1) throw std::invalid_argument("invalid hex digit");
	return value;
}

std::string hex_to_string(const std::string& input)
{
	const auto len = input.length();
	if (len & 1) throw std::invalid_argument("odd length");

	std::string output;
	output.reserve(len / 2);
	for (auto it = input.begin(); it != input.end(); )
	{
		int hi = hex_value(*it++);
		int lo = hex_value(*it++);
		output.push_back(hi << 4 | lo);
	}
	return output;
}
