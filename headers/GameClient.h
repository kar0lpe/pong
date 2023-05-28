#pragma once
#include "Game.h"
#include "Paddle.h"
#include <SDL2/SDL.h>
#include <boost/lockfree/spsc_queue.hpp>
#include "SerializedMessage.h"
#include "GameContext.h"

class GameClient : public Game
{
public:
	GameClient(Network * network, 
			   boost::lockfree::spsc_queue<SerializedMessage>& pqIn, 
			   boost::lockfree::spsc_queue<SerializedMessage>& pqOut) : msgQ_In(pqIn), msgQ_Out(pqOut) {}
	bool init() override;
	void loop() override;
	void stop() override;
private:
	void printPoints(GameContext& gameContext);
	Network * m_network;
	SDL_Window * m_window;
	SDL_Renderer * m_renderer;
	boost::lockfree::spsc_queue<SerializedMessage>& msgQ_In;
	boost::lockfree::spsc_queue<SerializedMessage>& msgQ_Out;
};