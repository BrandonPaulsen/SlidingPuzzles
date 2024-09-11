#variables
COMPILE = g++
FLAGS = 
OBJS = game.o
OUT = source

#targets
all: main.cpp $(OBJS)
	$(COMPILE) $(FLAGS) -o $(OUT) main.cpp $(OBJS)
board.o: game.hpp game.cpp
	$(COMPILE) $(FLAGS) -c game.cpp
clean:
	rm -rf *.o $(OUT)