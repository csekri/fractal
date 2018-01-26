//Mandel drawing routines

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <SDL2/SDL.h>

#include <mpfr.h>

#include "complex.h"
#include "w_h_display.h"
#include "julia.h"


long int juliaConverge(complex *z, long int num_it, complex comp){
		long int i = 0;
		complex c, norm;
		init_complex(&c);
		init_complex(&norm);
		set_complex(&c, comp.r, comp.i);
		/*mpfr_set_d(c.r, -0.8, MPFR_RNDD);
		mpfr_set_d(c.i, 0.176, MPFR_RNDD);*/
		while (i < num_it){
				CMul(z, *z);
				CAdd(z, c);
				set_complex(&norm, z->r, z->i);
				CNorm(&norm);
				i++;
				if (mpfr_cmp_ui(norm.r, 4) > 0) break;
		}
		complex_clear(&c);
		complex_clear(&norm);

		return (i == num_it) ? -1 : i;
}

//the traditional way of coloring the mandelbrot
void mandelColorLayered(long int i, int *r, int *g, int *b){
		*r = (int) (1 + (double) sin(i) / 2) * 255;
		*g = (int) (1 + (double) cos(i) / 2) * 255;
		*b = (int) (1 + (double) sin(i) / 2) * 255;
}

//the smoothened, normalized way
//we need the escape complex number that's why we pass argument in
//isConverge
void mandelColorStriped(complex escape, long int i, int *r, int *g, int *b){
		mpfr_t smooth, log_2;
		mpfr_init(smooth);
		mpfr_init(log_2);
		mpfr_set_ui(log_2, 2, MPFR_RNDD);
		mpfr_log(log_2, log_2, MPFR_RNDD);
		mpfr_set_ui(smooth, i + 1, MPFR_RNDD);
		CMod(&escape);
		mpfr_log(escape.r, escape.r, MPFR_RNDD);
		mpfr_log(escape.r, escape.r, MPFR_RNDD);
		mpfr_div(escape.r, escape.r, log_2, MPFR_RNDD);
		mpfr_add(smooth, escape.r, smooth, MPFR_RNDD);
		long double sm = mpfr_get_ld(smooth, MPFR_RNDD);

		*r = (int) ((sin(sm * 100)) * 255);
		*g = (int) ((sin(sm * 100)) * 255);
		*b = (int) ((cos(sm * 100)) * 255);
		mpfr_clear(smooth);
		mpfr_clear(log_2);
}

void mandelColorSmooth(complex escape, long long int i, int *r, int *g, int *b){
		CMod(&escape);
		mpfr_log(escape.r, escape.r, MPFR_RNDD);
		long double esc = mpfr_get_ld(escape.r, MPFR_RNDD);
		esc = log(esc)/log(2);
		long double smooth = i + 1 - esc;

		*r = (int) ((sin(smooth / 107 + 3.6*3/2) + 1) * 127);
		*g = (int) ((sin(smooth / 103 + 3.6*3/2) + 1) * 127);
		*b = (int) ((cos(smooth / 100) + 1) * 127);
}

void zoomFunc(int mouse_x, int mouse_y, mpfr_t *midx, mpfr_t *midy,
	 mpfr_t left, mpfr_t up, long double *zoom, bool *draw, bool isIn, zoomData zd){

		mpfr_t help1, help2;
		mpfr_init(help1);
		mpfr_init(help2);
		long double ratio_x = (long double) mouse_x / (WIDTH);
		long double ratio_y = (long double) mouse_y / (HEIGHT);
		mpfr_sub(help1, *midx, left, MPFR_RNDD);
		mpfr_sub(help2, up, *midy, MPFR_RNDD);
		mpfr_mul_d(help1, help1, 2, MPFR_RNDD);
		mpfr_mul_d(help2, help2, 2, MPFR_RNDD);
		mpfr_mul_d(help1, help1, ratio_x, MPFR_RNDD);
		mpfr_mul_d(help2, help2, ratio_y, MPFR_RNDD);
		mpfr_add(*midx, left, help1, MPFR_RNDD);
		mpfr_sub(*midy, up, help2, MPFR_RNDD);

		if (isIn == true) *zoom *= zd.newZoom;
		else *zoom /= zd.newZoom;
		*draw = true;

		mpfr_clear(help1);
		mpfr_clear(help2);

}

void zoomInOut(mpfr_t *midx, mpfr_t *midy, mpfr_t left, mpfr_t up, long double *zoom, bool *draw, bool *exit_, zoomData *zd, bool *drawCorrespondingJulia){
		SDL_Event event;
		while (SDL_PollEvent(&event)){
				if (event.type == SDL_MOUSEBUTTONDOWN){
						switch (event.button.button) {
								case SDL_BUTTON_LEFT:
										zoomFunc(event.button.x, event.button.y, midx, midy, left, up, zoom, draw, true, *zd);
								break;
								case SDL_BUTTON_RIGHT:
										zoomFunc(event.button.x, event.button.y, midx, midy, left, up, zoom, draw, false, *zd);
								break;
						}
				}
				if (event.type == SDL_QUIT) *exit_ = true;
				if (event.type == SDL_KEYDOWN){
						int ch = event.key.keysym.sym;
						if (ch == SDLK_LEFT && zd->newZoom > 1.0){
								zd->newZoom -= 0.5;
						}
						else if (ch == SDLK_RIGHT){
								zd->newZoom += 0.5;
						}
						else if (ch == SDLK_UP){
								zd->num_it += 25;
						}
						else if (ch == SDLK_DOWN){
								zd->num_it -= 25;
						}
						else if (ch == 'j'){
								*drawCorrespondingJulia = true;
						}
				}
				if (event.type == SDL_TEXTINPUT)
				{
						SDL_StartTextInput();
						if (event.type == SDL_TEXTINPUT){
								int ch = *event.text.text;
								if (ch == '+'){
										zd->num_it += 25;
								}
								else if (ch == '-' && zd->num_it > 50){
										zd->num_it -= 25;
								}
								SDL_StartTextInput();
						}
				}
				printf("Num it: %d\n", zd->num_it);
				printf("NewZoom: %f\n", zd->newZoom);
				printf("Zoom: %Lf x\n", *zoom);
				mpfr_printf("Real part:  %.60Rf\nImaginary part: %.60Rf\n", *midx, *midy);
				printf("\n\n");

		}
}

void calculateLeftAndUp(mpfr_t *left, mpfr_t *up, mpfr_t midx, mpfr_t midy, long double zoom){
		mpfr_t help1, help2;
		mpfr_init(help1);
		mpfr_init(help2);
		long double screenRatio = (long double) HEIGHT / WIDTH;
		mpfr_set_ld(help1, (long double) 1 / (zoom / 1.5), MPFR_RNDD);
		mpfr_sub(*left, midx, help1, MPFR_RNDD);
		mpfr_set_ld(help2, (long double) screenRatio / (zoom / 1.5), MPFR_RNDD);
		mpfr_add(*up, midy, help2, MPFR_RNDD);
		mpfr_clear(help1);
		mpfr_clear(help2);
}

void drawJulia(display *d, mpfr_t *midx, mpfr_t *midy, long double zoom, bool *exit_, SDL_Event *event, int coloringMethod, zoomData zd, complex comp){
		mpfr_t left, up, help1, help2;
		complex c;
		color colour;
		init_complex(&c);
		mpfr_init(left);
		mpfr_init(up);
		mpfr_init(help1);
		mpfr_init(help2);
		calculateLeftAndUp(&left, &up, *midx, *midy, zoom);

		for (int i = 0; i < WIDTH; i++){
				if (*exit_ == true) break;
				for (int j = 0; j < HEIGHT; j++){
					  mpfr_sub(help1, *midx, left, MPFR_RNDD);
						mpfr_mul_d(help1, help1, 2 * (double) i / WIDTH, MPFR_RNDD);
						mpfr_add(help1, left, help1, MPFR_RNDD);

						mpfr_sub(help2, up, *midy, MPFR_RNDD);
						mpfr_mul_d(help2, help2, 2 * (double) j / HEIGHT, MPFR_RNDD);
						mpfr_sub(help2, up, help2, MPFR_RNDD);

						set_complex(&c, help1, help2);

						long long int n = juliaConverge(&c, zd.num_it, comp);
						//printf("%lld\n", n);
						//complex_printf(c);
						if (n < 0){
								SDL_RenderDrawPoint(d->renderer, i, j);
						} else {
								switch (coloringMethod) {
										case LAYERED:
												mandelColorLayered(n, &colour.r, &colour.g, &colour.b);
										break;
										case SMOOTH:
												mandelColorSmooth(c, n, &colour.r, &colour.g, &colour.b);
										break;
										case STRIPED:
												mandelColorStriped(c, n, &colour.r, &colour.g, &colour.b);
										break;
								}
								SDL_SetRenderDrawColor(d->renderer, colour.r, colour.g, colour.b, 255);
								SDL_RenderDrawPoint(d->renderer, i, j);
								SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255); // yellow
						}
				}
				if (i % 2 == 0){
						SDL_UpdateWindowSurface(d->window);
						while (SDL_PollEvent(event)){
								if (event->type == SDL_QUIT) *exit_ = true;
						}
			  }
		}
		complex_clear(&c);
		mpfr_clear(left);
		mpfr_clear(up);
		mpfr_clear(help1);
		mpfr_clear(help2);
}



void runJulia(display *d, int *changeState, SDL_Event *event){
		long double zoom = 0.6;
		bool draw = true;
		bool exit_ = false;
		zoomData zd = { 1000, 2 };
		complex comp;
		bool notWantedBool = false;
		init_complex(&comp);
		mpfr_set_d(comp.r, -0.7269, MPFR_RNDD);
		mpfr_set_d(comp.i, 0.1889, MPFR_RNDD);

		mpfr_t left, up;
		mpfr_init(left);
		mpfr_init(up);

		mpfr_t midx, midy;
		mpfr_init(midx);
		mpfr_init(midy);
		mpfr_set_d(midx, -0.5, MPFR_RNDD);
		mpfr_set_d(midy, 0, MPFR_RNDD);

		while (! exit_){
				if (draw){
						SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255);
						SDL_UpdateWindowSurface(d->window);
						drawJulia(d, &midx, &midy, zoom, &exit_, event, 2, zd, comp);
						draw = false;
				}
				calculateLeftAndUp(&left, &up, midx, midy, zoom);
				zoomInOut(&midx, &midy, left, up, &zoom, &draw, &exit_, &zd, &notWantedBool);
		}

		*changeState = 1;

		mpfr_clear(midx);
		mpfr_clear(midy);
		mpfr_clear(left);
		mpfr_clear(up);
}



int JuliaMain(){
				mpfr_set_default_prec(100);
				complex c;
				mpfr_t real, imag;
				mpfr_init(real);
				mpfr_init(imag);
				init_complex(&c);

				display *d = malloc(sizeof(display));
				SDL_Init(SDL_INIT_VIDEO);
				int x = SDL_WINDOWPOS_UNDEFINED, y = SDL_WINDOWPOS_UNDEFINED;
				d->window = SDL_CreateWindow("Fractals", x, y, WIDTH, HEIGHT, 0);
				SDL_Surface *surface = SDL_GetWindowSurface(d->window);
				d->renderer = SDL_CreateSoftwareRenderer(surface);
				SDL_Event event;
				SDL_SetRenderDrawBlendMode(d->renderer, SDL_BLENDMODE_BLEND);

				int currentMenuItem;

				SDL_UpdateWindowSurface(d->window);
				SDL_RenderClear(d->renderer);

						runJulia(d, &currentMenuItem, &event);


        SDL_DestroyRenderer(d->renderer);
        SDL_DestroyWindow(d->window);
        SDL_Quit();
    printf("\n");
    return 0;
}
