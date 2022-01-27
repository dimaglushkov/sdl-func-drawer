#ifndef SDL_FUNC_DRAWER_SDL_FUNC_DRAWER_H
#define SDL_FUNC_DRAWER_SDL_FUNC_DRAWER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_events.h"
#include "stdio.h"
#include <stdlib.h>
#include <time.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int w, h, num;
    double x_min, x_max, y_max, y_min, cx, cy, step, y_limit;
    char quadrants[4];
} drawer_t;

struct color_set {
    SDL_Color white, black, red, green, blue;
};

extern const struct color_set colors;

typedef double (*f)(double);

drawer_t* SFD_init(int w, int h, double x_min, double x_max, double y_min, double y_max, int n);
void SFD_draw(drawer_t* dr, f func[]);
void SFD_close(drawer_t* dr);


#endif //SDL_FUNC_DRAWER_SDL_FUNC_DRAWER_H
