#pragma once
#include "SerializedMessage.h"

using namespace boost::asio;
class Connection
{
public:
	virtual void handleConnection() = 0;
	virtual void stopConnection() = 0;
	virtual void sendMessage(SerializedMessage& msg) = 0;
	virtual ip::tcp::socket& getSocket() = 0;
};