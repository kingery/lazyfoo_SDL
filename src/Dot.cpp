#include "Dot.hh"

Dot::Dot() {
	mPosX = 0;
	mPosY = 0;
	
	mVelX = 0;
	mVelY = 0;
	
	mCollider.w = DOT_WIDTH;
	mCollider.h = DOT_HEIGHT;
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

void Dot::move(SDL_Rect& wall, int maxXpos, int maxYpos) {
	mPosX += mVelX;
	mPosY += mVelY;
	mCollider.x = mPosX;
	mCollider.y = mPosY;
	
	bool collision = checkCollision(wall);
	
	if ((mPosX < 0) || (mPosX + DOT_WIDTH >= maxXpos) || collision) {
		mPosX -= mVelX;
		mCollider.x = mPosX;
	}
	
	if ((mPosY < 0) || (mPosY + DOT_HEIGHT >= maxYpos) || collision) {
		mPosY -= mVelY;
		mCollider.y = mPosX;
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

bool Dot::checkCollision(SDL_Rect& wall) {
	// bounding box collision
	int leftDot, leftWall;
	int rightDot, rightWall;
	int topDot, topWall;
	int bottomDot, bottomWall;
	
	// set values for dot
	leftDot = mCollider.x;
	rightDot = mCollider.x + mCollider.w;
	topDot = mCollider.y;
	bottomDot = mCollider.y + mCollider.h;
	
	// set values for wall
	leftWall = wall.x;
	rightWall = wall.x + wall.w;
	topWall = wall.y;
	bottomWall = wall.y + wall.h;
	
	// check if dot is outside wall
	if (topDot >= bottomWall
		|| bottomDot <= topWall
		|| leftDot >= rightWall
		|| rightDot <= leftWall) {
			return false;
	}
	
	return true;
}