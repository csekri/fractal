#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdbool.h>

#include "font.h"
#include "w_h_display.h"
#include "guide.h"

char *guideTitle = "User Manual";
char *mandelTitle = "Mandelbrot set";
char *juliaTitle = "Julia set";
char *buddhaTitle = "Buddhabrot fractal";
char *lsystemTitle = "Lindenmayer system";
char *guideText = "After launching the program we can see a simple menu. Buy default the selected item is the Mandelbrot set. We can navigate using the arrow keys or by moving the mouse pointer over the text. We can select the item both by clicking on it or pressing <ENTER>.";
char *mandelText = "The Mandelbrot set is the set of complex numbers c for which the function f_c(z)=z^2+c does not diverge when iterated from z=0, i.e., for which the sequence f_c(0), f_c(f_c(0)) etc., remains bounded in absolute value.\nIts definition and name are due to Adrien Douady, in tribute to the mathematician Benoit Mandelbrot. The set is connected to a Julia set, and related Julia sets produce similarly complex fractal shapes.\nMandelbrot set images may be created by sampling the complex numbers and determining, for each sample point c, whether the result of iterating the above function goes to infinity. Treating the real and imaginary parts of c as image coordinates (x+yi) on the complex plane, pixels may then be colored according to how rapidly the sequence z_n^2+c diverges, with the color 0 (black) usually used for points where the sequence does not diverge. If c is held constant and the initial value of z denoted by z_0—is variable instead, one obtains the corresponding Julia set for each point c in the parameter space of the simple function.";
char *juliaText = "Let R(z) be rational function R=P(z)/Q(z), where z is the element of Riemann sphere (the Riemann sphere is the union of the complex numbers and infinity, like the ideal point in the projective geometry). The \"filled-in\" Julia set J_R is the set of points z which do not approach infinity after R(z) is repeatedly applied (corresponding to a strange attractor). In our program we can plot a special case of Julia sets, when R(z)=z^2+c.";
char *lsystemText = "An L-system or Lindenmayer system is a parallel rewriting system and a type of formal grammar. An L-system consists of an alphabet of symbols that can be used to make strings, a collection of production rules that expand each symbol into some larger string of symbols, an initial axiom string from which to begin construction, and a mechanism for translating the generated strings into geometric structures. L-systems were introduced and developed in 1968 by Aristid Lindenmayer, a Hungarian theoretical biologist and botanist at the University of Utrecht. Lindenmayer used L-systems to describe the behaviour of plant cells and to model the growth processes of plant development. L-systems have also been used to model the morphology of a variety of organisms and can be used to generate self-similar fractals such as iterated function systems.\nWe use the rules that the free software inkscape use:\n    Any of A, B, C, D, E, F: draw forward\n    Any of G, H, I, J, K, L: move forward\n    +: turn left\n    -: turn right\n    |: turn 180 degrees\n    [: remember point\n    ]: return to remembered point\nThe program does not allow any other character in the code.";

void writeSuff(SDL_Renderer *renderer, int vspace, int *getTextHeight){
    text mytext;
    int hspace = 100;
    int textWidth = 1300;
    TTF_Font *font20, *font54;

    load_TTF_File("COURIER.TTF", &font54, 54);
    load_TTF_File("COURIER.TTF", &font20, 30);
    {
        render_text2(renderer, hspace, vspace, guideTitle, font54, &mytext.texture, &mytext.rect, textWidth);
        render_text2(renderer, hspace, mytext.rect.y + 60, guideText, font20, &mytext.texture, &mytext.rect, textWidth);
        render_text2(renderer, hspace, mytext.rect.y + mytext.rect.h + 20, mandelTitle, font54, &mytext.texture, &mytext.rect, textWidth);
        render_text2(renderer, hspace, mytext.rect.y + mytext.rect.h + 60, mandelText, font20, &mytext.texture, &mytext.rect, textWidth);
        render_text2(renderer, hspace, mytext.rect.y + mytext.rect.h + 20, juliaTitle, font54, &mytext.texture, &mytext.rect, textWidth);
        render_text2(renderer, hspace, mytext.rect.y + mytext.rect.h + 60, juliaText, font20, &mytext.texture, &mytext.rect, textWidth);
        render_text2(renderer, hspace, mytext.rect.y + mytext.rect.h + 20, lsystemTitle, font54, &mytext.texture, &mytext.rect, textWidth);
        render_text2(renderer, hspace, mytext.rect.y + mytext.rect.h + 60, lsystemText, font20, &mytext.texture, &mytext.rect, textWidth);
    }
    SDL_DestroyTexture(mytext.texture);
    TTF_CloseFont(font20);
    TTF_CloseFont(font54);
    font20 = NULL;
    font54 = NULL;
    *getTextHeight = mytext.rect.y + mytext.rect.h - vspace;
}

void detailItem(display *d, int *changeState){
    SDL_Event event;
    bool exit_ = false;
    int vspace = 30;
    int textHeight = 0;
    while (! exit_){
        SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255);
        SDL_RenderClear(d->renderer);
        writeSuff(d->renderer, vspace, &textHeight);
        SDL_UpdateWindowSurface(d->window);
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_KEYDOWN){
                switch (event.key.keysym.sym) {
                    case SDLK_DOWN:
                    if (vspace > -textHeight + 800)
                        vspace -= 20;
                    break;
                    case SDLK_UP:
                    if (vspace < 30) vspace += 20;
                    break;
                    case SDLK_BACKSPACE:
                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                    case SDLK_ESCAPE:
                        exit_ = true;
                    break;
                }
            }
            if (event.type == SDL_MOUSEWHEEL){
                if (event.wheel.y > 0){
                    if (vspace < 30) vspace += event.wheel.y * 20;
                } else if (event.wheel.y < 0){
                    if (vspace > -textHeight + 800) vspace += event.wheel.y * 20;
                }
            }
            if (event.type == SDL_MOUSEMOTION){
                if (event.button.button == SDL_BUTTON_LEFT){
                  if (event.motion.yrel > 0){
                      if (vspace < 30) vspace += event.motion.yrel;
                  } else if (event.motion.yrel < 0){
                      if (vspace > -textHeight + 800) vspace += event.motion.yrel;
                  }
                 }
            }
        }
    }
    //SDL_Delay(5000);
    *changeState = 0;

}

int DetailMain(){

    return 0;
}
