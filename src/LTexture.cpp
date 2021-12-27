#include "LTexture.hh"
#include <stdio.h>
#include <string>
#include <SDL2/SDL_image.h>

LTexture::LTexture() {
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture() {
    free();
}

bool LTexture::loadFromFile(std::string path, SDL_Renderer* renderer) {
    free(); // clear out pre-existing texture

    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s. SDL_image error: %s\n", path.c_str(), IMG_GetError());
    } else {
        // color key the image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xff, 0xff));
        // create texture from surface
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
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

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* font, SDL_Renderer* renderer) {
    free();

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
    if (textSurface == NULL) {
        printf("unable to render text surface :( error: %s\n", TTF_GetError());
    } else {
        mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (mTexture == NULL) {
            printf("unable to create texture from rendered text! error: %\n", SDL_GetError());
        } else {
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        SDL_FreeSurface(textSurface);
    }

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

void LTexture::setBlendMode(SDL_BlendMode blending) {
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Renderer* renderer, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    // set clip rendering dimensions
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeight() {
    return mHeight;
}