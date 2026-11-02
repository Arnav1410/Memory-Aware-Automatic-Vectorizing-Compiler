.PHONY: all build btp-tools clean tinyc1 tinyc2 tinyc3 backend vectorization \
        test test-tinyc1 test-tinyc2 test-tinyc3 test-backend test-vectorization test-btp-scalar test-p3-suite

# Default build: the small backend/vectorization tools.
all: btp-tools

# Build every part of the project.
build: tinyc1 tinyc2 tinyc3 btp-tools

# Tools used after TinyC Part 3.
btp-tools: backend vectorization

backend:
	$(MAKE) -C backend

vectorization:
	$(MAKE) -C vectorization

tinyc1:
	$(MAKE) -C tinyc_part1

tinyc2:
	$(MAKE) -C tinyc_part2

tinyc3:
	$(MAKE) -C tinyc_part3

# Build everything needed for the first end-to-end BTP experiment.
test-btp-scalar: tinyc3 backend
	bash scripts/test_scalar_pipeline.sh

test-backend: backend
	$(MAKE) -C backend test

test-vectorization: vectorization
	$(MAKE) -C vectorization test

test-tinyc1: tinyc1
	./tinyc_part1/tinyc1 tinyc_part1/test.c

test-tinyc2: tinyc2
	./tinyc_part2/tinyc2 tinyc_part2/test.c /tmp/tinyc2_output.txt
	cat /tmp/tinyc2_output.txt

test-tinyc3: tinyc3
	$(MAKE) -C tinyc_part3 test

# Run the non-interactive tests. Part 1/2 tests print their output.
test: test-backend test-vectorization test-tinyc3

clean:
	$(MAKE) -C backend clean
	$(MAKE) -C vectorization clean
	$(MAKE) -C tinyc_part1 clean || true
	$(MAKE) -C tinyc_part2 clean || true
	$(MAKE) -C tinyc_part3 clean || true


test-p3-suite: tinyc3
	bash tinyc_part3/tests/run_tests.sh

.PHONY: test-memory test-memory-unit benchmark-memory demo-memory test-source-archive test-runtime calibrate-memory evaluate-memory present-memory

test-memory-unit: tinyc3 backend
	mkdir -p build/memory
	./tinyc_part3/tinyc3 examples/memory/kernels.c targets/x86_64_tinyc.conf build/memory/kernels.tac > build/memory/frontend.log
	$(CXX) -std=c++17 -Wall -Wextra -O2 tests/memory/unit.cpp backend/ir.cpp backend/memory/program.cpp backend/memory/analysis.cpp backend/memory/cost.cpp -o build/memory/unit
	./build/memory/unit build/memory/kernels.tac

test-memory: test-memory-unit
	python3 tests/memory/run.py
	python3 tests/memory/forward.py

benchmark-memory: tinyc3 backend
	python3 benchmarks/run.py

demo-memory: test-p3-suite test-btp-scalar test-memory benchmark-memory present-memory benchmark-forward

test-source-archive:
	python3 scripts/test_source_archive.py

test-runtime: tinyc3 backend
	python3 tests/memory/runtime.py

calibrate-memory: tinyc3 backend
	python3 benchmarks/calibrate.py

evaluate-memory: tinyc3 backend
	python3 benchmarks/evaluate.py

present-memory: tinyc3 backend
	python3 scripts/present_memory.py

.PHONY: test-forward benchmark-forward
test-forward: test-memory-unit
	python3 tests/memory/forward.py

benchmark-forward: tinyc3 backend
	python3 benchmarks/forward.py

.PHONY: test-applications benchmark-applications report-applications
test-applications: tinyc3 backend
	python3 benchmarks/applications/test_metadata.py
	python3 benchmarks/applications/run.py check

benchmark-applications: tinyc3 backend
	python3 benchmarks/applications/run.py bench

report-applications:
	python3 benchmarks/applications/report.py

.PHONY: benchmark-application-stack-control
benchmark-application-stack-control:
	python3 benchmarks/applications/stack_probe.py
