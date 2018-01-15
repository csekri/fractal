/*
This module contains functions to render Buddhabrot
*/

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "w_h_display.h"
#include "buddha.h"

//we don't need multi-precision numbers so it's better to define a double complex library
struct ldouble_complex{
    long double r, i;
};
typedef struct ldouble_complex ldcomplex;

//multiplication
ldcomplex ldcomplex_mul(ldcomplex a, ldcomplex b){
    ldcomplex out;
    out.r = a.r * b.r - a.i * b.i;
    out.i = a.r * b.i + a.i * b.r;
    return out;
}

//addition
ldcomplex ldcomplex_add(ldcomplex a, ldcomplex b){
    ldcomplex out;
    out.r = a.r + b.r;
    out.i = a.i + b.i;
    return out;
}

//norm of a complex number
long double ldcomplex_norm(ldcomplex a){
    return a.r * a.r + a.i * a.i;
}

//using Central Limit Theorem with 1000 samples
double normalDistribution(){
    double sum = 0;
    double normal = 0;
    int num_samples = 1000;
    for (int i = 0; i < num_samples; i++){
        sum += (double) rand() / RAND_MAX;
    }
    normal = (sum - (double) (num_samples / 2)) / sqrt((double) num_samples / 12);

    return normal;
}

//uniform distribution or
//if X is a random variable then X ~ U(0,1)
double unifDist(){
    return (double) rand() / RAND_MAX;
}

//if X is a random variable then it's distribution:
//X ~ U(-1,1)
//we can generate random double in range [-1,1]
double unifDist_neg1to1(){
    double x = unifDist();
    return (x - 0.5) * 2;
}

//set all elements of a matrix to 0
void initMatrix(int h, int w, int matrix[h][w]){
    for (int i = 0; i < w; i++)
    for (int j = 0; j < h; j++)
        matrix[j][i] = 0;
}

//draw the Buddhabrot pixel values with transparency onto the screen
void drawPixels(SDL_Renderer *renderer, int h, int w, int pixel[h][w]){

    for (int i = 0; i < w; i++)
    for (int j = 0; j < h; j++){
        if (pixel[j][i] < 256){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, pixel[j][i]);
        } else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawPoint(renderer, i, j);
    }
}

//we check if the random sample is in the biggest circle
bool bulb2Test(long double x, long double y){
    return ((x + 1) * (x + 1) + y * y) < (long double) 1 / 16 ? true : false;
}

//we check if the random sample is in the cardioid
bool cardioidTest(long double x, long double y){
    long double q = (x - (long double) 1 / 4) * (x - (long double) 1 / 4) + y * y;
    return q * (q + (x - (long double) 1 / 4)) < y * y * (long double) 1 / 4 ? true : false;
}

//this is a random sample calculation
void aRandomSample(int h, int w, int pixel[h][w]){
    int num_iterate = 20000;
    double transformationConstant1 = 930;
    double transformationConstant2 = 0.7;
    long double x = 0, y = 0;
    //while (x > -1 || x < 1){
    x = unifDist_neg1to1();
    y = unifDist_neg1to1();

    x = x * (double) WIDTH / transformationConstant1 - transformationConstant2;
    y = y * (double) HEIGHT / transformationConstant1;
    ldcomplex c = { x, y };
    ldcomplex z = { 0, 0 };

    for (int i = 0; i < num_iterate; i++){
        if (cardioidTest(x, y) || bulb2Test(x, y)) break;
        z = ldcomplex_add(z, c);
        z = ldcomplex_mul(z, z);
        if (ldcomplex_norm(z) >= 4 && i < 1000) break;
        if (ldcomplex_norm(z) >= 4){
            z = (ldcomplex) { 0, 0 };
            for (int j = 0; j < i; j++){
                z = ldcomplex_mul(z, z);
                z = ldcomplex_add(z, c);

                long double numx, numy;
                numx = (z.r + transformationConstant2) * (long double) transformationConstant1 / WIDTH;
                numy = z.i * (long double) transformationConstant1 / HEIGHT;
                numx = (numx + 1) * w / 2;
                numy = (numy + 1) * h / 2;
                int xx, yy;
                xx = (int) numx;
                yy = (int) numy;

                if (0 < xx && xx < w && yy > 0 && yy < h){
                    pixel[yy][xx] += 1;
                    pixel[h - yy][xx] += 1;
                }
            }
            break;
        }
    }
}

//everything is put here in the same function that we can call from
//the header
void buddhaBrot(display *d, int *changeState){
    SDL_Event event;
    srand(time(NULL));
    int pixel[HEIGHT][WIDTH];
    initMatrix(HEIGHT, WIDTH, pixel);
    SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255);
    SDL_RenderClear(d->renderer);
    SDL_UpdateWindowSurface(d->window);
    for (int i = 0; i < 5000000; i++){

        aRandomSample(HEIGHT, WIDTH, pixel);
        if (i % 20000 == 0) {
            SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255);
            SDL_RenderClear(d->renderer);
            SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, 255);
            drawPixels(d->renderer, HEIGHT, WIDTH, pixel);
            SDL_UpdateWindowSurface(d->window);
            bool quit = false;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) quit = true;
            }
            if (quit) break;
        }

    }
    bool exit_ = false;
    SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(d->renderer, 0,1,100, 1);
    SDL_UpdateWindowSurface(d->window);
    while (! exit_){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) exit_ = true;
        }
    }
    SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255);
    SDL_RenderClear(d->renderer);
    *changeState = 0;
}

//a place to test random variables
int BuddhaMain(){
    setbuf(stdout, NULL);
    srand(time(NULL));
    double max=-1, num = 0;
    for (int i = 0; i < 100; i++)
    {
        num = normalDistribution();
        if (max < num) max = num;
        printf("num: %f , max: %f\n", num, max);
    }
    printf("\nThe max is: %f\n", max);

    for (int i = 0; i < 100; i++) printf("%f\n", unifDist_neg1to1());


    return 0;
}
