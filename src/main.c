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

void print_usage(){
    printf("sdl_func_drawer - app to draw math function's graph. Written on C with SDL2.0\n");
    printf("usage: main.c x_min x_max <?y_min> <?y_max>\n");
    printf("more info at https://github.com/dimaglushkov/sdl-func-drawer");
}

int main(int argc, char *argv[]) {
    f func[] = {f1, f2, f3, f4};
    int f_size = sizeof(func) / sizeof (func[0]);
    double x_min, x_max, y_min = 0, y_max = 0;
    if (argc < 3) {
        print_usage();
        return 1;
    }
    x_min = strtod(argv[1], NULL);
    x_max = strtod(argv[2], NULL);
    if (argc == 5) {
        y_min = strtod(argv[3], NULL);
        y_max = strtod(argv[4], NULL);
    }

    if (x_min >= x_max) {
        print_usage();
        return 1;
    }

    drawer_t* dr = SFD_init(1800, 1000, x_min, x_max, y_min, y_max, f_size);
    if (dr == NULL)
        return 1;

    SFD_draw(dr, func);
    SFD_close(dr);
    return 0;
}