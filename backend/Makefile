CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2

MEMORY_SOURCES = memory/program.cpp memory/analysis.cpp memory/cost.cpp memory/emitter.cpp

all: ir_simulator x86_64_backend

ir_simulator: ir.cpp ir.hpp ir_simulator.cpp
	$(CXX) $(CXXFLAGS) ir.cpp ir_simulator.cpp -o $@

x86_64_backend: ir.cpp ir.hpp x86_64_backend.cpp $(MEMORY_SOURCES) memory/compiler.hpp ../common/target_info.cpp ../common/target_info.hpp
	$(CXX) $(CXXFLAGS) -I../common ir.cpp ../common/target_info.cpp x86_64_backend.cpp $(MEMORY_SOURCES) -o $@

clean:
	rm -f ir_simulator x86_64_backend *.o

.PHONY: all clean test


test: all
	bash ../scripts/run_smoke_tests.sh
