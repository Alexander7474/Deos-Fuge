CC = g++
CFLAGS = -std=c++11 -Wall -I/usr/include/freetype2
LIBS = -lbbop -lglfw -lGLEW -lGL -lfreetype 
SRCS = main.cpp src/personnage.cpp src/map.cpp src/game.cpp
OBJS = $(SRCSM:.cpp=.o) $(SRCS:.cpp=.o)

all: final

final: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f final $(OBJS)
	rm -rf /usr/local/shaders
