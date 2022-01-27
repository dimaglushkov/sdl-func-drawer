#include <math.h>
#include <unistd.h>
#include "sdl_func_drawer.h"

double func(double x) {
    return 1/x;
}

void print_usage() {
    printf("usage: main.c x_min x_max");
}

int main(int argc, char *argv[]) {
    double x_min, x_max;
    if (argc < 3) {
        print_usage();
        return 1;
    }
    x_min = strtod(argv[1], NULL);
    x_max = strtod(argv[2], NULL);
    if (x_min >= x_max) {
        print_usage();
        return 1;
    }


    drawer_t* dr = dr_init(1000, 1000);
    if (dr == NULL)
        return 1;
    dr->x_max = x_max;
    dr->x_min = x_min;

    dr_draw(dr, func, random_color());

    dr_close(dr);
    return 0;
}