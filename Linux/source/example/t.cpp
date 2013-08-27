#include "SDL/SDL.h"
#include <string>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
const int SCREEN_BPP = 32; //32-bit color mode

SDL_Surface* message = NULL;
SDL_Surface* screen = NULL;

SDL_Surface* load_image(std::string filename) {
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = SDL_LoadBMP(filename.c_str());
    return loadedImage;
    if (loadedImage != NULL) {
        optimizedImage = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);
    }
    return optimizedImage;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(source, NULL, destination, &offset);
}

int main(int argc, char* args[]) {
    SDL_Surface* hello = NULL;
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        return 1;
    }
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
    if(screen == NULL) {
        return 1;
    }
    SDL_WM_SetCaption("Lesson 3", NULL);
    hello = load_image("hello.bmp");
    apply_surface(10, 0, hello, screen);
    SDL_Flip(screen);
    SDL_Delay(2000);
    SDL_FreeSurface(hello);
    SDL_Quit();
    return 0;
}
