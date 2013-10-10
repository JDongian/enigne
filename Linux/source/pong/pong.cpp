#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32; //32-bit color mode

const int CLIP_BASIC = 4;
const int CLIP_MOUSEOVER = 0;
const int CLIP_MOUSEOUT = 1;
const int CLIP_MOUSEDOWN = 2;
const int CLIP_MOUSEUP = 3;

SDL_Surface *background = NULL;
SDL_Surface *buttonSheet = NULL;
SDL_Surface *screen = NULL;

SDL_Event event;

SDL_Rect clips[4];

class Button {
    private:
        SDL_Rect box;
        SDL_Rect* clip;
        float velocity;
        float pos;
    public:
        Button(int x, int y, int w, int h);
        void handle_events();
        void show();
};

void set_clips() {
    clips[CLIP_MOUSEOVER].x = 0;
    clips[CLIP_MOUSEOVER].y = 0;
    clips[CLIP_MOUSEOVER].w = 320;
    clips[CLIP_MOUSEOVER].h = 240;

    clips[CLIP_MOUSEOUT].x = 320;
    clips[CLIP_MOUSEOUT].y = 0;
    clips[CLIP_MOUSEOUT].w = 320;
    clips[CLIP_MOUSEOUT].h = 240;

    clips[CLIP_MOUSEDOWN].x = 0;
    clips[CLIP_MOUSEDOWN].y = 240;
    clips[CLIP_MOUSEDOWN].w = 320;
    clips[CLIP_MOUSEDOWN].h = 240;

    clips[CLIP_MOUSEUP].x = 320;
    clips[CLIP_MOUSEUP].y = 240;
    clips[CLIP_MOUSEUP].w = 320;
    clips[CLIP_MOUSEUP].h = 240;

    clips[CLIP_BASIC].x = 0;
    clips[CLIP_BASIC].y = 0;
    clips[CLIP_BASIC].w = 16;
    clips[CLIP_BASIC].h = 128;
}

Button::Button(int x, int y, int w, int h) {
    box.x = x;
    box.y = y;
    box.w = w;
    box.h = h;
    velocity = 0;
    pos = box.y;
    clip = &clips[CLIP_BASIC];
}

void Button::handle_events() {
    Uint8 *keystates = SDL_GetKeyState(NULL);
    velocity *= 0.94;
    /* UP */
    if(keystates[SDLK_k] || keystates[SDLK_UP]) {
        if (velocity > -3) {
            velocity -= 0.08;
        }
    }
    /* DOWN */
    if(keystates[SDLK_j] || keystates[SDLK_DOWN]) {
        if (velocity < 3) {
            velocity += 0.08;
        }
    }
    pos += velocity;
    box.y = pos+0.5;
    printf("%f\n", velocity);
}

SDL_Surface *load_image (std::string filename) {
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load(filename.c_str());
    if (loadedImage != NULL) {
        optimizedImage = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);
        if (optimizedImage != NULL) {
            SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY,
                            SDL_MapRGB(optimizedImage->format,
                                       0, 0xFF, 0xFF));
        }
    }
    return optimizedImage;
}

void apply_surface(int x, int y,
                   SDL_Surface* source,
                   SDL_Surface* destination,
                   SDL_Rect* clip = NULL) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(source, clip, destination, &offset);
}

void Button::show() {
    SDL_BlitSurface(background, NULL, screen, NULL);
    apply_surface(box.x, box.y, buttonSheet, screen, clip);
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
    //Wat
    if(SDL_EnableUNICODE(1) == -1) {
        return false;
    }
    SDL_WM_SetCaption("TTF Test", NULL);
    return true;
}

void clean_up() {
    SDL_FreeSurface(buttonSheet);
    SDL_FreeSurface(screen);
    SDL_Quit();
}

bool load_files() {
    background = load_image("background.png");
    buttonSheet = load_image("paddle.png");
    if (background == NULL) {
        return false;
    }
    if (buttonSheet == NULL) {
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
    set_clips();
    Button myButton(30, 0, 100, 100);
    while (!quit) {
        myButton.handle_events();
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
                     SDL_MapRGB(screen->format, 0xFF,0xFF, 0xEE));
        myButton.show(); 
        if (SDL_Flip(screen) == -1) {
            return 1;
        }
    }
    clean_up();
    return 0;
}
