/* This program is to create strings for Lindenmayer systems*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "triangle_further.h"
#include "w_h_display.h"
#include "font.h"
#include "menu.h"
#include "l-system.h"
#include "stack.h"
#include "sdl_lsystem.h"

enum {SELECTED, BACKSPACE, NEWCHAR, NOTHING};

struct ibox{
    int todo;
    int id;
    SDL_Rect rect;
    char newch;
    char *str;
};
typedef struct ibox ibox;

const long double PI = 3.14159265358979323846264338;

SDL_Rect startButtonRect = {300, 760, 100, 50};

void draw(SDL_Renderer *renderer, state *sta, long double length){
    long double endx = sta->pos.x + cos((long double) sta->angle / 180 * PI) * length;
    long double endy = sta->pos.y + sin((long double) sta->angle / 180 * PI) * length;
    SDL_RenderDrawLine(renderer, sta->pos.x, sta->pos.y, endx, endy);
    sta->pos.x = endx;
    sta->pos.y = endy;
}

void move(state *sta, long double length){
    sta->pos.x = sta->pos.x + cos((long double) sta->angle / 180 * PI) * length;
    sta->pos.y = sta->pos.y + sin((long double) sta->angle / 180 * PI) * length;
}

void rotation(state *sta, long double angle){
    sta->angle += (long double) angle;
}

static bool mouseinRect(SDL_Rect rect, int mouse_x, int mouse_y){
    return isMousePointerInRect(rect.x, rect.x + rect.w, rect.y, rect.y + rect.h, mouse_x, mouse_y);
//the traditional method is below:
//return (rect.x < mouse_x && mouse_x < rect.x + rect.w && rect.y < mouse_y && mouse_y < rect.y + rect.h) ? true : false;
}

//delete substring at pos
static void deleteString(long int pos, long int length, char *a){
    char *add;
    add = (char*)malloc(strlen(a) - length + 1);
    strncpy(add, a, pos);
    add[pos] = '\0';
    strcat(add, a+pos+length);
    strcpy(a, add);
    free(add);
}

static void deleteNotBrackets(char *input, char *output){
    strcpy(output, input);
    for (int i = strlen(input) - 1; i >= 0; i--){
        if (! (input[i] == '(' || input[i] == ')' || input[i] == '[' ||
              input[i] == ']' || input[i] == '{' || input[i] == '}'))
            deleteString(i, 1, output);
    }
}

//check if parenthesis is ok
bool parenthesis(char *input){
    //for parenthesis
    int leftBr = 0; int rightBr = 0;
    char *in = malloc(strlen(input) + 1);
    strcpy(in, input);
    deleteNotBrackets(input, in);
    for (int i = 0; i < strlen(in); i++){
        if (in[i] == '[') leftBr++;
        if (in[i] == ']') rightBr++;
        if (rightBr > leftBr){
            free(in);
            return false;
        }
    }
    free(in);
    if (leftBr != rightBr) return false;
    return true;
}

void cutAtEquals(char *input, char *ch, char *str){
    *ch = input[1];
    strcpy(str, input+3);
}

bool errorWithAngle(char *angle){
    char str[50] = "";
    sprintf(str, "%d", atoi(angle+1));
    if (strcmp(str, angle+1) != 0) return false;
    return true;
}

void moveLeft(char **input){
    for (int j = 0; j < 14; j++){
        for (int i = 3; i < 14; i++){
            if (input[i][0] == '\0'){
                strcpy(input[i], input[i + 1]);
                strcpy(input[i + 1], "");
            }
        }
    }
}

void readLsystem(ibox *boxs, char **output, int *numRules){
    strcpy(output[1], boxs[0].str+1);
    strcpy(output[2], boxs[2].str+1);
    for (int i = 0; i < 6; i++){
        if (strlen(boxs[i + 3].str) > 3){
            cutAtEquals(boxs[i + 3].str, &output[2 * i + 3][0], output[2 * i + 4]);
            output[2 * i + 3][1] = '\0';
            (*numRules)++;
        }
    }
    moveLeft(output);
}



void inputBox(SDL_Renderer *renderer, ibox *box){
    SDL_Rect rect = {box->rect.x, box->rect.y, 500, 50};
    text txt;
    TTF_Font *font16;
    load_TTF_File("COURIER.TTF", &font16, 20);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int length = strlen(box->str);
    switch (box->todo) {
      case BACKSPACE:
          if (length > 1) box->str[length - 1] = '\0';
          box->todo = SELECTED;
      break;
      case NEWCHAR:
          if (length < 40){
              box->str[length] = box->newch;
              box->str[length + 1] = '\0';
          }
          box->todo = SELECTED;
      break;
    }
    render_text(renderer, box->rect.x + 5, box->rect.y + 15, box->str, font16, &txt.texture, &txt.rect);
    SDL_RenderDrawRect(renderer, &rect);
    TTF_CloseFont(font16);
    font16 = NULL;
}

void lsystemEvents(ibox *boxs ,bool *exit_, bool *trigger){
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        switch (event.type) {
            case SDL_QUIT:
                if (event.type == SDL_QUIT) *exit_ = true;
            break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT){
                    int mouse_x, mouse_y;
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    for (int i = 0; i < 9; i++){
                        if (mouseinRect(boxs[i].rect, mouse_x, mouse_y)){
                            boxs[i].todo = SELECTED;
                        } else boxs[i].todo = NOTHING;
                    }
                    if (mouseinRect(startButtonRect, mouse_x, mouse_y)) *trigger = true;
                }
            break;
            case SDL_KEYDOWN:{
                int keycode = event.key.keysym.sym;
                int keymod = event.key.keysym.mod;
                if (event.key.keysym.sym == SDLK_BACKSPACE){
                    for (int i = 0; i < 9; i++){
                        if (boxs[i].todo == SELECTED) boxs[i].todo = BACKSPACE;
                    }
                }
                if (keycode == SDLK_TAB && keymod == KMOD_NONE){
                    int selected = -1;
                    for (int i = 0; i < 9; i++){
                        if (boxs[i].todo == SELECTED) selected = i;
                    }
                    if (selected >= 0){
                        if (selected < 8){
                            boxs[selected].todo = NOTHING;
                            boxs[selected + 1].todo = SELECTED;
                        } else{
                            boxs[8].todo = NOTHING;
                            boxs[0].todo = SELECTED;
                        }
                    }
                }
                else if(keycode == SDLK_TAB && (keymod == KMOD_RSHIFT || keymod == KMOD_LSHIFT)){
                    int selected  = -1;
                    for (int i = 0; i < 9; i++){
                        if (boxs[i].todo == SELECTED) selected = i;
                    }
                    if (selected  >= 0){
                        if (selected > 0){
                            boxs[selected].todo = NOTHING;
                            boxs[selected - 1].todo = SELECTED;
                        } else{
                            boxs[0].todo = NOTHING;
                            boxs[8].todo = SELECTED;
                        }
                    }
                }
                if (keycode == SDLK_RETURN){
                    *trigger = true;
                }
            }
            break;
            case SDL_TEXTINPUT:
            {
                SDL_StartTextInput();
                if (event.type == SDL_TEXTINPUT){
                    int ch = *event.text.text;
                    if ((ch <= 'l' && ch >= 'a')
                            || (ch <= 'L' && ch >= 'A')
                            || (ch <= '9' && ch >= '0')
                            || ch == '='
                            || ch == '|' || ch == '['
                            || ch == ']' || ch == '+'
                            || ch == '+' || ch == '-'){
                        for (int i = 0; i < 9; i++){
                            if (boxs[i].todo == SELECTED){
                                boxs[i].todo = NEWCHAR;
                                boxs[i].newch = toupper(ch);
                            }

                        }
                    }
                    SDL_StartTextInput();
                }
            }
        }
    }
}

void drawBarSepLine(SDL_Renderer *renderer){
    int barWidth = 650;
    SDL_RenderDrawLine(renderer, barWidth, 0, barWidth, HEIGHT);
}

void drawLabels(SDL_Renderer *renderer){
    TTF_Font *font16;
    int x = 10;
    load_TTF_File("COURIER.TTF", &font16, 18);
    text txt;
    render_text(renderer, x, 115, "Iterations:", font16, &txt.texture, &txt.rect);
    render_text(renderer, x, 115 + 70, "Angle:", font16, &txt.texture, &txt.rect);
    render_text(renderer, x, 115 + 140, "Axiom:", font16, &txt.texture, &txt.rect);
    render_text(renderer, x, 115 + 210, "Rule 1:", font16, &txt.texture, &txt.rect);
    render_text(renderer, x, 115 + 280, "Rule 2:", font16, &txt.texture, &txt.rect);
    render_text(renderer, x, 115 + 350, "Rule 3:", font16, &txt.texture, &txt.rect);
    render_text(renderer, x, 115 + 420, "Rule 4:", font16, &txt.texture, &txt.rect);
    render_text(renderer, x, 115 + 490, "Rule 5:", font16, &txt.texture, &txt.rect);
    render_text(renderer, x, 115 + 560, "Rule 6:", font16, &txt.texture, &txt.rect);
    TTF_CloseFont(font16);
    font16 = NULL;
}

void cursor(SDL_Renderer *renderer, ibox *boxs, int *count){
    char str[100] = "";
    int millis = 70;
    text txt;
    TTF_Font *font20;
    load_TTF_File("COURIER.TTF", &font20, 20);
    if (*count < millis){
        for (int i = 0; i < 9; i++){
            if (boxs[i].todo == SELECTED){
                for (int j = 0; j < strlen(boxs[i].str); j++) str[j] = ' ';
                strcat(str, "|");
                str[strlen(str)] = '\0';
                render_text(renderer, boxs[i].rect.x - 1, boxs[i].rect.y + 15, str, font20, &txt.texture, &txt.rect);
            }
        }

    }
    (*count)++;
    if (*count > 2 * millis) *count = 0;
    TTF_CloseFont(font20);
    font20 = NULL;
}

void initBoxs(ibox *boxs){
    for (int i = 0; i < 9; i++){
        boxs[i] = (ibox) { NOTHING, i, {130, 100 + 70 * i, 500, 50}, ' ', malloc(100)};
        strcpy(boxs[i].str, " ");
    }
    strcpy(boxs[0].str, " 2");
    strcpy(boxs[1].str, " 60");
    strcpy(boxs[2].str, " A");
    strcpy(boxs[3].str, " A=B+A+B");
    strcpy(boxs[4].str, " B=A-B-A");
}

void deinitBoxs(ibox *boxs){
    for (int i = 0; i < 9; i++){
        free(boxs[i].str);
        boxs[i].str = NULL;
    }
    free(boxs);
    boxs = NULL;
}

void startButton(SDL_Renderer *renderer){
    TTF_Font *font20;
    load_TTF_File("COURIER.TTF", &font20, 50);
    text txt;
    render_text(renderer, startButtonRect.x, startButtonRect.y, "Draw", font20, &txt.texture, &startButtonRect);
    SDL_RenderDrawRect(renderer, &startButtonRect);
    TTF_CloseFont(font20);
    font20 = NULL;
}

long double min(long double a, long double b){
    return a > b ? b : a;
}

void drawLsystem(display *d, char **data, char *Angle, SDL_Texture **texture, int numRules, ibox *boxs, int *errCode){
    SDL_Event event;
    struct System sys = conversion(2 * numRules + 2, data);
    state sta = {{ 1175, 475 }, -90 };
    long int lenght = length(sys, 2 * numRules + 2, sys.gen);
    if (lenght == -1) lenght = 50;
    char *ls_str = malloc(lenght + 1);
    strcpy(ls_str, iteration(data, 2 * numRules + 2));
    int angle = atoi(Angle);
    if (strlen(ls_str) < 100) printf("%s", ls_str);
    if (strcmp(ls_str, "INVALID_INPUT_THE_PROGRAM_DOES_NOTHING") == 0){
      *errCode = 0;
    } else if (strcmp(ls_str, "TOO_LONG_IS_THE_CALCULATED_STRING") == 0){
      *errCode = 1;
    } else if (! parenthesis(ls_str)){
      *errCode = 2;
    } else if (! errorWithAngle(Angle)){
      *errCode = 0;
    }
    else {
      list *bracket = newStack();
      long double max_x = sta.pos.x;
      long double min_x = sta.pos.x;
      long double max_y = sta.pos.y;
      long double min_y = sta.pos.y;

      for (long int i = 0; i < lenght; i++){
          if ('A' <= ls_str[i] && ls_str[i] <= 'F'){
              move(&sta, 1);
          } else if ('G' <= ls_str[i] && ls_str[i] <= 'L'){
              move(&sta, 1);
          } else if ('|' == ls_str[i]){
              rotation(&sta, 180);
          } else if (ls_str[i] == '+') rotation(&sta, angle);
         else if (ls_str[i] == '-') rotation(&sta, -angle);
         if (ls_str[i] == '[') push(bracket, sta);
         if (ls_str[i] == ']') sta = pop(bracket);
          if (sta.pos.x > max_x) max_x = sta.pos.x;
          if (sta.pos.y > max_y) max_y = sta.pos.y;
          if (sta.pos.x < min_x) min_x = sta.pos.x;
          if (sta.pos.y < min_y) min_y = sta.pos.y;
      }

      SDL_DestroyTexture(*texture);
      *texture = SDL_CreateTexture(d->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
      SDL_SetRenderTarget(d->renderer, *texture);
      SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 0);
      SDL_RenderClear(d->renderer);
      SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, 255);

      long double dif_x = max_x - min_x;
      long double dif_y = max_y - min_y;
      long double len = min(1 / dif_x * 1050, 1 / dif_y * 950) * 0.9;
      sta.pos.x = 1175;
      sta.pos.y = 475;
      sta.angle = -90;

      for (long int i = 0; i < lenght; i++){
          if ('A' <= ls_str[i] && ls_str[i] <= 'F'){
              move(&sta, len);
          } else if ('G' <= ls_str[i] && ls_str[i] <= 'L'){
              move(&sta, len);
          } else if ('|' == ls_str[i]){
              rotation(&sta, 180);
          } else if (ls_str[i] == '+') rotation(&sta, angle);
          else if (ls_str[i] == '-') rotation(&sta, -angle);
          if (ls_str[i] == '[') push(bracket, sta);
          if (ls_str[i] == ']') sta = pop(bracket);

          if (sta.pos.x > max_x) max_x = sta.pos.x;
          if (sta.pos.y > max_y) max_y = sta.pos.y;
          if (sta.pos.x < min_x) min_x = sta.pos.x;
          if (sta.pos.y < min_y) min_y = sta.pos.y;
      }
      sta.pos.x = 2 * 1175 - (max_x + min_x) / 2;
      sta.pos.y = 2 * 475 - (max_y + min_y) / 2;
      sta.angle = -90;
      for (long int i = 0; i < lenght; i++){
          SDL_SetRenderTarget(d->renderer, *texture);
          if ('A' <= ls_str[i] && ls_str[i] <= 'F'){
              draw(d->renderer, &sta, len);
          } if ('|' == ls_str[i]){
              rotation(&sta, 180);
          } else if ('G' <= ls_str[i] && ls_str[i] <= 'L'){
              move(&sta, len);
          } else if (ls_str[i] == '+') rotation(&sta, angle);
         else if (ls_str[i] == '-') rotation(&sta, -angle);
         if (ls_str[i] == '[') push(bracket, sta);
         if (ls_str[i] == ']') sta = pop(bracket);
         SDL_SetRenderTarget(d->renderer, NULL);
         SDL_RenderCopy(d->renderer, *texture, NULL, NULL);
         for (long int i = 0; i < 9; i++){
             inputBox(d->renderer, &boxs[i]);
         }

         drawBarSepLine(d->renderer);
         drawLabels(d->renderer);
         startButton(d->renderer);

         SDL_UpdateWindowSurface(d->window);
         SDL_SetRenderTarget(d->renderer, NULL);
         bool ok = false;
         while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) ok = true;
         }
         if (ok) break;
      }
    }
    free(ls_str);
}
void lsystemMenu(display *d, int *changeState){
    SDL_Texture *lsystemTexture = SDL_CreateTexture(d->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
    ibox *boxs = malloc (sizeof (ibox) * 9);
    for (int i = 0; i < 9; i++){
        boxs[i].str = malloc(100);
    }
    char **data = malloc(15 * 100);
    for (int i = 0; i < 15; i++){
        data[i] = malloc(100);
        data[i][0] = '\0';
    }
    initBoxs(boxs);
    boxs[0].todo = SELECTED;


    int i = 0;

    SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255);
    SDL_RenderClear(d->renderer);
    SDL_UpdateWindowSurface(d->window);
    bool exit_ = false;
    bool trigger = false;
    int errCode = -1;
    while (! exit_) {
        SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255);
        SDL_RenderClear(d->renderer);
        lsystemEvents(boxs, &exit_, &trigger);
        SDL_RenderCopy(d->renderer, lsystemTexture, NULL, NULL);
        for (int i = 0; i < 9; i++){
            inputBox(d->renderer, &boxs[i]);
        }
        if (trigger){
            errCode = -1;
            int numRules = 0;
            readLsystem(boxs, data, &numRules);
            drawLsystem(d, data, boxs[1].str, &lsystemTexture, numRules, boxs, &errCode);
            trigger = false;

        }
        if (errCode != -1){
            TTF_Font *font;
            text txt;
            load_TTF_File("COURIER.TTF", &font, 20);
            int hspace = 870;
            switch (errCode) {
              case 0:
                  render_text(d->renderer, 100, hspace, "INVALID_INPUT_THE_PROGRAM_DOES_NOTHING", font, &txt.texture, &txt.rect);
              break;
              case 1:
                  render_text(d->renderer, 100, hspace, "TOO_LONG_IS_THE_CALCULATED_STRING", font, &txt.texture, &txt.rect);
              break;
              case 2:
                  render_text(d->renderer, 100, hspace, "Left and right brackets does not match!", font, &txt.texture, &txt.rect);
              break;
            }
            TTF_CloseFont(font);
            font = NULL;
        }
        drawBarSepLine(d->renderer);
        drawLabels(d->renderer);
        startButton(d->renderer);
        cursor(d->renderer, boxs, &i);
        SDL_UpdateWindowSurface(d->window);
    }
    *changeState = 0;
    deinitBoxs(boxs);
    SDL_DestroyTexture(lsystemTexture);
    for (int i = 0; i < 15; i++){
        free(data[i]);
    }
    free(data);
}


int SDL_lsystemMain(int n, char **argv){

    setbuf(stdout,NULL);

    display *d = malloc(sizeof(display));
    SDL_Init(SDL_INIT_VIDEO);
    int x = SDL_WINDOWPOS_UNDEFINED, y = SDL_WINDOWPOS_UNDEFINED;
    d->window = SDL_CreateWindow("Fractals", x, y, WIDTH, HEIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(d->window);
    d->renderer = SDL_CreateSoftwareRenderer(surface);
    SDL_SetRenderDrawBlendMode(d->renderer, SDL_BLENDMODE_BLEND);
    TTF_Init();

      int i;
      lsystemMenu(d, &i);

  //begin_release_resources------------------------
      TTF_Quit();
      SDL_DestroyRenderer(d->renderer);
      SDL_DestroyWindow(d->window);
      SDL_Quit();
    return 0;
}
