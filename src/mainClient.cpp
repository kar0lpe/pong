#include "Client.h"
#include "GameClient.h"
#include <boost/asio.hpp>
#include <boost/lockfree/spsc_queue.hpp>
int main(int argc, char** args) 
{
	Network* networkConnection;
	
	boost::lockfree::spsc_queue<SerializedMessage> msgQ_In{1000};
	boost::lockfree::spsc_queue<SerializedMessage> msgQ_Out{1000};
	
	networkConnection = new Client(msgQ_In, msgQ_Out);
	bool result = networkConnection->start();
	if(!result)
		return 1;
	Game* game = new GameClient(networkConnection, msgQ_In, msgQ_Out);
	game->init();
	game->loop();
	
	networkConnection->stop();
	return 0;
}
