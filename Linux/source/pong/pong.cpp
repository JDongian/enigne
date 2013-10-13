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

class Paddle {
    private:
        SDL_Rect clip;
        float x_pos;
        float y_pos;
        float length;
        float width;
        float velocity;
    public:
        Paddle(int x, int y, int len);
        void update_position();
        void handle_events();
        void show();
};

class Ball {
    private:
        SDL_Rect clip;
        float x_pos;
        float y_pos;
        float radius; //= 32
        double x_velocity;
        double y_velocity;
    public:
        Ball(int x, int y, int r, double dx, double dy);
        void update_position(Paddle user);
        void show();
};

Ball::Ball(int x, int y, int r, double dx, double dy) {
    x_pos = x;
    y_pos = y;
    radius = r;
    x_velocity = dx;
    y_velocity = dy;
    clip.x = 0;
    clip.y = 0;
    clip.w = radius*2;
    clip.h = radius*2;
}

void Ball::update_position(Paddle user) {
    /* X position update. */
    if (0 >= x_pos) {
        x_pos = 0.01;
        x_velocity *= -1;
    } else if (x_pos >= SCREEN_WIDTH-radius) {
        x_velocity *= -1;
        x_pos = SCREEN_WIDTH-radius-0.01;
    } else {
        x_pos += x_velocity;
    }
    /* Y position update. */
    if (0 >= y_pos) {
        y_pos = 0.01;
        y_velocity *= -1;
    } else if (y_pos >= SCREEN_HEIGHT-radius) {
        y_velocity *= -1;
        y_pos = SCREEN_HEIGHT-radius-0.01;
    } else {
        y_pos += y_velocity;
    }
    printf("%f, %f\n", x_velocity, y_velocity);
}

Paddle::Paddle(int x, int y, int len) {
    x_pos = x;
    y_pos = y;
    length = 128;
    width = 16;
    velocity = 0;
}

bool Paddle::hit_ball(SDL_Rect position) {
    return 0;
}

void Paddle::update_position() {
    /* Velocity decay function. */
    velocity *= 0.96;
    /* Y position update. */
    y_pos += velocity;
    clip.x = 0;
    clip.y = 0;
    clip.w = width;
    clip.h = length;
}

void Paddle::handle_events() {
    Uint8 *keystates = SDL_GetKeyState(NULL);
    /* UP */
    if (keystates[SDLK_k] || keystates[SDLK_UP]) {
        if (velocity > -3) {
            velocity -= 0.06;
        }
    }
    /* DOWN */
    if (keystates[SDLK_j] || keystates[SDLK_DOWN]) {
        if (velocity < 3) {
            velocity += 0.06;
        }
    }
    if (y_pos <= 0) {
        y_pos = 0;
    } else if (y_pos >= SCREEN_HEIGHT-length) {
        y_pos = SCREEN_HEIGHT-length;
    }
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

void Ball::show() {
    apply_surface(x_pos, y_pos, ball_sheet, screen, &clip);
}

void Paddle::show() {
    apply_surface(x_pos, y_pos, paddle_sheet, screen, &clip);
}

SDL_Surface *load_image (std::string filename) {
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
    //Wat
    if(SDL_EnableUNICODE(1) == -1) {
        return false;
    }
    SDL_WM_SetCaption("SDL Pong", NULL);
    return true;
}

void clean_up() {
    SDL_FreeSurface(ball_sheet);
    SDL_FreeSurface(paddle_sheet);
    SDL_FreeSurface(screen);
    SDL_Quit();
}

bool load_files() {
    ball_sheet = load_image("ball.png");
    paddle_sheet = load_image("paddle.png");
    if (ball_sheet == NULL) {
        return false;
    }
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
    Ball ball1(50, 50, 32, 0.4, 0.4);
    Paddle player_paddle(30, 0, 256);
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
        ball1.update_position(player_paddle); 
        player_paddle.update_position();
        player_paddle.handle_events();
        SDL_FillRect(screen, &screen->clip_rect,
                     SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
        ball1.show();
        player_paddle.show(); 
        if (SDL_Flip(screen) == -1) {
            return 1;
        }
    }
    clean_up();
    return 0;
}
