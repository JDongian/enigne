#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32; //32-bit color mode

SDL_Surface* image = NULL;
SDL_Surface* background = NULL;
SDL_Surface* screen = NULL;

SDL_Event event;

SDL_Surface* load_image(std::string filename) {
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load(filename.c_str());
    if (loadedImage != NULL) {
        optimizedImage = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);
        if (optimizedImage != NULL) {
            //Color key out white (0xFFFFFF)
            Uint32 colorkey = SDL_MapRGB(optimizedImage->format, 0xFF, 0xFF, 0xFF);
            SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorkey);
        }
        return optimizedImage;
    }
    return NULL;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(source, NULL, destination, &offset);
}

int init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        return 1;
    }
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
    if(screen == NULL) {
        return 1;
    }
    SDL_WM_SetCaption("Lesson 4", NULL);
    return 0;
}

bool load_image_resources() {
    image = load_image("fruits.jpg");
    if (image == NULL) {
        return false;
    }
    background = load_image("bkgrd.bmp");
    if (background == NULL) {
        return false;
    }
    return true;
}

void clean_up() {
    SDL_FreeSurface(image);
    SDL_FreeSurface(background);
    SDL_Quit();
}

int main(int argc, char* args[]) {
    bool quit = false;
    if (init() == 1) {
        return 1;
    }
    if (load_image_resources() == false) {
        return 1;
    }
    apply_surface(0, 0, background, screen);
    apply_surface(320, 0, background, screen);
    apply_surface(0, 240, background, screen);
    apply_surface(320, 240, background, screen);
    apply_surface(180, 0, image, screen);
    if (SDL_Flip(screen) == -1) {
        return 1;
    }
    while (quit == false) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
    }
    clean_up();
    return 0;
}
