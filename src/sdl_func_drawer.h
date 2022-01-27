#ifndef SDL_FUNC_DRAWER_SDL_FUNC_DRAWER_H
#define SDL_FUNC_DRAWER_SDL_FUNC_DRAWER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_events.h"
#include "stdio.h"
#include <stdlib.h>
#include <time.h>

struct color_set {
    SDL_Color white, black, red, green, blue;
};

extern const struct color_set colors;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int w, h;
    double x_min, x_max, y_max, y_min, cx, cy, step, y_limit;
    char quadrants[4];
} drawer_t;

SDL_Color random_color();

drawer_t* dr_init(int w, int h);
void dr_draw(drawer_t* dr, double (*func)(double), SDL_Color color);
void dr_close(drawer_t* dr);

#endif //SDL_FUNC_DRAWER_SDL_FUNC_DRAWER_H
