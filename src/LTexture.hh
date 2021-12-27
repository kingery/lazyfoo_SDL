#ifndef LTEXTURE_HH
#define LTEXTURE_HH

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class LTexture
{
    public:
        LTexture();
        ~LTexture();
        bool loadFromFile(std::string path, SDL_Renderer* renderer);
		
		#if defined(SDL_TTF_MAJOR_VERSION)
        bool loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* font, SDL_Renderer* renderer);
        #endif
		
		void free();
        void setColor(Uint8 red, Uint8 green, Uint8 blue);
        void setBlendMode(SDL_BlendMode blending);
        void setAlpha(Uint8 alpha);
        void render(int x, int y, SDL_Renderer* renderer, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
        int getWidth();
        int getHeight();

    private:
        SDL_Texture* mTexture;

        int mHeight;
        int mWidth;
};

#endif