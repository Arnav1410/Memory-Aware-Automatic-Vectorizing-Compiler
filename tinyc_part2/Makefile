CXX ?= g++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -O2

all: tinyc2

tinyc2: tinyc2.tab.o lex.yy.o ast.o main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

tinyc2.tab.c: tinyc2.y ast.hpp
	bison -d -o tinyc2.tab.c tinyc2.y

tinyc2.tab.h: tinyc2.tab.c
	@test -f tinyc2.tab.h

lex.yy.c: tinyc2.l tinyc2.tab.h
	flex tinyc2.l

tinyc2.tab.o: tinyc2.tab.c ast.hpp
	$(CXX) $(CXXFLAGS) -c tinyc2.tab.c -o $@

lex.yy.o: lex.yy.c tinyc2.tab.h ast.hpp
	$(CXX) $(CXXFLAGS) -c lex.yy.c -o $@

ast.o: ast.cpp ast.hpp
	$(CXX) $(CXXFLAGS) -c ast.cpp -o $@

main.o: main.cpp ast.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@

clean:
	rm -f tinyc2 tinyc2.tab.c tinyc2.tab.h lex.yy.c *.o

.PHONY: all clean
