#pragma once
#include "Network.h"

class Game
{
public:
	Game() {}
	virtual bool init() = 0;
	virtual void loop() = 0;
	virtual void stop() = 0;
};