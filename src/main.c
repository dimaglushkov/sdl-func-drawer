#include <math.h>
#include <unistd.h>
#include "sdl_func_drawer.h"

double f1(double x) {
    return 1/x;
}

double f2(double x) {
    return x*x;
}

double f3(double x) {
    return 3*x - 5;
}

double f4(double x) {
    return sqrt(x);
}


int main(int argc, char *argv[]) {
    f func[] = {f1, f2, f3, f4};
    double x_min, x_max;
    if (argc < 3) {
        printf("usage: main.c x_min x_max");
        return 1;
    }
    x_min = strtod(argv[1], NULL);
    x_max = strtod(argv[2], NULL);
    if (x_min >= x_max) {
        return 1;
    }


    drawer_t* dr = dr_init(1800, 1000, sizeof(func) / sizeof (func[0]));
    if (dr == NULL)
        return 1;
    dr->x_max = x_max;
    dr->x_min = x_min;

    dr_draw(dr, func);

    dr_close(dr);
    return 0;
}