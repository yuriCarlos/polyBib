cc = gcc 

all: test.c
	$(CC) polyBib.c test.c -o test -lglut -lGL -lGLU -lm

clean:
	rm test
