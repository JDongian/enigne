#ifndef RENDER_H
#define RENDER_H

#include <string>
#include <SDL_image.h>
#include <SDL.h>

class Render
{
public:
	Render(string filename, int x, int y, SDL_Surface * destination);
	~Render();

private:
    SDL_Surface * m_optimizedImage;

    void load_image(string filename);
    void apply_surface(int x, int y, SDL_Surface * source, SDL_Surface * destination);

#endif