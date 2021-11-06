#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <WS2tcpip.h>
#include "ClientSocket.h"
#include "Message.h"
#include "Contact.h"

#pragma comment(lib, "ws2_32.lib")




const std::string MESSAGE = "MessageU client at your service.\n\n"
"10)Register\n20)Request for clients list\n"
"30)Request for public key\n"
"40)Request for waiting messages\n"
"50)Send a text message\n"
"51)Send a request for symetric key\n"
"52)Send your symetric key\n"
"0)Exit client\n";


