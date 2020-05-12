# DEBUG=-Wfatal-errors

all: build run

build: 
	g++ -w -std=c++14 $(DEBUG) \
	./src/*.cpp \
	-o game \
	-I"./lib/lua" \
	-L"./lib/lua" \
	-llua \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_mixer;

clean:
	rm ./game

run: 
	./game;