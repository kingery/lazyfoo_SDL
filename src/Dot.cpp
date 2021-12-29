#include "Dot.hh"

Dot::Dot() {
	mPosX = 0;
	mPosY = 0;
	
	mVelX = 0;
	mVelY = 0;
}

void Dot::handleEvent(SDL_Event& e) {
	// if event is keypress, adjust velocity
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
			case SDLK_UP: mVelY -= DOT_VEL; break;
			case SDLK_DOWN: mVelY += DOT_VEL; break;
			case SDLK_LEFT: mVelX -= DOT_VEL; break;
			case SDLK_RIGHT: mVelX += DOT_VEL; break;
		}
	} else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
			case SDLK_UP: mVelY += DOT_VEL; break;
			case SDLK_DOWN: mVelY -= DOT_VEL; break;
			case SDLK_LEFT: mVelX += DOT_VEL; break;
			case SDLK_RIGHT: mVelX -= DOT_VEL; break;
		}
	}
}

void Dot::move(int maxXpos, int maxYpos) {
	mPosX += mVelX;
	mPosY += mVelY;
	
	if ((mPosX < 0) || (mPosX >= maxXpos)) {
		mPosX -= mVelX;
	}
	
	if ((mPosY < 0) || (mPosY >= maxYpos)) {
		mPosY -= mVelY;
	}
}

void Dot::render(LTexture texture, SDL_Renderer* renderer) {
	texture.render(mPosX, mPosY, renderer);
}

int Dot::getXpos() {
	return mPosX;
}

int Dot::getYpos() {
	return mPosY;
}