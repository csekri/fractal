#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "font.h"
#include "w_h_display.h"

void load_TTF_File(const char *path, TTF_Font **font, char fontsize){
    *font = TTF_OpenFont(path, fontsize);
    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }
}

//this function is from https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2
void render_text(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor = {255, 255, 255, 0};

    surface = TTF_RenderText_Solid(font, text, textColor);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
    SDL_RenderCopy(renderer, *texture, NULL, rect);
    SDL_DestroyTexture(*texture);
}

//colored text
void render_textc(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect, SDL_Color col) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor = {col.r, col.g, col.b, 255};

    surface = TTF_RenderText_Solid(font, text, textColor);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
    SDL_RenderCopy(renderer, *texture, NULL, rect);
    SDL_DestroyTexture(*texture);
}

void render_text2(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect, int width) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor = {255, 255, 255, 0};

    surface = TTF_RenderText_Blended_Wrapped(font, text, textColor, width);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
    SDL_RenderCopy(renderer, *texture, NULL, rect);
    SDL_DestroyTexture(*texture);
}

int FontMain() {
    SDL_Event event;
    TTF_Font *font;
    text mandelText, juliaText, buddhaText, l_systemText, exitText;
    int quit;
    TTF_Init();
    load_TTF_File("COURIER.TTF", &font, 44);
    /* Inint TTF. */
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Fractals", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    quit = 0;
    while (quit == 0) {
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        int menuVSpace = 400;
        render_text(renderer, menuVSpace, 50, "Mandelbrot set", font, &mandelText.texture, &mandelText.rect);
        render_text(renderer, menuVSpace, 100, "Julia set", font, &juliaText.texture, &juliaText.rect);
        render_text(renderer, menuVSpace, 150, "Buddhabrot fractal", font, &buddhaText.texture, &buddhaText.rect);
        render_text(renderer, menuVSpace, 200, "Lindenmayer system", font, &l_systemText.texture, &l_systemText.rect);

        SDL_SetRenderDrawColor(renderer, 255,255,255, 30);
        SDL_RenderDrawLine(renderer, 0,0,100,100);

        SDL_UpdateWindowSurface(window);
    }

    /* Deinit TTF. */
    SDL_DestroyTexture(mandelText.texture);
    SDL_DestroyTexture(buddhaText.texture);
    SDL_DestroyTexture(juliaText.texture);
    SDL_DestroyTexture(l_systemText.texture);
    SDL_DestroyTexture(exitText.texture);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
