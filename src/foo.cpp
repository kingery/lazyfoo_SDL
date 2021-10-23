#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Start SDL and create window
bool init();
// loads media
bool loadMedia();
// frees media and shuts down SDL
void close();
// load an image as texture
SDL_Texture* loadTexture(string path);

// global variables :( will address later
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

int main(int argc, char* args[]) {
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;

    if (!init()) {
        printf("Failed initialization\n");
    } else {
        if (!loadMedia()) {
            printf("Failed to load media\n");
        } else {
            bool quit = false;
            SDL_Event e;

            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }

                // clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff); // clear with white backgrounkk
                SDL_RenderClear(gRenderer);

                // render red-filler quad
                SDL_Rect fillRect = {
                    SCREEN_WIDTH / 4,
                    SCREEN_HEIGHT / 4,
                    SCREEN_WIDTH / 2,
                    SCREEN_HEIGHT / 2
                };

                SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0x00, 0xff);
                SDL_RenderFillRect(gRenderer, &fillRect); // filled-in rectangle

                // draw green outlined box
                SDL_Rect outlineRect = {
                    SCREEN_WIDTH / 6,
                    SCREEN_HEIGHT / 6,
                    SCREEN_WIDTH * 2/3,
                    SCREEN_HEIGHT * 2/3
                };

                SDL_SetRenderDrawColor(gRenderer, 0x00, 0xff, 0x00, 0xff);
                SDL_RenderDrawRect(gRenderer, &outlineRect);

                // draw a solid line (1 pixel)
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xff, 0xff);
                SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

                // draw dotten line
                SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0, 0xff); // yellow
                for(int i = 0; i < SCREEN_HEIGHT; i += 4) {
                    SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH / 2, i); // renderer, x pos, y pos
                }

                // update screen
                SDL_RenderPresent(gRenderer);
            }
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
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

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
            }
        }
    }

    return success;
}

bool loadMedia() {
    bool success = true;

    // using geometry rendering. No media to load!
    return success;
}

void close() {
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture(string path) {
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if (loadedSurface == NULL) {
        printf("Failed to load image %s :( error: %s\n", path.c_str(), IMG_GetError());
    } else { // surface loaded! Time to create the texture
        //
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Failed to create texture :( error:; %s\n", SDL_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}
