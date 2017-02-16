NAME	 = myprog
CXX	 = g++
CXXFLAGS = -std=c++11 -Wall -I$(INCDIR) -I$(INCDIREIGEN)

SRCDIR	 = ./src
INCDIR	 = ./include
INCDIREIGEN = /usr/local/include/eigen3
SOURCES	 = $(SRCDIR)/main.cpp $(SRCDIR)/environment.cpp $(SRCDIR)/gridhandler.cpp $(SRCDIR)/dynamicprog.cpp
OBJECTS	 = $(SOURCES:.cpp=.o)

all:	CXXFLAGS += -O3
all:  $(NAME)

debug:	CXXFLAGS += -g -DDEBUG
debug:	$(NAME)

$(NAME):  $(OBJECTS)
	$(CXX) $^ -o $@ $(CXXLFAGS)

$(SRCDIR)/%.o: %.cpp
	$(CXX) $^ -c $< $(CXXFLAGS)

clean:
	rm -f $(NAME)
	rm -f $(OBJECTS)