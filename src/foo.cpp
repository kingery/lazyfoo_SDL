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
// load individual image
SDL_Surface* loadSurface(string path);

// global variables :( will address later
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gPNGSurface = NULL;

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

                // Apply image to surface
                SDL_BlitScaled(gPNGSurface, NULL, gScreenSurface, NULL);
                // update surface
                SDL_UpdateWindowSurface(gWindow);
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
            // init PNG loading
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags)) {
                printf("SDL_Image failed to init! Error: %s\n", IMG_GetError());
                success = false;
            } else {
                gScreenSurface = SDL_GetWindowSurface(gWindow);
            }
        }
    }

    return success;
}

bool loadMedia() {
    bool success = true;

    // load PNG surface
    gPNGSurface = loadSurface("loaded.png");
    if (gPNGSurface == NULL) {
        printf("Failed to load PNG surface!\n");
        success = false;
    }

    return success;
}

void close() {
    SDL_FreeSurface(gPNGSurface);
    gPNGSurface = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    IMG_Quit();
    SDL_Quit();
}

SDL_Surface* loadSurface(string path) {

    // pointer to optimized surface
    SDL_Surface* optimizedSurface = NULL;

    // load image a specified path
    //SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if (loadedSurface == NULL) {
        printf("Unable to load image %s! error: %s\n", path.c_str(), IMG_GetError());
    } else {
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if (optimizedSurface == NULL) {
            printf("Unable to optimize image %s! Error: %s\n", path.c_str(), SDL_GetError());    
        }
        // get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}
