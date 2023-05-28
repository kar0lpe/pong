#include "PlayerConnection.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <boost/lexical_cast.hpp>
#include "SerializedMessage.h"

using namespace std;
void PlayerConnection::handleConnection()
{
	try
	{
		readRequest();
	}
	catch(boost::system::system_error&)
	{
		socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		socket.close();
	}
	
}

void PlayerConnection::sendMessage(SerializedMessage& msg)
{
	boost::asio::streambuf b;
	std::ostream os(&b);
	msg.save(os);

	size_t n = socket.send(b.data());

}
void PlayerConnection::readRequest()
{
	if(socket.available())
	{
		handleRequest();
	}
}

void PlayerConnection::handleRequest()
{
	boost::system::error_code error;
	boost::asio::streambuf buf;

	auto bytes = boost::asio::read(socket, buf, boost::asio::transfer_exactly(70),error);

	ostream oss(&buf);
	std::stringstream ss;
	ss << oss.rdbuf();
	std::string str_data = ss.str();
	
	//cout<<"received "<<bytes<<" bytes"<<endl;
	
	SerializedMessage msg;
	msg.load(str_data);
	msgQ_In.push(msg);
}

PlayerConnection::~PlayerConnection()
{
	socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
	socket.close();
}

void PlayerConnection::writeToSocket(const std::string & msg)
{
	socket.write_some(buffer(msg));
}