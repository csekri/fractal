/*THIS IS A USER FRIENDLY FRACTAL EXPLORER PROGRAM*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "w_h_display.h"
#include "complex.h"
#include "julia.h"
#include "mandel.h"
#include "buddha.h"
#include "font.h"
#include "menu.h"
#include "sdl_lsystem.h"
#include "guide.h"

//make menu options more readable
enum states {MANDEL = -1, JULIA = -2, BUDDHA = -3, LSYSTEM = -4, GUIDE = -5, EXIT = -6};

int main(){
//begin_initializations-------------------------------------------------
    setbuf(stdout, NULL);
    srand(time(NULL));
    mpfr_set_default_prec(600);
    int currentMenuItem = 0;

    display *d = malloc(sizeof(display));
    SDL_Init(SDL_INIT_VIDEO);
    int x = SDL_WINDOWPOS_UNDEFINED, y = SDL_WINDOWPOS_UNDEFINED;
    d->window = SDL_CreateWindow("Fractals", x, y, WIDTH, HEIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(d->window);
    d->renderer = SDL_CreateSoftwareRenderer(surface);
    SDL_Event event;
    SDL_SetRenderDrawBlendMode(d->renderer, SDL_BLENDMODE_BLEND);
    TTF_Font *font54, *font16;
    TTF_Init();
    load_TTF_File("COURIER.TTF", &font54, 54);
    load_TTF_File("COURIER.TTF", &font16, 24);
//end_initializations---------------------------------------------------

    bool running = true;

    while(running)
    {
        SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 0);
        SDL_RenderClear(d->renderer);
        switch (currentMenuItem){
            case MANDEL:
                runMandel(d, &currentMenuItem);
            break;
            case JULIA:
                runJulia(d, &currentMenuItem, &event);
            break;
            case BUDDHA:
                buddhaBrot(d, &currentMenuItem);
            break;
            case LSYSTEM:
                lsystemMenu(d, &currentMenuItem);
            break;
            case GUIDE:
                detailItem(d, &currentMenuItem);
            break;
            case EXIT:
                running = false;
            break;
            default :
                drawMenu(d, font54, &running, &event, &currentMenuItem);
        }

        SDL_UpdateWindowSurface(d->window);
    }

//begin_release_resources------------------------
    TTF_CloseFont(font54);
    font54 = NULL;
    TTF_CloseFont(font16);
    font16 = NULL;
    TTF_Quit();
    SDL_DestroyRenderer(d->renderer);
    SDL_DestroyWindow(d->window);
    SDL_Quit();
//end_release_resources--------------------------
    return 0;

}
