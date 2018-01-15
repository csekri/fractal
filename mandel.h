/*
draw the mandelbrot
*/

//perform the iterations
long int mandelConverge(complex *c, long int num_it);

//draw the fractal
void drawMandel(display *d, mpfr_t *midx, mpfr_t *midy, long double zoom, bool *exit_, SDL_Event *event, bool isLogisticMap, int colorMethod, zoomData zd);

//put drawing fractal into a loop
void runMandel(display *d, int *changeState);
