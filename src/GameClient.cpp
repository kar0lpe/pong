#include "GameClient.h"
#include <iostream>
#include <list>
#include <boost/lockfree/spsc_queue.hpp>
#include "PlayerData.h"
#include "SerializedMessage.h"
#include "GameContext.h"

using namespace std;



bool GameClient::init()
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

void GameClient::loop()
{
	GameContext gameContext;
	gameContext.addNextPlayer(1, gameContext.boardX2);
	gameContext.updatePaddlePosition(1, 100);
	gameContext.setGameActive(false);
	gameContext.updateBallPosition(250, 100);

	Paddle paddlePosition{500, 100, 500, 150};

	SDL_Rect sdlBall;
    sdlBall.x = gameContext.getBallPositionX();
    sdlBall.y = gameContext.getBallPositionY();
    sdlBall.w = 10;
    sdlBall.h = 10;
	
	bool quit = false;
    SDL_Event event;
		Uint32 totalFrameTicks = 0;
	
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
				gameContext.updatePaddlePosition(1, event.motion.y);
				break;
        }

		if(!gameContext.isGameActive())
		{
			gameContext.setGameActive(true);
			gameContext.addNextPlayer(0, gameContext.boardX1);
			gameContext.updatePaddlePosition(0, 150);
			
			SerializedMessage msgOut;
			msgOut.playerActive = true;
			msgQ_Out.push(msgOut);
		}
		else
		{
			SerializedMessage msgIn;
			if(msgQ_In.pop(msgIn))
			{
				if(!msgIn.playerActive)
				{
					quit = true;
					break; 
				}
				gameContext.updateBallPosition(msgIn.ballX, msgIn.ballY);

				gameContext.updatePaddlePosition(0, msgIn.paddleY);
				gameContext.updatePoints(0, msgIn.pointsServer);
				gameContext.updatePoints(1, msgIn.pointsClient);

				sdlBall.x = gameContext.getBallPositionX();
				
				SDL_SetRenderDrawColor( m_renderer, 0, 0, 255, 255 );
				SDL_RenderFillRect( m_renderer, &sdlBall );
				
				SDL_SetRenderDrawColor(m_renderer, 128, 128, 128, 255);

				float paddleClientX, paddleServerX, paddleClientY, paddleServerY;
				paddleClientX = gameContext.getPaddlePositionX(1);
				paddleServerX = gameContext.getPaddlePositionX(0);
				paddleClientY = gameContext.getPaddlePosition(1);
				paddleServerY = gameContext.getPaddlePosition(0);
	
				SDL_RenderDrawLine(m_renderer, paddleClientX, paddleClientY, paddleClientX, paddleClientY + gameContext.paddleSize);

				SDL_RenderDrawLine(m_renderer, paddleServerX, paddleServerY, paddleServerX, paddleServerY + gameContext.paddleSize);
			}
			
			SerializedMessage msgOut;

			msgOut.paddle2Y = gameContext.getPaddlePosition(1);
			msgOut.playerActive = true;

			if(!msgQ_Out.push(msgOut))
			{
				std::cout << "Unable to push msg\n";
			}
			printPoints(gameContext);
		}

        SDL_SetRenderDrawColor(m_renderer, 128, 128, 128, 255);

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

void GameClient::stop()
{
	SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void GameClient::printPoints(GameContext& gameContext)
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