#variables
COMPILE = g++
FLAGS = 
OBJS = 
OUT = source

#targets
all: main.cpp $(OBJS)
	$(COMPILE) $(FLAGS) -o $(OUT) main.cpp $(OBJS)
clean:
	rm -rf *.o $(OUT)