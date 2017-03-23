NAME	 = myprog.out
CXX	 = g++
CXXFLAGS = -std=c++11 -lpthread -Wall -I$(INCDIR) -I$(INCDEPENDENCIES)

SRCDIR	 = ./src
INCDIR	 = ./include
INCDEPENDENCIES = ./dependencies
SOURCES	 = $(SRCDIR)/main.cpp $(SRCDIR)/environment.cpp $(SRCDIR)/gridhandler.cpp $(SRCDIR)/agent.cpp $(SRCDIR)/functions.cpp
OBJECTS	 = $(SOURCES:.cpp=.o)

all:	CXXFLAGS += -O3
all:  $(NAME)

debug:	CXXFLAGS += -g -DDEBUG
debug:	$(NAME)

$(NAME):  $(OBJECTS)
	$(CXX) $^ -o $@ $(CXXFLAGS)

$(SRCDIR)/%.o: %.cpp
	$(CXX) $^ -c $< $(CXXFLAGS)

clean:
	rm -f $(NAME)
	rm -f $(OBJECTS)
