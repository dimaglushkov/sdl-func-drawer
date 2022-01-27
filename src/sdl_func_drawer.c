#include "sdl_func_drawer.h"

const struct color_set colors = {
        {255,255,255,255},
        {0,0,0,255},
        {255,0,0,255},
        {0,255,0,255},
        {0,0,255,0}
};

int write_info(drawer_t* dr);
int get_y_min_max(drawer_t* dr, double (*func)(double));
int draw_xoy(drawer_t* dr);


drawer_t* dr_init(int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        SDL_Quit();
        return NULL;
    }
    drawer_t* dr = malloc(sizeof *dr);

    dr->window = NULL;
    dr->renderer = NULL;
    dr->w = w;
    dr->h = h;


    if (SDL_CreateWindowAndRenderer(dr->w, dr->h, 0, &dr->window, &dr->renderer)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        dr_close(dr);
        return NULL;
    }

    return dr;
}


void dr_draw(drawer_t* dr, double (*func)(double), SDL_Color color) {
    SDL_Event event;
    SDL_SetRenderDrawColor(dr->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(dr->renderer);

    get_y_min_max(dr, func);
    write_info(dr);



    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            break;
        }
    }

}

int draw_xoy(drawer_t* dr){

}

int write_info(drawer_t* dr) {
    TTF_Init();
    char buffer[50];
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/TTF/noto-mono.ttf", 15);
    if (font == NULL) {
        SDL_Log("Unable to open font: %s", SDL_GetError());
        TTF_Quit();
        return 1;
    }

    sprintf(buffer, "x lies in [%.3f, %.3f]", dr->x_min, dr->x_max);
    SDL_Surface* TextSurface = TTF_RenderText_Solid(font, buffer, colors.white);
    SDL_Texture* TextTexture = SDL_CreateTextureFromSurface(dr->renderer, TextSurface);
    SDL_Rect XTextRect = {10, 10, TextSurface->w, TextSurface->h };
    SDL_RenderCopy(dr->renderer, TextTexture, NULL, &XTextRect);
    buffer[0] = '\0';

    sprintf(buffer, "y lies in [%.3f, %.3f]", dr->y_min, dr->y_max);
    TextSurface = TTF_RenderText_Solid(font, buffer, colors.white);
    TextTexture = SDL_CreateTextureFromSurface(dr->renderer, TextSurface);
    SDL_Rect YTextRect = {10, 12 + XTextRect.h, TextSurface->w, TextSurface->h };
    SDL_RenderCopy(dr->renderer, TextTexture, NULL, &YTextRect);

    SDL_RenderPresent(dr->renderer);

    SDL_FreeSurface(TextSurface);
    SDL_DestroyTexture(TextTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    return 0;
}

int get_y_min_max(drawer_t* dr, double (*func)(double)){
    double i = dr->x_min;
    double val = func(dr->x_min);

//  find initial values
    do {
        dr->y_max = dr->y_min = func(i);
        i += 0.1;
    } while (isnan(dr->y_min) && i < dr->x_max);

//  find actual values
    while (i < dr->x_max) {
        val = func(i);
        if (isnan(val))
            continue;

        if (val > dr->y_max)
            dr->y_max = val;
        if (val < dr->y_min)
            dr->y_min = val;
        i += 0.1;
    }
}

void dr_close(drawer_t* dr) {
    if (dr->renderer) {
        SDL_DestroyRenderer(dr->renderer);
    }
    if (dr->window) {
        SDL_DestroyWindow(dr->window);
    }
    SDL_Quit();
    free(dr);
}

SDL_Color random_color(){
    SDL_Color color = {rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100, 255};
    return color;
}

