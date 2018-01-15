/*
complex number library
*/

#include <mpfr.h>

struct complex{
//imaginary part
    mpfr_t r;
//real part
    mpfr_t i;
};
typedef struct complex complex;

//initialization
void init_complex(complex *c);

//set the value
void set_complex(complex *c, mpfr_t a, mpfr_t b);

//addition
void CAdd(complex *a, complex b);

//subtraction
void CSub(complex *a, complex b);

//multiplication
void CMul(complex *a, complex b);

//norm of a vector
void CNorm(complex *a);

//modulus of a vector
void CMod(complex *a);

//print the complex number
void complex_printf(complex a);

//free the rubbish
void complex_clear(complex *c);
