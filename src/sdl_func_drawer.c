#include "sdl_func_drawer.h"

const struct color_set colors = {
        {255,255,255,255},
        {0,0,0,255},
        {255,0,0,255},
        {0,255,0,255},
        {0,0,255,0}
};

SDL_Color random_color();
int write_info(drawer_t* dr);
int precalculate(drawer_t* dr, f* func);
int draw_xoy(drawer_t* dr);
int draw_func(drawer_t * dr, double (*func)(double), SDL_Color color);
int draw_func_legend(drawer_t * dr, int k, SDL_Color color);


drawer_t* SFD_init(int w, int h, double x_min, double x_max, double y_min, double y_max, int n) {
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
    dr->num = n;
    dr->x_min = x_min;
    dr->x_max = x_max;
    dr->y_min = y_min;
    dr->y_max = y_max;

    if (SDL_CreateWindowAndRenderer(dr->w, dr->h, 0, &dr->window, &dr->renderer)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        SFD_close(dr);
        return NULL;
    }

    return dr;
}

void SFD_draw(drawer_t* dr, f func[]) {
    SDL_Event event;
    SDL_SetRenderDrawColor(dr->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(dr->renderer);

    precalculate(dr, func);
    draw_xoy(dr);
    for (int i = 0; i < dr->num; i++) {
        SDL_Color color = random_color();
        draw_func(dr, func[i], color);
        draw_func_legend(dr, i, color);
    }
    write_info(dr);

    SDL_RenderPresent(dr->renderer);
    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            break;
        }
    }

}

int draw_func_legend(drawer_t * dr, int k, SDL_Color color){
    TTF_Init();
    char buffer[10];
    TTF_Font *font = TTF_OpenFont("assets/noto-mono.ttf", 16);
    if (font == NULL) {
        SDL_Log("Unable to open font: %s", SDL_GetError());
        TTF_Quit();
        return 1;
    }
    sprintf(buffer, "func %d", k + 1);
    SDL_Surface* t_surface = TTF_RenderText_Solid(font, buffer, color);
    SDL_Texture* t_texture = SDL_CreateTextureFromSurface(dr->renderer, t_surface);
    SDL_Rect t_rect = {10, dr->h - 10 - t_surface->h * (k + 1), t_surface->w, t_surface->h };
    SDL_RenderCopy(dr->renderer, t_texture, NULL, &t_rect);

    SDL_SetRenderDrawColor(dr->renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(dr->renderer, 20 + t_rect.w, t_rect.y + t_rect.h / 2, 20 + t_rect.w + 40, t_rect.y + t_rect.h / 2);

    SDL_RenderPresent(dr->renderer);

    SDL_FreeSurface(t_surface);
    SDL_DestroyTexture(t_texture);
    TTF_CloseFont(font);
    TTF_Quit();
    return 0;
}


int write_info(drawer_t* dr) {
    TTF_Init();
    char buffer[50], quad_buffer[10];
    TTF_Font *font = TTF_OpenFont("assets/noto-mono.ttf", 15);
    if (font == NULL) {
        SDL_Log("Unable to open font: %s", SDL_GetError());
        TTF_Quit();
        return 1;
    }
    SDL_Color text_color = colors.green;

    sprintf(buffer, "x lies in [%.3f, %.3f]", dr->x_min, dr->x_max);
    SDL_Surface* t_surface = TTF_RenderText_Solid(font, buffer, text_color);
    SDL_Texture* t_texture = SDL_CreateTextureFromSurface(dr->renderer, t_surface);
    SDL_Rect x_rect = {10, 10, t_surface->w, t_surface->h };
    SDL_RenderCopy(dr->renderer, t_texture, NULL, &x_rect);
    buffer[0] = '\0';

    sprintf(buffer, "y lies in [%.3f, %.3f]", dr->y_min, dr->y_max);
    t_surface = TTF_RenderText_Solid(font, buffer, text_color);
    t_texture = SDL_CreateTextureFromSurface(dr->renderer, t_surface);
    SDL_Rect y_rect = {10, x_rect.y + x_rect.h + 2, t_surface->w, t_surface->h };
    SDL_RenderCopy(dr->renderer, t_texture, NULL, &y_rect);
    buffer[0] = '\0';

    for (int i = 0, j = 0; i < 4; i++){
        if(dr->quadrants[i]) {
            sprintf(&quad_buffer[j++], "%d", i+1);
            quad_buffer[j++] = ' ';
        }
        quad_buffer[j] = '\0';
    }
    sprintf(buffer, "quadrants: %s", quad_buffer);
    t_surface = TTF_RenderText_Solid(font, buffer, text_color);
    t_texture = SDL_CreateTextureFromSurface(dr->renderer, t_surface);
    SDL_Rect quad_rect = {10, y_rect.y + y_rect.h + 2, t_surface->w, t_surface->h };
    SDL_RenderCopy(dr->renderer, t_texture, NULL, &quad_rect);

    SDL_RenderPresent(dr->renderer);

    SDL_FreeSurface(t_surface);
    SDL_DestroyTexture(t_texture);
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

int draw_func(drawer_t * dr, double (*func)(double), SDL_Color color) {
    double y;
    SDL_SetRenderDrawColor(dr->renderer, color.r, color.g, color.b, color.a);

    for (double x = dr->x_min; x <= dr->x_max; x += dr->step) {
        y = func(x);
        if (isnan(y))
            continue;
        if (fabs(y) > dr->y_limit)
            y = dr->y_limit;
        int a = floor((x + fabs(dr->x_min)) * dr->cx);
        int b = floor((fabs(dr->y_max) - y) * dr->cy);
        SDL_RenderDrawPoint(dr->renderer, floor((x - dr->x_min) * dr->cx), floor((fabs(dr->y_max) - y) * dr->cy) );
    }
    SDL_RenderPresent(dr->renderer);

}

int precalculate(drawer_t* dr, f* func) {
    double i = dr->x_min;
    double val;

    // find optimal step
    dr->step = (fabs(dr->x_min) + fabs(dr->x_max)) / (dr->w * 5);

    // ind optimal y limit
    dr->y_limit = (fabs(dr->x_min) + fabs(dr->x_max)) * 10;

    if (!dr->y_min && !dr->y_max) {
    // find initial ymin ymax values
        do {
            dr->y_max = dr->y_min = func[0](i);
            i += dr->step;
        } while (isnan(dr->y_min) || (isinf(dr->y_min)) || dr->y_max > dr->y_limit);

        // find actual ymin ymax  values
        for (int k = 0; k < dr->num; k++) {
            for (double x = i; x <= dr->x_max; x += dr->step) {
                val = func[k](x);
                if (isnan(val))
                    continue;
                if (val > dr->y_limit)
                    val = dr->y_limit;
                if (val < -1 * dr->y_limit)
                    val = -1 * dr->y_limit;

                if (val > dr->y_max)
                    dr->y_max = val;
                if (val < dr->y_min)
                    dr->y_min = val;
            }
            i = dr->x_min;
        }
    }
    // find graph quadrants
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

    // calculate conversion coefficients
    dr->cx = dr->w / (fabs(dr->x_max - dr->x_min));
    dr->cy = dr->h / (fabs(dr->y_max - dr->y_min));
}

SDL_Color random_color() {
    SDL_Color color = {rand() % 175 + 50, rand() % 205 + 50, rand() % 205 + 50, 255};
    return color;
}


void SFD_close(drawer_t* dr) {
    if (dr->renderer) {
        SDL_DestroyRenderer(dr->renderer);
    }
    if (dr->window) {
        SDL_DestroyWindow(dr->window);
    }
    SDL_Quit();
    free(dr);
}

