/*
font rendering
*/
struct text{
    SDL_Rect rect;
    SDL_Texture *texture;
};
typedef struct text text;

//load the file
void load_TTF_File(const char *path, TTF_Font **font, char fontsize);

//render simple text in white
void render_text(SDL_Renderer *renderer, int x, int y, char *text,
TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect);

//render wrapped text in white
void render_textc(SDL_Renderer *renderer, int x, int y, char *text,
TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect, SDL_Color col);

//coloured text
void render_text2(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect, int width);
