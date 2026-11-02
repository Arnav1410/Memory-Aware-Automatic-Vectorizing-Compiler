CXX ?= g++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -O2

all: vectorizer_demo

vectorizer_demo: vector_ir.cpp vector_ir.hpp vectorizer_demo.cpp
	$(CXX) $(CXXFLAGS) vector_ir.cpp vectorizer_demo.cpp -o $@

clean:
	rm -f vectorizer_demo *.o

.PHONY: all clean test


test: vectorizer_demo
	./vectorizer_demo
