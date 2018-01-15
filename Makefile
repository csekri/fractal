.PHONY: complex mandel font triangle menu main
GCC = gcc -std=c99 -O3 -pedantic -Wall -o main

complex:
	$(GCC) -DComplexMain=main complex.c -lmpfr -lgmp
	./main

mandel:
	$(GCC) -DMandelMain=main mandel.c font.c complex.c -lm -lmpfr -lgmp -lSDL2 -lSDL2_ttf

font:
	$(GCC) -DFontMain=main font.c -lm -lmpfr -lgmp -lSDL2 -lSDL2_ttf
	./main

triangle:
	$(GCC) -DTriangle_furtherMain=main triangle_further.c -lm
	./main

l-system:
	$(GCC) -DLsystemMain=main l-system.c -lm
	./main

drawlsystem:
	$(GCC) -DSDL_lsystemMain=main sdl_lsystem.c menu.c font.c l-system.c triangle_further.c -lm -lSDL2 -lSDL2_ttf
	./main

menu:
	$(GCC) -DMenuMain=main menu.c triangle_further.c font.c -lm -lmpfr -lgmp -lSDL2 -lSDL2_ttf
	./main

main:
	$(GCC) main.c sdl_lsystem.c stack.c menu.c triangle_further.c guide.c buddha.c mandel.c julia.c font.c l-system.c complex.c -lm -lmpfr -lgmp -lSDL2 -lSDL2_ttf
	./main
