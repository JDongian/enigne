CC=g++
FILES=life2.cpp
OPT=-Wall -std=c++11
LIB=-lSDL2 -lSDL2_ttf
DBG=-g
OUT=life

build: $(FILES)
	$(CC) -o $(OUT) $(FILES) $(OPT) -O3 $(LIB)

debug: $(FILES)
	$(CC) -o $(OUT) $(FILES) $(OPT) $(LIB) $(DBG)

clean:
	rm -f *.o core

rebuild: clean build
