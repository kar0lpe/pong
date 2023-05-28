#include "GameContext.h"

void GameContext::addNextPlayer(int playerId, float paddleX)
{
	PlayerData newPlayer;
	newPlayer.paddleX = paddleX;
	newPlayer.paddleY = 0;
	newPlayer.points = 0;
	players[playerId] = newPlayer;
}

void GameContext::removePlayer(int playerId)
{
}

void GameContext::setGameActive(bool active)
{
	gameActive = active;
}

void GameContext::updatePaddlePosition(int playerId, float y)
{
	players.at(playerId).paddleY = y; //!!
}

float GameContext::getPaddlePosition(int id)
{
	return players.at(id).paddleY;
}
float GameContext::getPaddlePositionX(int id)
{
	return players.at(id).paddleX;
	
}
void GameContext::getBallPosition(float& x, float& y)
{
	x = ballX;
	y = ballY;
}

void GameContext::updateBallPosition(float x, float y)
{
	ballX = x;
	ballY = y;
}

bool GameContext::isGameActive()
{
	return gameActive;
}

void GameContext::addPoint(int playerId)
{
	players.at(playerId).points++; 
}

void GameContext::updatePoints(int playerId, int points)
{
	players.at(playerId).points = points;
}

int GameContext::getPoints(int playerId)
{
	return players.at(playerId).points;
}

float GameContext::getBallPositionX()
{
	return ballX;
}

float GameContext::getBallPositionY()
{
	return ballY;
}
