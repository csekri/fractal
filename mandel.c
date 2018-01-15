//Mandel drawing routines

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <mpfr.h>

#include "complex.h"
#include "font.h"
#include "w_h_display.h"
#include "julia.h"
#include "mandel.h"

long int mandelConverge(complex *c, long int num_it){
		long int i = 0;
		complex z, norm;
		init_complex(&z);
		init_complex(&norm);
		set_complex(&z, c->r, c->i);
		while (i < num_it){
				CMul(&z, z);
				CAdd(&z, *c);
				set_complex(&norm, z.r, z.i);
				CNorm(&norm);
				i++;
				if (mpfr_cmp_ui(norm.r, 4) > 0) break;
		}
		set_complex(c, z.r, z.i);
		complex_clear(&z);
		complex_clear(&norm);

		return (i == num_it) ? -1 : i;
}

void logisticMap(display *d, mpfr_t w, int pixel_x){
		mpfr_t help;
		mpfr_init(help);
		mpfr_set(help, w, MPFR_RNDD);
		if (mpfr_cmp_d(w, 0.25) < 0 && mpfr_cmp_d(w, -2) > 0)
		for (int i = 1; i < 200; i++){
				mpfr_mul(help, help, help, MPFR_RNDD);
				mpfr_add(help, w, help, MPFR_RNDD);
				if (i > 100){
						SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, 255);
						SDL_RenderDrawPoint(d->renderer, pixel_x, HEIGHT / 2 + 100 * mpfr_get_ld(help, MPFR_RNDD));
				}
		}
		mpfr_clear(help);
}

static bool mouseinRect(SDL_Rect rect, int mouse_x, int mouse_y){
	return (rect.x < mouse_x && mouse_x < rect.x + rect.w && rect.y < mouse_y && mouse_y < rect.y + rect.h) ? true : false;
}

void mandelMenu(display *d, bool *isLogisticMap, int *coloringMethod, bool *quit){
text text1, text2, text3, text;
SDL_Event event;
		*coloringMethod = LAYERED;
		TTF_Font *font1, *font2;
		SDL_Rect checkbutton = {100, 450, 30, 30};
		load_TTF_File("COURIER.TTF", &font1, 50);
		load_TTF_File("COURIER.TTF", &font2, 25);
		bool _exit = false;
		render_text(d->renderer, 100, 100, "Choose coloring method:", font1, &text1.texture, &text.rect);
		render_text(d->renderer, 100 + 30, 160, "Traditional coloring", font2, &text1.texture, &text1.rect);
		render_text(d->renderer, 100 + 30, 220, "Smooth coloring", font2, &text2.texture, &text2.rect);
		render_text(d->renderer, 100 + 30, 280, "Coloring with stripes", font2, &text3.texture, &text3.rect);
		render_text2(d->renderer, 100, 340, "Do you want to draw the corresponding Bifurcation diagram?", font1, &text.texture, &text.rect, 1200);
		SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(d->renderer, &checkbutton);
//draw checkbutton
		SDL_UpdateWindowSurface(d->window);

		while (! _exit){
				while (SDL_PollEvent(&event)){
						if (event.type == SDL_KEYDOWN){
								if (event.key.keysym.sym == SDLK_BACKSPACE) {
										*quit = true; _exit = true;
								}
								if (event.key.keysym.sym == SDLK_RETURN) _exit = true;
						}
						if (event.type == SDL_MOUSEBUTTONDOWN){
								if (event.button.button == SDL_BUTTON_LEFT){
										int mouse_x, mouse_y;
										SDL_GetMouseState(&mouse_x, &mouse_y);
										if (mouseinRect(text1.rect, mouse_x, mouse_y)) *coloringMethod = LAYERED;
										if (mouseinRect(text2.rect, mouse_x, mouse_y)) *coloringMethod = SMOOTH;
										if (mouseinRect(text3.rect, mouse_x, mouse_y)) *coloringMethod = STRIPED;
										if (mouseinRect(checkbutton, mouse_x, mouse_y)){
												if (*isLogisticMap == false) *isLogisticMap = true;
												else if (*isLogisticMap == true) *isLogisticMap = false;
										}
								}
						}
				}
				int indentation = 100;
				SDL_Color col = {0, 0, 0, 255};
				render_textc(d->renderer, indentation, 160, "o", font2, &text1.texture, &text.rect, col);
				render_textc(d->renderer, indentation, 220, "o", font2, &text1.texture, &text.rect, col);
				render_textc(d->renderer, indentation, 280, "o", font2, &text1.texture, &text.rect, col);
				SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, 255);
				switch (*coloringMethod) {
						case LAYERED:
								render_text(d->renderer, indentation, 160, "o", font2, &text1.texture, &text.rect);
						break;
						case SMOOTH:
								render_text(d->renderer, indentation, 220, "o", font2, &text1.texture, &text.rect);
						break;
						case STRIPED:
								render_text(d->renderer, indentation, 280, "o", font2, &text1.texture, &text.rect);
						break;
				}

				if (*isLogisticMap == true) SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, 255);
				else SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255);


				SDL_RenderDrawLine(d->renderer, checkbutton.x, checkbutton.y, checkbutton.x + checkbutton.w, checkbutton.y + checkbutton.h);
				SDL_RenderDrawLine(d->renderer, checkbutton.x + checkbutton.w, checkbutton.y, checkbutton.x, checkbutton.y + checkbutton.h);
				SDL_UpdateWindowSurface(d->window);
		}
		TTF_CloseFont(font1);
		TTF_CloseFont(font2);
		font1 = NULL;
		font2 = NULL;
}

void drawMandel(display *d, mpfr_t *midx, mpfr_t *midy, long double zoom, bool *exit_, SDL_Event *event, bool isLogisticMap, int coloringMethod, zoomData zd){
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

				mpfr_sub(help1, *midx, left, MPFR_RNDD);
				mpfr_mul_d(help1, help1, 2 * (double) i / WIDTH, MPFR_RNDD);
				mpfr_add(help1, left, help1, MPFR_RNDD);

				for (int j = 0; j < HEIGHT; j++){

						mpfr_sub(help2, up, *midy, MPFR_RNDD);
						mpfr_mul_d(help2, help2, 2 * (double) j / HEIGHT, MPFR_RNDD);
						mpfr_sub(help2, up, help2, MPFR_RNDD);

						set_complex(&c, help1, help2);


						long long int n = mandelConverge(&c, zd.num_it);
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
				if (isLogisticMap == true) logisticMap(d, help1, i);
				if (i % 2 == 0){
						SDL_UpdateWindowSurface(d->window);
				}
				if (i % 5 == 0) {
						while (SDL_PollEvent(event)){
								if (event->type == SDL_QUIT) *exit_ = true;
								if (event->type == SDL_KEYDOWN){

								}
						}
				}
		}
		complex_clear(&c);
		mpfr_clear(left);
		mpfr_clear(up);
		mpfr_clear(help1);
		mpfr_clear(help2);
}

void runMandel(display *d, int *changeState){
		zoomData zd = {100, 2};
		bool isLogisticMap = false;
		int coloringMethod = SMOOTH;
		SDL_Event event;
		long double zoom = 0.6;
		bool draw = true;
		bool exit_ = false;

		mpfr_t left, up;
		mpfr_init(left);
		mpfr_init(up);

		mpfr_t midx, midy;
		mpfr_init(midx);
		mpfr_init(midy);
		mpfr_set_d(midx, -0.5, MPFR_RNDD);
		mpfr_set_d(midy, 0, MPFR_RNDD);

		bool drawCorrespondingJulia = false;
		mandelMenu(d, &isLogisticMap, &coloringMethod, &exit_);
		while (! exit_){
				if (draw){
						SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255);
						SDL_RenderClear(d->renderer);
						drawMandel(d, &midx, &midy, zoom, &exit_, &event, isLogisticMap, coloringMethod, zd);
						isLogisticMap = false;
						draw = false;
				}
				calculateLeftAndUp(&left, &up, midx, midy, zoom);
				zoomInOut(&midx, &midy, left, up, &zoom, &draw, &exit_, &zd, &drawCorrespondingJulia);
				if (drawCorrespondingJulia){
						bool _exit = false;
						zoomData zd = {300, 2};
						complex comp;
						init_complex(&comp);
						set_complex(&comp, midx, midy);
						mpfr_set_d(midx, 0, MPFR_RNDD);
						mpfr_set_d(midy, 0, MPFR_RNDD);
								drawJulia(d, &midx,  &midy, 0.6, &exit_, &event, coloringMethod, zd, comp);
						while (! _exit){
								while (SDL_PollEvent(&event)){
										if (event.type == SDL_QUIT) _exit = true;
								}
						}
						mpfr_set(midx, comp.r, MPFR_RNDD);
						mpfr_set(midy, comp.i, MPFR_RNDD);
						complex_clear(&comp);

						drawCorrespondingJulia = false;
						_exit = false;
						draw = true;
				}
		}

		*changeState = 0;
		mpfr_clear(midx);
		mpfr_clear(midy);
		mpfr_clear(left);
		mpfr_clear(up);
}



int MandelMain(){
				mpfr_set_default_prec(100);
				setbuf(stdout, NULL);
        SDL_Init(SDL_INIT_VIDEO);
				TTF_Init();
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
		    SDL_SetRenderDrawBlendMode(d->renderer, SDL_BLENDMODE_BLEND);

				int currentMenuItem;

				SDL_RenderPresent(d->renderer);
				SDL_RenderClear(d->renderer);
						//bool isLogisticMap = false;
						//int coloringMethod = 1;
						runMandel(d, &currentMenuItem);
						//mandelMenu(d, &isLogisticMap, &coloringMethod, &isLogisticMap);

        SDL_DestroyRenderer(d->renderer);
        SDL_DestroyWindow(d->window);
        SDL_Quit();
    return 0;
}
