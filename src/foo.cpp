#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyPressSurface {
    kpsDEFAULT,
    kpsUP,
    kpsDOWN,
    kpsLEFT,
    kpsRIGHT,
    kpsTOTAL
};

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
SDL_Surface* gKeyPressSurfaces[kpsTOTAL];
SDL_Surface* gCurrentSurface = NULL;

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
            gCurrentSurface = gKeyPressSurfaces[kpsDEFAULT];

            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    } else if (e.type == SDL_KEYDOWN) {
                        switch( e.key.keysym.sym) {
                            case SDLK_UP:
                                gCurrentSurface = gKeyPressSurfaces[kpsUP];
                                break;

                            case SDLK_DOWN:
                                gCurrentSurface = gKeyPressSurfaces[kpsDOWN];
                                break;

                            case SDLK_LEFT:
                                gCurrentSurface = gKeyPressSurfaces[kpsLEFT];
                                break;

                            case SDLK_RIGHT:
                                gCurrentSurface = gKeyPressSurfaces[kpsRIGHT];
                                break;

                            default:
                                gCurrentSurface = gKeyPressSurfaces[kpsDEFAULT];
                                break;
                        }
                    }
                }
                // Apply image to surface
                SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
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
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
}

bool loadMedia() {
    bool success = true;


    gKeyPressSurfaces[kpsDEFAULT] = loadSurface("press.bmp"); 
    if (gKeyPressSurfaces[kpsDEFAULT] == NULL) {
        printf("Failed to load default image!\n");
        success = false;
    }
    gKeyPressSurfaces[kpsUP] = loadSurface("up.bmp");
    if (gKeyPressSurfaces[kpsUP] == NULL) {
        printf("Failed to load up image!\n");
        success = false;
    }
    gKeyPressSurfaces[kpsDOWN] = loadSurface("down.bmp");
    if (gKeyPressSurfaces[kpsDOWN] == NULL) {
        printf("Failed to load down image!\n");
        success = false;
    }

    gKeyPressSurfaces[kpsLEFT] = loadSurface("left.bmp");
    if (gKeyPressSurfaces[kpsLEFT] == NULL) {
        printf("Failed to load left image!\n");
        success = false;
    }

    gKeyPressSurfaces[kpsRIGHT] = loadSurface("right.bmp");
    if (gKeyPressSurfaces[kpsRIGHT] == NULL) {
        printf("Failed to load right image!\n");
        success = false;
    }

    return success;
}

void close() {
    SDL_FreeSurface(gCurrentSurface);
    gCurrentSurface = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

SDL_Surface* loadSurface(string path) {
    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! error: %s\n", path.c_str(), SDL_GetError());
    }

    return loadedSurface;
}
