#include "SDL/SDL.h"
#include <string>
#include <stdio.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
const int SCREEN_BPP = 32; //32-bit color mode

SDL_Surface* message = NULL;
SDL_Surface* background = NULL;
SDL_Surface* screen = NULL;

SDL_Surface* load_image(std::string filename) {
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = SDL_LoadBMP(filename.c_str());
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
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        return 1;
    }
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
    if(screen == NULL) {
        return 1;
    }
    SDL_WM_SetCaption("Lesson 3", NULL);
    message = load_image("hello.bmp");
    background = load_image("bkgrd.bmp");
    apply_surface(0, 0, background, screen);
    apply_surface(320, 0, background, screen);
    apply_surface(0, 240, background, screen);
    apply_surface(320, 240, background, screen);
    apply_surface(180, 32, message, screen);
    if (SDL_Flip(screen) == -1) {
        return -1;
    }
    SDL_Delay(2000);
    SDL_FreeSurface(message);
    SDL_FreeSurface(background);
    SDL_Quit();
    return 0;
}
