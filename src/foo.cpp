#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[]) {
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize :( SDL_Error: %s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow(
                "SDL Tutorial, yo!",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN
        );

        if (window == NULL) {
            printf("Couldn't create window :( SDL_Error: %s\n", SDL_GetError());
        } else {
            // get window surface
            screenSurface = SDL_GetWindowSurface(window);
            // fill the surface with color (white)
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xf0, 0x0f, 0x00));
            // update the surface
            SDL_UpdateWindowSurface(window);
            // wait before closing
            SDL_Delay(10000);
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
