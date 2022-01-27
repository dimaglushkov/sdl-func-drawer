#include "sdl_func_drawer.h"

const struct color_set colors = {
        {255,255,255,255},
        {0,0,0,255},
        {255,0,0,255},
        {0,255,0,255},
        {0,0,255,0}
};

const double DX = 0.1;

int write_info(drawer_t* dr);
int precalculate(drawer_t* dr, double (*func)(double));
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

    precalculate(dr, func);
    write_info(dr);
    draw_xoy(dr);

    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            break;
        }
    }

}


int write_info(drawer_t* dr) {
    TTF_Init();
    char buffer[50], quad_buffer[10];
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
    SDL_Rect YTextRect = {10, XTextRect.y + XTextRect.h + 2, TextSurface->w, TextSurface->h };
    SDL_RenderCopy(dr->renderer, TextTexture, NULL, &YTextRect);
    buffer[0] = '\0';

    for (int i = 0, j = 0; i < 4; i++){
        if(dr->quadrants[i]) {
            sprintf(&quad_buffer[j++], "%d", i+1);
            quad_buffer[j++] = ' ';
        }
        quad_buffer[j] = '\0';
    }
    sprintf(buffer, "quadrants: %s", quad_buffer);
    TextSurface = TTF_RenderText_Solid(font, buffer, colors.white);
    TextTexture = SDL_CreateTextureFromSurface(dr->renderer, TextSurface);
    SDL_Rect QuadTextRect = {10, YTextRect.y + YTextRect.h + 2, TextSurface->w, TextSurface->h };
    SDL_RenderCopy(dr->renderer, TextTexture, NULL, &QuadTextRect);

    SDL_RenderPresent(dr->renderer);

    SDL_FreeSurface(TextSurface);
    SDL_DestroyTexture(TextTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    return 0;
}

int draw_xoy(drawer_t* dr) {
    SDL_SetRenderDrawColor(dr->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    if((dr->quadrants[0] && dr->quadrants[3]) || (dr->quadrants[1] && dr->quadrants[2])) {
        SDL_RenderDrawLine(dr->renderer, 0, floor(dr->cy * dr->y_max), dr->w, floor(dr->cy * dr->y_max));
    }

    if((dr->quadrants[0] && dr->quadrants[1]) || (dr->quadrants[2] && dr->quadrants[3])) {
        SDL_RenderDrawLine(dr->renderer, floor(dr->cx * fabs(dr->x_min)), 0, floor(dr->cx * fabs(dr->x_min)), dr->h);
    }
    SDL_RenderPresent(dr->renderer);

}

int precalculate(drawer_t* dr, double (*func)(double)) {
    double i = dr->x_min;
    double val = func(dr->x_min);

//  find initial ymin ymax values
    do {
        dr->y_max = dr->y_min = func(i);
        i += DX;
    } while (isnan(dr->y_min) && i < dr->x_max);

//  find actual ymin ymax  values
    while (i < dr->x_max) {
        val = func(i);
        if (isnan(val))
            continue;

        if (val > dr->y_max)
            dr->y_max = val;
        if (val < dr->y_min)
            dr->y_min = val;
        i += DX;
    }

//  find graph quadrants
    for(int j =0; j < 4; j++)
        dr->quadrants[j] = 0;

    if (dr->x_max > 0 && dr->y_max > 0)
        dr->quadrants[0] = 1;
    if (dr->x_min < 0 && dr->y_max > 0)
        dr->quadrants[1] = 1;
    if (dr->x_min < 0 && dr->y_min < 0)
        dr->quadrants[2] = 1;
    if (dr->x_max > 0 && dr->y_min < 0)
        dr->quadrants[3] = 1;

//  calculate conversion coefficients
    dr->cx = dr->w / (fabs(dr->x_min) + fabs(dr->x_max));
    dr->cy = dr->h / (fabs(dr->y_min) + fabs(dr->y_max));

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

SDL_Color random_color() {
    SDL_Color color = {rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100, 255};
    return color;
}

