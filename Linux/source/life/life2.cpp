#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

uint32_t last_frame_time = 0;


void render_text(int x, int y, SDL_Surface *surface,
                 TTF_Font* font, char buff[], SDL_Color c) {
    SDL_Surface* message = NULL;
    int lineskip = TTF_FontLineSkip(font);
    int line = 0;
    int i = 0;
    char* begin = buff;
    while (buff[i]) {
        if (buff[i] == '\n') {
            buff[i++] = '\0';
            message = TTF_RenderText_Blended(font, begin, c);
//            apply_surface(x, y + line++*lineskip,
//                          message,
//                          surface);
            SDL_FreeSurface(message);
            while(buff[i] == '\n') {
                ++i;
            }
            begin = buff+i;
        }
        ++i;
    }
    message = TTF_RenderText_Blended(font, begin, c);
//    apply_surface(x, y + line++*lineskip,
//                  message,
//                  surface);
    SDL_FreeSurface(message);
}

void update_world(int w, int h, uint32_t* pxbuf) {
    for (int i = 0; i < w*h; ++i) {
        pxbuf[i] = 0xAAAAAAFF;
    }
}

void draw_world(int w, int h, uint32_t* pxbuf, SDL_Renderer* r) {
    SDL_Texture* texture = NULL;
    texture = SDL_CreateTexture(r,
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                w, h);
    if (texture == NULL) {
        printf(SDL_GetError());
        return;
    }
    SDL_UpdateTexture(texture, NULL, pxbuf, w*sizeof(uint32_t));
    SDL_RenderClear(r);
    SDL_RenderCopy(r, texture, NULL, NULL);
    SDL_RenderPresent(r);
};

int handle_key_down(SDL_Keysym keysym) {
    switch(keysym.sym) {
        case SDLK_ESCAPE:
            return -1;
            break;
        default:
            break;
    }
    return 0;
}

int process_events(void) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_KEYDOWN:
                return handle_key_down(event.key.keysym);
                break;
            case SDL_QUIT:
                return -1;
                break;
        }
    }
    return 0;
}

int init_sdl(int w, int h, SDL_Window** window, SDL_Renderer** ren) {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        printf(SDL_GetError());
    }
    *window = SDL_CreateWindow("OpenGL Life", 0, 0, w, h,
                               SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf(SDL_GetError());
        return 1;
    }
    *ren = SDL_CreateRenderer(*window, -1,
                              SDL_RENDERER_ACCELERATED |
                              SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        printf(SDL_GetError());
        return 1;
    }
    return 0;
}

int init_bugs(int w, int h, SDL_Window** window, SDL_Renderer** ren) {
}

int main(int argc, char** argv) {
    int width = 640;
    int height = 480;
    bool quit = false;
    SDL_Window* window = NULL;
    SDL_Renderer* ren = NULL;
    static uint32_t* pxbuf = NULL;
    static uint32_t* bug_data = NULL;
    //Initialize game
    pxbuf = new uint32_t[width*height];
    bug_data = new uint32_t[width*height/4];
    memset(pxbuf, 0xFFFFFFFF, width*height); 
    memset(bug_data, 0, width*height/4); 
    
    init_sdl(width, height, &window, &ren);
    printf("SDL successfully initialized.\n");

    while (!quit) {
        last_frame_time = SDL_GetTicks();
        if (process_events() == -1) {
            quit = true;
        }
        update_world(width, height, pxbuf);
        draw_world(width, height, pxbuf, ren);
        printf("FPS: %f  \r", 1000./(SDL_GetTicks()-last_frame_time));
    }
    SDL_Quit();
    return 0;
}
