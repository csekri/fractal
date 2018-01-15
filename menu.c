
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "w_h_display.h"
#include "font.h"
#include "triangle_further.h"
#include "menu.h"

//menu label constants
char op1[] = "Mandelbrot set";
char op2[] = "Julia set";
char op3[] = "Buddhabrot fractal";
char op4[] = "Lindenmayer system";
char op5[] = "Guide";
char op6[] = "Exit";

//menu indentation
enum {HSPACE = 100, VSPACE = 100, SKIP = 60};

//current item - we need it in the main.c
int currentMenuItem = 0;

//enum states {NOTHING, MANDEL, JULIA, BUDDHA, LSYSTEM, GUIDE, EXIT};


//left, right, above, below
bool isMousePointerInRect(int l, int r, int a, int b, int mouse_x, int mouse_y){
    char l_str[5], r_str[5], a_str[5], b_str[5], x_str[5], y_str[5];
    sprintf(l_str, "%d", l);
    sprintf(r_str, "%d", r);
    sprintf(a_str, "%d", a);
    sprintf(b_str, "%d", b);
    sprintf(x_str, "%d", mouse_x);
    sprintf(y_str, "%d", mouse_y);
    bool firstTriangle = false, secondtriangle = false;
    if (triangle(l_str, a_str, r_str, a_str, l_str, b_str, x_str, y_str) == Inside)
    firstTriangle = true;
    if (triangle(r_str, b_str, r_str, a_str, l_str, b_str, x_str, y_str) == Inside)
    secondtriangle = true;
    return firstTriangle || secondtriangle;
}

void mouseMenuItemHighlight(SDL_Rect rect, int itemID, int mouse_x, int mouse_y, int *currentMenuItem){
    if (isMousePointerInRect(rect.x, rect.x + rect.w, rect.y, rect.y + rect.h, mouse_x, mouse_y))
    {
         *currentMenuItem = itemID;
    }
}

void drawMenu(display *d, TTF_Font *font, bool *running, SDL_Event *event, int *currentMenuItem){
    text mandelText, juliaText, buddhaText, l_systemText, guideText, exitText;
    text arrow;
    SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, 255);
    render_text(d->renderer, HSPACE, VSPACE,            op1, font, &mandelText.texture,   &mandelText.rect);
    render_text(d->renderer, HSPACE, VSPACE + SKIP,     op2, font, &juliaText.texture,    &juliaText.rect);
    render_text(d->renderer, HSPACE, VSPACE + 2 * SKIP, op3, font, &buddhaText.texture,   &buddhaText.rect);
    render_text(d->renderer, HSPACE, VSPACE + 3 * SKIP, op4, font, &l_systemText.texture, &l_systemText.rect);
    render_text(d->renderer, HSPACE, VSPACE + 4 * SKIP, op5, font, &guideText.texture, &guideText.rect);
    render_text(d->renderer, HSPACE, VSPACE + 5 * SKIP, op6, font, &exitText.texture, &exitText.rect);
    bool mouse = false;
    int mouse_x = 0, mouse_y = 0;
    while (SDL_PollEvent(event))
    {
        switch (event->type) {
          case SDL_QUIT:
              *running = false;
          break;
          case SDL_MOUSEMOTION:
              if (event->motion.xrel != 0 || event->motion.yrel != 0) mouse = true;
              mouse_x = event->motion.x;
              mouse_y = event->motion.y;
          break;
          case SDL_KEYDOWN:
              if (event->key.keysym.sym == SDLK_LEFT ||
              event->key.keysym.sym == SDLK_UP){
                  if (*currentMenuItem == 0) { *currentMenuItem = 5; } else
                  { (*currentMenuItem)--; }
              }
              if (event->key.keysym.sym == SDLK_RIGHT ||
              event->key.keysym.sym == SDLK_DOWN){
                  if (*currentMenuItem == 5) { *currentMenuItem = 0; } else
                  { (*currentMenuItem)++; }
              }
              if (event->key.keysym.sym == SDLK_KP_ENTER || event->key.keysym.sym == SDLK_RETURN){
                  *currentMenuItem = - *currentMenuItem - 1;
              }
          break;
          case SDL_MOUSEBUTTONDOWN:
              //rect.x, rect.x + rect.w, rect.y, rect.y + rect.h
              if (event->button.button == SDL_BUTTON_LEFT &&
                  (isMousePointerInRect(mandelText.rect.x, mandelText.rect.x + mandelText.rect.w,
                    mandelText.rect.y, mandelText.rect.y + mandelText.rect.h, event->button.x, event->button.y) ||
                  isMousePointerInRect(juliaText.rect.x, juliaText.rect.x + juliaText.rect.w,
                    juliaText.rect.y, juliaText.rect.y + juliaText.rect.h, event->button.x, event->button.y) ||
                  isMousePointerInRect(buddhaText.rect.x, buddhaText.rect.x + buddhaText.rect.w,
                    buddhaText.rect.y, buddhaText.rect.y + buddhaText.rect.h, event->button.x, event->button.y) ||
                  isMousePointerInRect(l_systemText.rect.x, l_systemText.rect.x + l_systemText.rect.w,
                    l_systemText.rect.y, l_systemText.rect.y + l_systemText.rect.h, event->button.x, event->button.y) ||
                  isMousePointerInRect(guideText.rect.x, guideText.rect.x + guideText.rect.w,
                    guideText.rect.y, guideText.rect.y + guideText.rect.h, event->button.x, event->button.y) ||
                  isMousePointerInRect(exitText.rect.x, exitText.rect.x + exitText.rect.w,
                    exitText.rect.y, exitText.rect.y + exitText.rect.h, event->button.x, event->button.y)
                )) *currentMenuItem = - *currentMenuItem - 1;
          break;

        }
    }
    if (mouse){
        mouseMenuItemHighlight(mandelText.rect, 0, mouse_x, mouse_y, currentMenuItem);
        mouseMenuItemHighlight(juliaText.rect, 1, mouse_x, mouse_y, currentMenuItem);
        mouseMenuItemHighlight(buddhaText.rect, 2, mouse_x, mouse_y, currentMenuItem);
        mouseMenuItemHighlight(l_systemText.rect, 3, mouse_x, mouse_y, currentMenuItem);
        mouseMenuItemHighlight(guideText.rect, 4, mouse_x, mouse_y, currentMenuItem);
        mouseMenuItemHighlight(exitText.rect, 5, mouse_x, mouse_y, currentMenuItem);
    }
    if (*currentMenuItem >= 0)
    render_text(d->renderer, HSPACE - 40, VSPACE + *currentMenuItem * SKIP, ">", font, &arrow.texture, &arrow.rect);
}


int MenuMain(){
      setbuf(stdout, NULL);
      TTF_Font *font;
      TTF_Init();
      load_TTF_File("COURIER.TTF", &font, 54);
      int currentMenuItem = 0;

      /* Inint TTF. */
      display *d = malloc(sizeof(display));
      SDL_Init(SDL_INIT_VIDEO);
      int x = SDL_WINDOWPOS_UNDEFINED, y = SDL_WINDOWPOS_UNDEFINED;
      d->window = SDL_CreateWindow("Fractals", x, y, WIDTH, HEIGHT, 0);
      SDL_Surface *surface = SDL_GetWindowSurface(d->window);
      d->renderer = SDL_CreateSoftwareRenderer(surface);
      SDL_Event event;
      SDL_SetRenderDrawBlendMode(d->renderer, SDL_BLENDMODE_BLEND);

      bool running = true;

      while (running) {
          /*while (SDL_PollEvent(&event) == 1) {
              if (event.type == SDL_QUIT) {
                  running = false;
              }
          }*/
          SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 0);
          SDL_RenderClear(d->renderer);

          drawMenu(d, font, &running, &event, &currentMenuItem);
          SDL_UpdateWindowSurface(d->window);
      }

      /* Deinit TTF. */

      TTF_Quit();

      SDL_DestroyRenderer(d->renderer);
      SDL_DestroyWindow(d->window);
      SDL_Quit();
      return EXIT_SUCCESS;



}
