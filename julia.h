
struct color{
	  int r,g,b;
};
typedef struct color color;

struct zoomData{
		int num_it;
		double newZoom;
};
typedef struct zoomData zoomData;

enum colorM { STRIPED = 0, LAYERED = 1, SMOOTH = 2 };

long int juliaConverge(complex *c, long int num_it, complex comp);

long int mandelConverge(complex *c, long int num_it);

void mandelColorLayered(long int i, int *r, int *g, int *b);

void mandelColorStriped(complex escape, long int i, int *r, int *g, int *b);

void mandelColorSmooth(complex escape, long long int i, int *r, int *g, int *b);

void zoomFunc(int mouse_x, int mouse_y, mpfr_t *midx, mpfr_t *midy,
	 mpfr_t left, mpfr_t up, long double *zoom, bool *draw, bool isIn, zoomData zd);

void zoomInOut(mpfr_t *midx, mpfr_t *midy, mpfr_t left, mpfr_t up, long double *zoom, bool *draw, bool *exit_, zoomData *zd, bool *drawCorrespondingJulia);

void calculateLeftAndUp(mpfr_t *left, mpfr_t *up, mpfr_t midx, mpfr_t midy, long double zoom);

void drawJulia(display *d, mpfr_t *midx, mpfr_t *midy, long double zoom, bool *exit_, SDL_Event *event, int colorMethod, zoomData zd, complex comp);

void runJulia(display *d, int *changeState, SDL_Event *event);
