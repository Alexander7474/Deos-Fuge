CC = g++
CFLAGS = -std=c++11 -Wall -I/opt/homebrew/Cellar/freetype/2.13.2/include/freetype2 -I/opt/homebrew/Cellar/glew/2.2.0_1/include/ -I/usr/local/include/GLFW

LIBS =  -lbbop -L/opt/homebrew/Cellar/freetype/2.13.2/lib -L/opt/homebrew/Cellar/glew/2.2.0_1/lib -framework Cocoa -framework OpenGL -framework IOKit -lfreetype  -lglfw3 -lGLEW.2.2.0 

SRCS = main.cpp src/personnage.cpp src/map.cpp src/game.cpp src/player.cpp src/perso_info.cpp src/menu.cpp

OBJS = $(SRCSM:.cpp=.o) $(SRCS:.cpp=.o)

all: final

final: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f final $(OBJS)
	rm -rf /usr/local/shaders
