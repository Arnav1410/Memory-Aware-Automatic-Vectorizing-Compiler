CXX ?= g++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -O2 -I../common

all: tinyc3

tinyc3: tinyc3.tab.o lex.yy.o translator.o ../common/target_info.o main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

tinyc3.tab.c: tinyc3.y translator.hpp ../common/target_info.hpp
	bison -d -o tinyc3.tab.c tinyc3.y

tinyc3.tab.h: tinyc3.tab.c
	@test -f tinyc3.tab.h

lex.yy.c: tinyc3.l tinyc3.tab.h
	flex tinyc3.l

tinyc3.tab.o: tinyc3.tab.c translator.hpp ../common/target_info.hpp
	$(CXX) $(CXXFLAGS) -c tinyc3.tab.c -o $@

lex.yy.o: lex.yy.c tinyc3.tab.h translator.hpp
	$(CXX) $(CXXFLAGS) -c lex.yy.c -o $@

translator.o: translator.cpp translator.hpp ../common/target_info.hpp
	$(CXX) $(CXXFLAGS) -c translator.cpp -o $@

../common/target_info.o: ../common/target_info.cpp ../common/target_info.hpp
	$(CXX) $(CXXFLAGS) -c ../common/target_info.cpp -o $@

main.o: main.cpp translator.hpp ../common/target_info.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@

clean:
	rm -f tinyc3 tinyc3.tab.c tinyc3.tab.h lex.yy.c *.o ../common/target_info.o

.PHONY: all clean test


test: tinyc3
	bash tests/run_tests.sh
