#include "Client.h"
#include "SerializedMessage.h"

bool Client::start()
{
	try
	{
		socket.connect(m_endpoint);
		connectionToServer = std::thread(&Client::handleConnection, this);
	}
	catch(boost::system::system_error & err) 
	{
		std::cout << "client stopped because waiting for server" << std::endl;
		return false;
	}
	return true;
}

void Client::handleConnection()
{
	while(clientRunning)
	{
		SerializedMessage msgToSend{};
		if(msgQ_Out.pop(msgToSend))
		{
			boost::asio::streambuf b;
			std::ostream os(&b);
			
			msgToSend.save(os);
			size_t n = socket.send(b.data());
		}
		
		if(socket.available())
		{
			handleRequest();
		}
	}
}
void Client::handleRequest()
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
	
	if(!msgQ_In.push(msg))
		std::this_thread::sleep_for(1s);

}
	
void Client::stop() 
{
	clientRunning = false;
	if(connectionToServer.joinable())
		connectionToServer.join();
}

void Client::writeToSocket(const std::string & msg)
{
	socket.write_some(buffer(msg));
}