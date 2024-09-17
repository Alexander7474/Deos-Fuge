CC = g++
CFLAGS = -std=c++11 -Wall -I/usr/include/freetype2
LIBS = -L/usr/local/lib/ -lbbop -lglfw3 -lGLEW -lGL -lfreetype  -lLDtkLoader

SRCS = main.cpp src/particle.cpp src/personnages/personnage.cpp src/map.cpp src/game.cpp src/player.cpp src/menu/menu.cpp src/personnages/perso_info.cpp src/personnages/knight.cpp src/personnages/white_knight.cpp

OBJS = $(SRCSM:.cpp=.o) $(SRCS:.cpp=.o)

all: final

final: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f final $(OBJS)
	rm -rf /usr/local/shaders
