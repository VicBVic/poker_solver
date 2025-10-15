COMPILER = g++
CFLAGS = -Wall -Wextra -std=c++17 -O3 -g
LDFLAGS =

SOURCES = card_format.cpp card_set.cpp poker_hand.cpp main.cpp poker_math.cpp ev_solver.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = poker_solver_demo

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(COMPILER) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(COMPILER) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)