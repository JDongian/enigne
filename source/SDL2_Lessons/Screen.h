#ifndef SCREEN_H
#define SCREEN_H

#include <SDL.h>
#include <string>

class Screen
{
public:
	~Screen();

	bool flip_screen();
    void delay_screen(int milliseconds);

    bool get_screen_state() const;
    static ScreenWriter& get_instance();

    SDL_Surface * get_screen() const;
private:
    ScreenWriter();

    void initialize();
    bool m_screenFailure;

    SDL_Surface * m_screen;

};

#endif