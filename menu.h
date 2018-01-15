/*
draws the menu
*/

//draw menu, called in main
void drawMenu(display *d, TTF_Font *font,
  bool *running, SDL_Event *event, int *currentMenuItem);

//mouse pointer in a rectangle
bool isMousePointerInRect(int l, int r, int a, int b, int mouse_x, int mouse_y);
