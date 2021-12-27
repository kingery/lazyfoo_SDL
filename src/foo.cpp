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
    
    gFont = TTF_OpenFont("src/assets/lazy.ttf", 28);
    if (gFont == NULL) {
        printf("Failed to load font :( error: %s\n", TTF_GetError());
        success = false;
    } else {
        SDL_Color textColor = {0, 0, 0};
        if (!gTextTexture.loadFromRenderedText("Rendered from text!", textColor, gFont, gRenderer)) {
            printf("filad to render text texture :(\n");
            success = false;
        }
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
		}

		//clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(gRenderer);

		// render current frame
		gTextTexture.render((SCREEN_WIDTH - gTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gTextTexture.getHeight()) / 2, gRenderer);

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
