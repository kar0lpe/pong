#pragma once
#include "Network.h"
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <mutex>
#include <vector>
#include <memory>
#include "PlayerConnection.h"
#include <thread>
#include <atomic>
#include "SerializedMessage.h"
#include <boost/lockfree/spsc_queue.hpp>


using namespace boost::asio;
class Server : public Network
{
public:
	Server(boost::lockfree::spsc_queue<SerializedMessage>& pqIn,
		   boost::lockfree::spsc_queue<SerializedMessage>& pqOut)  :m_io_context(), playerConnectionMutex(), msgQ_In(pqIn), msgQ_Out(pqOut){}
	bool start() override;
	void stop() override;
	~Server() override {};
	
private:
	static int nextId;
	void accept_new_player();
	void handle_players();
	io_context m_io_context;
	std::recursive_mutex playerConnectionMutex;
	std::vector<std::shared_ptr<PlayerConnection>> playersConnections;
	std::thread acceptorThread;
	std::atomic<bool> serverRunning{false};
	std::thread handlerPlayersThread;
	boost::lockfree::spsc_queue<SerializedMessage>& msgQ_In;
	boost::lockfree::spsc_queue<SerializedMessage>& msgQ_Out;
};