all:
	gcc bmp_maker.c -o bmp_maker
	./bmp_maker
	rm bmp_maker