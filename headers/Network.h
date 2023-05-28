#pragma once
class Network
{
public:
	virtual bool start() = 0;
	virtual void stop() = 0;
	virtual ~Network() = 0;
};

