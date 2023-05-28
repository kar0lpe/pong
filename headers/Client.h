#pragma once
#include "Network.h"
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <mutex>
#include <vector>
#include <memory>
#include <thread>
#include <string>
#include <iostream>
#include "SerializedMessage.h"


using namespace boost::asio;
const std::string localhostIp = "127.0.0.1";

class Client : public Network
{
public:
	Client(boost::lockfree::spsc_queue<SerializedMessage>& pqIn,
		   boost::lockfree::spsc_queue<SerializedMessage>& pqOut) : m_io_context(),socket(m_io_context),m_endpoint(ip::make_address(localhostIp), 8001), msgQ_In(pqIn), msgQ_Out(pqOut) {}
	bool start() override;
	void stop() override;
	~Client() override {};
private:
	void writeToSocket(const std::string &);
	void handleRequest();
	std::atomic<bool> clientRunning{true};

	void handleConnection();
	io_context m_io_context;
	ip::tcp::socket socket;
	char bufor[1024];
	int readed;
	ip::tcp::endpoint m_endpoint;
	std::thread connectionToServer;
	boost::lockfree::spsc_queue<SerializedMessage> & msgQ_In;
	boost::lockfree::spsc_queue<SerializedMessage> & msgQ_Out;

};