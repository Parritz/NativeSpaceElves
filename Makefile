all:
	g++ -g --std=c++17 -I./include -L./lib ./src/*.cpp ./src/glad.c -lglfw3dll -o spaceelves