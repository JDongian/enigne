#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32; //32-bit color mode

SDL_Surface *ball_sheet = NULL;
SDL_Surface *paddle_sheet = NULL;
SDL_Surface *screen = NULL;

SDL_Event event;

void apply_surface(int x, int y,
                   SDL_Surface* source,
                   SDL_Surface* destination,
                   SDL_Rect* clip = NULL) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(source, clip, destination, &offset);
}

SDL_Surface *load_image(std::string filename) {
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load(filename.c_str());
    if (loadedImage != NULL) {
        optimizedImage = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);
        if (optimizedImage != NULL) {
            SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY,
                            SDL_MapRGB(optimizedImage->format,
                                       0, 0x00, 0x00));
        }
    }
    return optimizedImage;
}

SDL_Surface* load_image_alpha(std::string filename) {
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load(filename.c_str());
    if (loadedImage != NULL) {
        optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
        SDL_FreeSurface(loadedImage);
        if (optimizedImage != NULL) {
            SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY,
                            SDL_MapRGB(optimizedImage->format,
                                       0, 0x00, 0x00));
        }
    }
    return optimizedImage;
}

int init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        return false;
    }
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
                              SCREEN_BPP, SDL_SWSURFACE);
    if(screen == NULL) {
        return false;
    }
    /* What does this do? */
    if(SDL_EnableUNICODE(1) == -1) {
        return false;
    }
    SDL_WM_SetCaption("SDL Windows", NULL);
    return true;
}

void clean_up() {
    SDL_FreeSurface(screen);
    SDL_Quit();
}

bool load_files() {
    paddle_sheet = load_image("paddle.png");
    if (paddle_sheet == NULL) {
        return false;
    }
    return true;
}

int main(int argc, char* args[]) {
    bool quit = false;
    if (init() == false) {
        return 1;
    }
    if (load_files() == false) {
        return 1;
    }
    while (!quit) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                /* Hotkey: 'q' for quit. */
                if (event.key.keysym.sym == SDLK_q) {
                    quit = true;
                }
            } else if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        SDL_FillRect(screen, &screen->clip_rect,
                     SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
        if (SDL_Flip(screen) == -1) {
            return 1;
        }
    }
    clean_up();
    return 0;
}
