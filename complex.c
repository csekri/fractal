#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include <mpfr.h>

#include "complex.h"

/*struct complex{
//imaginary part
    mpf_t r;
//real part
    mpf_t i;
};*/

void init_complex(complex *c){
    mpfr_init(c->r);
    mpfr_init(c->i);
}

void set_complex(complex *c, mpfr_t a, mpfr_t b){
    mpfr_set(c->r, a, MPFR_RNDD);
    mpfr_set(c->i, b, MPFR_RNDD);

}

void CAdd(complex *a, complex b){
    mpfr_add(a->r, a->r, b.r, MPFR_RNDD);
    mpfr_add(a->i, a->i, b.i, MPFR_RNDD);
}

void CSub(complex *a, complex b){
    mpfr_add(a->r, a->r, b.r, MPFR_RNDD);
    mpfr_add(a->i, a->i, b.i, MPFR_RNDD);
}

void CMul(complex *a, complex b){
    mpfr_t num1, num2;
    mpfr_t d, e;
    mpfr_init(d);
    mpfr_init(e);
    mpfr_init(num1);
    mpfr_init(num2);
    mpfr_mul(num1, a->r, b.r, MPFR_RNDD);
    mpfr_mul(num2, a->i, b.i, MPFR_RNDD);
    mpfr_sub(d, num1, num2, MPFR_RNDD);
    mpfr_mul(num1, a->r, b.i, MPFR_RNDD);
    mpfr_mul(num2, a->i, b.r, MPFR_RNDD);
    mpfr_add(e, num1, num2, MPFR_RNDD);
    set_complex(a, d, e);
    mpfr_clear(num1);
    mpfr_clear(num2);
    mpfr_clear(d);
    mpfr_clear(e);
}

void CNorm(complex *a){
    mpfr_t num1, num2;
    mpfr_init(num1);
    mpfr_init(num2);
    mpfr_mul(num1, a->r, a->r, MPFR_RNDD);
    mpfr_mul(num2, a->i, a->i, MPFR_RNDD);
    mpfr_add(a->r, num1, num2, MPFR_RNDD);
    mpfr_set_d(a->i, 0, MPFR_RNDD);
    mpfr_clear(num1);
    mpfr_clear(num2);
}

void CMod(complex *a){
    CNorm(a);
    mpfr_sqrt(a->r, a->r, MPFR_RNDD);
}

void complex_printf(complex a){
    mpfr_printf("Real part:      %.40Rf\nimaginary part: %.40Rf\n", a.r, a.i);
}

void complex_clear(complex *c){
    mpfr_clear(c->r);
    mpfr_clear(c->i);
}

int ComplexMain(){
    mpfr_set_default_prec (1000);
    mpfr_t number;
    mpfr_init(number);
    mpfr_t num;
    mpfr_init(num);
    mpfr_t num2;
    mpfr_init(num2);
    mpfr_t num3;
    mpfr_init(num3);
    mpfr_set_str(number, "3.14159265358979486487356382653872956837", 10, MPFR_RNDD);
    mpfr_set_str(num, "5.14159265358979486264876382653872956837", 10, MPFR_RNDD);
    mpfr_set_str(num2, "7.5325", 10, MPFR_RNDD);
    mpfr_set_str(num3, "9.0", 10, MPFR_RNDD);

    complex c,d;
    init_complex(&c);
    init_complex(&d);
    mpfr_set(c.r, number, MPFR_RNDD);
    mpfr_set(c.i, num, MPFR_RNDD);
    set_complex(&d, num2, num3);
    CAdd(&d, c);
    CNorm(&c);
    complex_printf(d);
    complex_printf(c);



    return 0;
}
