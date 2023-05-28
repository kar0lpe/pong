#include "Server.h"
#include <iostream>
int Server::nextId = 0;

bool Server::start()
{
	try
	{
		serverRunning = true;
		acceptorThread = std::thread(&Server::accept_new_player, this);
		handlerPlayersThread = std::thread(&Server::handle_players, this);
	}
	catch(boost::system::system_error & err) 
	{
		serverRunning = false;
		std::cout << "server stopped" << std::endl;
		return false;
	}
	return true;
}

void Server::accept_new_player()
{
	ip::tcp::acceptor acceptor(m_io_context, ip::tcp::endpoint(ip::tcp::v4(), 8001));
	bool waitingForClientConnection = true;
	while(waitingForClientConnection and serverRunning)
	{
		std::shared_ptr<PlayerConnection> newPlayer = std::make_shared<PlayerConnection>(nextId++, m_io_context, msgQ_In, msgQ_Out);
		
		acceptor.accept(newPlayer->getSocket());
		waitingForClientConnection = false;
		std::lock_guard<std::recursive_mutex> lock(playerConnectionMutex);
		playersConnections.push_back(newPlayer);
	}
}

void Server::handle_players()
{
	while(serverRunning)
	{
		SerializedMessage msgToSend{};
		if(msgQ_Out.pop(msgToSend))
		{
			std::lock_guard<std::recursive_mutex> lock(playerConnectionMutex);
			for(auto it = playersConnections.begin(); it != playersConnections.end(); it++)
			{
				(*it)->sendMessage(msgToSend);
			}
		}

		std::lock_guard<std::recursive_mutex> lock(playerConnectionMutex);
		for(auto it = playersConnections.begin(); it != playersConnections.end(); it++)
		{
			(*it)->handleConnection();
		}
	}
}

void Server::stop()
{
	serverRunning = false;

	if(acceptorThread.joinable())
		acceptorThread.join();

	if(handlerPlayersThread.joinable())
		handlerPlayersThread.join();
}