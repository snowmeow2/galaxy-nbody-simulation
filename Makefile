CC = g++
FLAGS = -fopenmp -std=c++11
EXE = simulation
OBJS = main.o tree.o function.o

$(EXE): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(EXE)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@

.DEFAULT_GOAL := all

all: $(EXE) clean

clean:
	rm -f $(OBJS)