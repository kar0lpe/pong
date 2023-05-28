#include "Server.h"
#include "GameServer.h"
#include <boost/asio.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include "SerializedMessage.h"

int main(int argc, char** args) 
{
	Network* networkConnection;
	
	boost::lockfree::spsc_queue<SerializedMessage> msgQ_In{1000};
	boost::lockfree::spsc_queue<SerializedMessage> msgQ_Out{1000};
	
	networkConnection = new Server(msgQ_In, msgQ_Out);
	bool result = networkConnection->start();

	Game* game = new GameServer(networkConnection, msgQ_In, msgQ_Out);
	game->init();
	game->loop();

	networkConnection->stop();
	return 0;
}
