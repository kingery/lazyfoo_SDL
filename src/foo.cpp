#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class LTexture {
    public:
        LTexture();
        ~LTexture();
        bool loadFromFile(std::string path);
        void free();
        void setColor(Uint8 red, Uint8 green, Uint8 blue);
        void render(int x, int y, SDL_Rect* clip = NULL);
        int getWidth();
        int getHeight();

    private:
        SDL_Texture* mTexture;

        int mHeight;
        int mWidth;
};

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

// scene sprites
SDL_Rect gSpriteClips[4];
LTexture gSpriteSheetTexture;

// Scene textures
LTexture gFooTexture;
LTexture gBackgroundTexture;

// modulated textures
LTexture gModulatedTexture;


LTexture::LTexture() {
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture() {
    free();
}

bool LTexture::loadFromFile(std::string path) {
    free(); // clear out pre-existing texture

    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s. SDL_image error: %s\n", path.c_str(), IMG_GetError());
    } else {
        // color key the image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xff, 0xff));
        // create texture from surface
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("unable to create texture from %s. Error: %s\n", path.c_str(), SDL_GetError());
        } else {
            // successfully loaded
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        SDL_FreeSurface(loadedSurface);
    }

    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::free() {
    if (mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::render(int x, int y, SDL_Rect* clip) {
    // create render quad of correct size in correct location
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    // set clip rendering dimensions
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // passing in a SDL_Rect as source rectangle
    // source rectangle defines which part of the texture to render
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeight() {
    return mHeight;
}

int main(int argc, char* args[]) {
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;

    gFooTexture = LTexture();
    gBackgroundTexture = LTexture();

    if (!init()) {
        printf("Failed initialization\n");
    } else {
        if (!loadMedia()) {
            printf("Failed to load media\n");
        } else {
            bool quit = false;
            SDL_Event e;

            Uint8 r = 255;
            Uint8 g = 255;
            Uint8 b = 255;

            int x_pos = 0;
            int y_pos = 0;

            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    } else if (e.type == SDL_KEYDOWN) {
                        switch (e.key.keysym.sym) {
                            // increase red
                            case SDLK_q:
                            r += 32;
                            break;

                            // increase green
                            case SDLK_w:
                            g += 32;
                            break;

                            // increase blue
                            case SDLK_e:
                            b += 32;
                            break;

                            // decrease red
                            case SDLK_a:
                            r -= 32;
                            break;

                            // decrease green
                            case SDLK_s:
                            g -= 32;
                            break;

                            // decrease blue
                            case SDLK_d:
                            b -= 32;
                            break;
                        }
                    }
                }

                //clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
                SDL_RenderClear(gRenderer);

                // modulate and render texture
                gModulatedTexture.setColor(r, g, b);
                gModulatedTexture.render(0, 0);

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

    // Load the foo
    if (!gFooTexture.loadFromFile("assets/foo.png")) {
        printf("Failed to load foo texture image\n");
        success = false;
    }

    if (!gBackgroundTexture.loadFromFile("assets/background.png")) {
        printf("Failed to load background texture image\n");
        success = false;
    }

    // load modulated texture
    if (!gModulatedTexture.loadFromFile("assets/colors.png")) {
        printf("failed to load modulated texture :(\n");
        success = false;
    }

    // load sprite sheet
    if (!gSpriteSheetTexture.loadFromFile("assets/dots.png")) {
        printf("failed to load sprite sheet texture :(\n");
        success = false;
    } else {
        //set top left sprite
        gSpriteClips[0].x = 0;
        gSpriteClips[0].y = 0;
        gSpriteClips[0].w = 100;
        gSpriteClips[0].h = 100;

        //set top right sprite
        gSpriteClips[1].x = 100;
        gSpriteClips[1].y = 0;
        gSpriteClips[1].w = 100;
        gSpriteClips[1].h = 100;

        //set bottom left
        gSpriteClips[2].x = 0;
        gSpriteClips[2].y = 100;
        gSpriteClips[2].w = 100;
        gSpriteClips[2].h = 100;

        //bottom right
        gSpriteClips[3].x = 100;
        gSpriteClips[3].y = 100;
        gSpriteClips[3].w = 100;
        gSpriteClips[3].h = 100;
    }

    return success;
}

void close() {
    // free loaded images
    gFooTexture.free();
    gBackgroundTexture.free();

    // destroy window
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
