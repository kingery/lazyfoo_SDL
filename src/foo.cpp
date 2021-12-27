#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include "LTexture.hh"

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Start SDL and create window
bool init();
// loads media
bool loadMedia();
// execute game loop
void runGame();
// frees media and shuts down SDL
void close();

// global variables :( will address later
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

TTF_Font *gFont = NULL;
LTexture gTextTexture;
LTexture gButtonSpriteSheetTexture;

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

enum LButtonSprite {
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

class LButton {
	public:
		LButton();
		
		void setPosition(int x, int y);
		void handleEvent(SDL_Event* e);
		void render();
	
	private:
		SDL_Point mPosition;
		LButtonSprite mCurrentSprite;
};

// mouse button sprites
SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];

LButton gButtons[TOTAL_BUTTONS];

LButton::LButton() {
	mPosition.x = 0;
	mPosition.y = 0;
	
	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition(int x, int y) {
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e) {
	// if mouse event happened
	if (e->type == SDL_MOUSEMOTION
		|| e->type == SDL_MOUSEBUTTONDOWN
		|| e->type == SDL_MOUSEBUTTONUP) {
		
		// get mouse postion
		int x, y;
		SDL_GetMouseState(&x, &y);
		
		// check if mouse is inside button
		bool inside = true;
		
		if (x < mPosition.x // mouse left of button
			|| x + BUTTON_WIDTH > mPosition.x // mouse right of button
			|| y < mPosition.y // mouse above button
			|| y + BUTTON_HEIGHT > mPosition.y) { // mouse below button
			inside = false;
		}
		
		if (!inside) {
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		} else {
			// mouse is inside button
			switch (e->type) {
				case SDL_MOUSEMOTION:
					mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
					printf("mouse motion\n");
					break;
				case SDL_MOUSEBUTTONDOWN:
					mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
					printf("mouse button down\n");
					break;
				case SDL_MOUSEBUTTONUP:
					mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
					printf("mouse button up\n");
					break;
			}
		}
	}
}

void LButton::render() {
	gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, gRenderer, &gSpriteClips[mCurrentSprite]);
}

int main(int argc, char* args[]) {
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;

    if (!init()) {
        printf("Failed initialization\n");
    } else {
        if (!loadMedia()) {
            printf("Failed to load media\n");
        } else {
			runGame();
        }
    }

    close();

    return 0;
}

bool init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL shit the bed during initialization :( error: %s\n", SDL_GetError());
        success = false;
    } else {
        gWindow = SDL_CreateWindow(
                "SDL Tutorial, G!",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN
        );

        if (gWindow == NULL) {
            printf("SDL shit the bed during window creation :( error: %s\n", SDL_GetError());
            success = false;
        } else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

            if (gRenderer == NULL) {
                printf("Renderer couldn't init :( error: %s\n", SDL_GetError());
                success = false;
            } else { // renderer is a go!
                SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
                
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image didn't init :( error: %s\n", IMG_GetError());
                    success = false;
                }

                if (TTF_Init() == -1) {
                    printf("SDL_tff could not initialize! error: %s\n", TTF_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia() {
    bool success = true;
    
    // load sprite sheets
	if (!gButtonSpriteSheetTexture.loadFromFile("src/assets/button.png", gRenderer)) {
		printf("failed to load button sprite sheet texture\n");
		success = false;
	} else {
		// set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i) {
			gSpriteClips[i].x = 0;
			gSpriteClips[i].y = i * 200;
			gSpriteClips[i].h = BUTTON_HEIGHT;
			gSpriteClips[i].w = BUTTON_WIDTH;
		}
		
		// set buttons in corners
		gButtons[0].setPosition(0,0);
		gButtons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
		gButtons[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);
		gButtons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
	}
    return success;
}

void runGame() {
	bool quit = false;
	SDL_Event e;

	double degrees = 0;
	SDL_RendererFlip flipType = SDL_FLIP_NONE;

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			
			// handle button events
			for (int i = 0; i < TOTAL_BUTTONS; ++i) {
				gButtons[i].handleEvent(&e);
			}
		}

		//clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(gRenderer);

		// render current frame
		// render buttons
		for (int i = 0; i < TOTAL_BUTTONS; ++i) {
			gButtons[i].render();
		}

		// update screen
		SDL_RenderPresent(gRenderer);
	}
}

void close() {
    // free loaded images
    gTextTexture.free();

    TTF_CloseFont(gFont);
    gFont = NULL;

    // destroy window
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    IMG_Quit();
    SDL_Quit();
}
