#include "GameServer.h"
#include <iostream>
#include <list>
#include <boost/lockfree/spsc_queue.hpp>
#include "PlayerData.h"
#include "SerializedMessage.h"
#include "GameContext.h"
#include <time.h>
using namespace std;



bool GameServer::init()
{
	SDL_Init(SDL_INIT_VIDEO);
	
    m_window = SDL_CreateWindow("SDL2 line drawing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
	if (!m_window) 
	{
		cout << "Error creating window: " << SDL_GetError()  << endl;
		return false;
	}
	
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);
	if (!m_renderer)
	{
		cout << "Error creating renderer: " << SDL_GetError() << endl;
		return false;
	}
	
	return true;
}

void GameServer::loop()
{
	GameContext gameContext;
	gameContext.updateBallPosition(250, 100);
	gameContext.addNextPlayer(0, gameContext.boardX1);
	gameContext.updatePaddlePosition(0, 100);
	gameContext.setGameActive(false);


	float velocity = 100;
	SDL_Rect sdlBall;
    sdlBall.x = gameContext.getBallPositionX();
    sdlBall.y = gameContext.getBallPositionY();
    sdlBall.w = gameContext.ballSize;
    sdlBall.h = gameContext.ballSize;
	Uint32 lastTime = SDL_GetTicks();	

	bool quit = false;
    SDL_Event event;
	
	while (!quit)
    {
		Uint32 startTicks = SDL_GetTicks();
		Uint64 startPerf = SDL_GetPerformanceCounter();
        SDL_PollEvent(&event);
		
        SDL_SetRenderDrawColor(m_renderer, 242, 242, 242, 255);
        SDL_RenderClear(m_renderer);

        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
			case SDL_MOUSEMOTION:
				gameContext.updatePaddlePosition(0, event.motion.y);
				break;

        }

		if(!gameContext.isGameActive())
		{
			SerializedMessage msgIn;
			if(msgQ_In.pop(msgIn))
			{
				if(msgIn.playerActive)
				{
					gameContext.addNextPlayer(1, gameContext.boardX2);
					gameContext.updatePaddlePosition(1, 100);
					gameContext.setGameActive(true);	
					
					lastTime = SDL_GetTicks();				
				}
			}
		}
		else
		{
			SerializedMessage msgIn;
			
			float ballPosX, ballPosY;
			ballPosX = gameContext.getBallPositionX();
			
			Uint32 time = SDL_GetTicks();
			float dT = (time - lastTime) / 1000.0f;
			float dX = dT * velocity;
			ballPosX += dX;
			sdlBall.x = ballPosX;
			
			gameContext.updateBallPosition(ballPosX, gameContext.getBallPositionY());
			
			SDL_SetRenderDrawColor( m_renderer, 0, 0, 255, 255 );
			SDL_RenderFillRect( m_renderer, &sdlBall );
			lastTime = time;

			float paddleX, paddleY;
			gameContext.getBallPosition(ballPosX, ballPosY);
			float paddleHeight = gameContext.paddleSize;
			float ballSize = gameContext.ballSize;
			
			if(ballPosX <= gameContext.boardX1)
			{
				paddleY = gameContext.getPaddlePosition(0);
					
				if((ballPosY + ballSize <= paddleY + paddleHeight and ballPosY + ballSize >= paddleY) 
					or (ballPosY >= paddleY and ballPosY <= paddleY + paddleHeight))
				{
					velocity *= -1;
				}
				else
				{
					gameContext.addPoint(1);
					gameContext.updateBallPosition(250, 100);
				}
			}
			else if(ballPosX >= gameContext.boardX2 - ballSize)
			{
				paddleY = gameContext.getPaddlePosition(1);
				if((ballPosY + ballSize <= paddleY + paddleHeight and ballPosY + ballSize >= paddleY) 
					or (ballPosY >= paddleY and ballPosY <= paddleY + paddleHeight))
				{
					velocity *= -1;
				}
				else
				{
					gameContext.addPoint(0);
					gameContext.updateBallPosition(250, 100);

				}
			}
			
			SDL_SetRenderDrawColor(m_renderer, 128, 128, 128, 255);

			for(int i = 0; i < 2; i++)
			{
				paddleX = gameContext.getPaddlePositionX(i);
				paddleY = gameContext.getPaddlePosition(i);
				SDL_RenderDrawLine(m_renderer, paddleX, paddleY, paddleX, paddleY + gameContext.paddleSize);
			}
						
			printPoints(gameContext);
			
			if(msgQ_In.pop(msgIn))
			{
				if(msgIn.playerActive)
				{
					gameContext.updatePaddlePosition(1, msgIn.paddle2Y);
				}
				else
				{
					quit = true;
					break;
				}
			}
			
			SerializedMessage msgOut;

			msgOut.ballX = gameContext.getBallPositionX();
			msgOut.ballY = gameContext.getBallPositionY();
			msgOut.paddleY = gameContext.getPaddlePosition(0);
			msgOut.pointsClient = gameContext.getPoints(1);
			msgOut.pointsServer = gameContext.getPoints(0);
			msgOut.playerActive = true;

			if(!msgQ_Out.push(msgOut))
			{
				std::cout << "Unable to push msg\n";
			}

		}

        SDL_RenderPresent(m_renderer);

		Uint64 endPerf = SDL_GetPerformanceCounter();
		Uint64 framePerf = endPerf - startPerf;

		float elapsedMS = framePerf / (float)SDL_GetPerformanceFrequency() * 1000.0f;

		if(floor(16.666f - elapsedMS) <= 0)
			continue;
		
		SDL_Delay(floor(16.666f - elapsedMS));
    }
	SerializedMessage msgOut;
	msgOut.playerActive = false;
	msgQ_Out.push(msgOut);
	stop();
}

void GameServer::stop()
{
	SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void GameServer::printPoints(GameContext& gameContext)
{
	float x = 20;
	for(int i = 0; i < gameContext.getPoints(0); i++)
	{
		SDL_Rect sdlPoint;
		sdlPoint.x = gameContext.boardX1 + x * i;
		sdlPoint.y = 400;
		sdlPoint.w = gameContext.ballSize;
		sdlPoint.h = gameContext.ballSize;
		SDL_SetRenderDrawColor( m_renderer, 125, 255, 0, 0 );
		SDL_RenderFillRect( m_renderer, &sdlPoint );

	}
	
	for(int i = 0; i < gameContext.getPoints(1); i++)
	{
		SDL_Rect sdlPoint;
		sdlPoint.x = gameContext.boardX2 + x * i;
		sdlPoint.y = 400;
		sdlPoint.w = gameContext.ballSize;
		sdlPoint.h = gameContext.ballSize;
		SDL_SetRenderDrawColor( m_renderer, 125, 255, 0, 0 );
		SDL_RenderFillRect( m_renderer, &sdlPoint );
	}
}