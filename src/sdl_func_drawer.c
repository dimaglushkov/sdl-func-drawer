#include "sdl_func_drawer.h"

const struct color_set colors = {
        {255,255,255,255},
        {0,0,0,255},
        {255,0,0,255},
        {0,255,0,255},
        {0,0,255,0}
};

int write_info(drawer_t* dr);


SDL_Color random_color(){

    SDL_Color color = {rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100, 255};
    return color;
}


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

    SDL_SetRenderDrawColor(dr->renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(dr->renderer, 320, 200, 300, 240);
    SDL_RenderDrawLine(dr->renderer, 300, 240, 340, 240);
    SDL_RenderDrawLine(dr->renderer, 340, 240, 320, 200);
    SDL_RenderPresent(dr->renderer);

    write_info(dr);
    SDL_RenderPresent(dr->renderer);


    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            break;
        }
    }

}

int write_info(drawer_t* dr) {
    TTF_Init();

    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/TTF/noto-mono.ttf", 15);
    if (font == NULL) {
        SDL_Log("Unable to open font: %s", SDL_GetError());
        TTF_Quit();
        return 1;
    }

    SDL_Surface* TextSurface = TTF_RenderText_Solid(font, "Message\nMessage", colors.white);
    SDL_Texture* TextTexture = SDL_CreateTextureFromSurface(dr->renderer, TextSurface);
    SDL_Rect TextRect;
    TextRect.x = 10;
    TextRect.y = 10;
    TextRect.w = TextSurface->w;
    TextRect.h = TextSurface->h;


    SDL_RenderCopy(dr->renderer, TextTexture, NULL, &TextRect);

    SDL_FreeSurface(TextSurface);
    SDL_DestroyTexture(TextTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    return 0;
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