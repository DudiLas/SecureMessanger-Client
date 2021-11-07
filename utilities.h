#ifndef UTILITIES
#define UTILITIES
#include <string>
#include <fstream>
#include "AESWrapper.h"
#include "Base64Wrapper.h"
#include "RSAWrapper.h"

#define PRIVATE_KEY_LEN 856

#define SIZEOFBYTE 8
#define VERSION 1
#define MAX_WORD_LEN 255
#define UUID_SIZE 16
#define PUBLIC_KEY_SIZE 160
#define HEADER_LEN 7
#define HEX_UUID_SIZE 32
#define PULL_MSG_HEADER_LEN 25

//request
#define REGISTER_CODE 1000
#define RECEIVE_LIST_CODE 1001
#define PUBLIC_KEY_CODE 1002
#define MSG_CODE 1003
#define PULL_CODE 1004

//messages 
#define SYM_REQ 1
#define SEND_SYM 2
#define SEND_MSG 3

//response 
#define SUC_REGISTER  2000
#define CLIENT_LIST 2001
#define PUBLIC_REQ 2002
#define MSG_REC 2003
#define PULL_REC 2004
#define ERROR  9000

typedef struct uuid { char uuid[16]; }uuid;
int fileExists(std::string name);
std::string ito4s(unsigned int num);
std::string sto2s(unsigned short num);
void fillWithBytes(std::string & data, int nBytes);
std::string uuidToStr(uuid id);
std::string chTostr(const char* ch, int len);
std::string chTostr(const unsigned char* ch, int len);
int fourBytesToInteger(std::string bytesNum);
int twoBytesToInteger(std::string bytesNum);
std::string string_to_hex(const std::string& input);
std::string hex_to_string(const std::string & input);



#endif