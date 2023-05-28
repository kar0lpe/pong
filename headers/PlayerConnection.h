#pragma once
#include <string>
#include <boost/asio.hpp>
#include "Connection.h"
#include <boost/lockfree/spsc_queue.hpp>
#include "SerializedMessage.h"

using namespace boost::asio;

class PlayerConnection : public Connection
{
public:
	PlayerConnection(int id, io_context& pio_context,
					 boost::lockfree::spsc_queue<SerializedMessage>& qIn,
					 boost::lockfree::spsc_queue<SerializedMessage>& qOut) : playerId(id), socket(pio_context), msgQ_In(qIn), msgQ_Out(qOut) {}
	void handleConnection() override;
	void stopConnection() override;
	void sendMessage(SerializedMessage& msg) override;
	ip::tcp::socket& getSocket() { return socket; }
	
private:
	void writeToSocket(const std::string & msg);
	int playerId;
	void readRequest();
	void handleRequest();
	ip::tcp::socket socket;
	char bufor[1024];
	bool ongoing;
	int readed;
	boost::lockfree::spsc_queue<SerializedMessage>& msgQ_In;
	boost::lockfree::spsc_queue<SerializedMessage>& msgQ_Out;
};