Kristof Cseh
University of Bristol
Faculty of Engineering
Department of Computer Science

This readme is the manual to compile the program and describes the modules.

Beyond the standard libraries in C, plenty other libraries are used but I tried to find libraries which are cross-platform.

The enumerated list of non-standard libraries I used:

1. MPFR
The MPFR library is a C library for multiple-precision floating-point computations with correct rounding.
I used this library to do multiprecision calculations with complex numbers.

Linux install:
sudo apt-get install libmpfr-dev

Before this I used GMP and that is installed as well so we may need gmp as well if these libraries are dependent.
Install:
sudo apt-get install libgmp3-dev

2. SDL2
Graphics purpose. Install:
sudo apt-get install libsdl2-dev

3. SDL2-ttf
This is to render font. I downloaded a ttf file for this which is Courier.
Install:
sudo apt-get install libsdl2-ttf-dev

To compile the project type

    make main

in the terminal window.

MODULES:

main.c

This is the main module. Everything is put here.

sdl_lsystem.c

This module is responsible for the graphics related to the Lindenmayer systems and the user interface there.

stack.c

This module is picked from the lecture slides but the integer is rewritten to a state structure. It is used to save and load position and angle in the Lindenmayer systems.

menu.c

This handles and draws the main menu with the clickable buttons.

triangle_further.c

This is my first assessed homework but here in another context. With this we devide the button rectangles into two triangles with the diagonals and detect whether the mouse pointer is above the button.

guide.c

This module draws a scrollable text (with mouse wheel, mouse click and drag, arrows) with an actual readme and explanation about the math background of these things. Some text is from wikipedia.

font.c

This module has functions that we can use to load .ttf files and render texts.

buddha.c

This module has a little double complex number library some functions to handle random variables with different distributions and most importantly to render the Buddhabrot fractal.

julia.c

This module is responsible to the maths and graphics of quadratic Julia sets.

mandel.c

This module is responsible to the maths and graphics of quadratic Mandelbrot sets.

l-system.c

This module is one of my former assessed homeworks. I did some basic changes so now the autotesting gives segfault, but runs properly in the main.c without any undefined behaviour.

complex.c

This module is a basic multiprecision library (powered by MPFR) and this routine is used in both julia.c and mandel.c.

/Note: in the program we can go back by clicking exit button or pressing <ENTER> or pressing <BACKSPACE>/
