#ifndef __DOT_HH
#define __DOT_HH

#include <stdio.h>
#include <SDL2/SDL.h>
#include "LTexture.hh"

class Dot
{
	public:
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;
		
		static const int DOT_VEL = 10;
		
		Dot();
		
		void handleEvent(SDL_Event& e);
		
		void move(int maxXpos, int maxYpos);
		
		void render(LTexture texture, SDL_Renderer* renderer);
		
		int getXpos();
		
		int getYpos();
	
	private:
		int mPosX, mPosY;
		
		int mVelX, mVelY;
};

#endif