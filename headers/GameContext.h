#pragma once
#include "PlayerData.h"
#include <map>

class GameContext
{
public:
	void addNextPlayer(int playerId, float X);
	void removePlayer(int playerId);
	void setGameActive(bool active);
	void updatePaddlePosition(int playerId, float height);
	float getPaddlePosition(int id);
	float getPaddlePositionX(int id);
	void getBallPosition(float& x, float& y);
	float getBallPositionX();
	float getBallPositionY();
	void updateBallPosition(float x, float y);
	bool isGameActive();
	void addPoint(int playerId);
	void updatePoints(int playerId, int points);
	int getPoints(int playerId);
	const float boardX1 = 100.0;
	const float boardX2 = 500.0;
	const float ballSize = 10.0;
	const float paddleSize = 50.0;
private:
	bool gameActive{false};
	std::map<int, PlayerData> players;
	float ballX, ballY;
};