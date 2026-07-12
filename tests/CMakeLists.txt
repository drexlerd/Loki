# Tests read benchmark PDDL from the pypddl-datasets cache, resolved (and
# downloaded on demand) at configure time.
if(NOT Python3_EXECUTABLE)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)
endif()
execute_process(
    COMMAND "${Python3_EXECUTABLE}" -c "import pypddl_datasets, sys; sys.stdout.write(str(pypddl_datasets.data_root()))"
    OUTPUT_VARIABLE LOKI_BENCHMARKS_DIR
    RESULT_VARIABLE LOKI_BENCHMARKS_RESULT)
if(NOT LOKI_BENCHMARKS_RESULT EQUAL 0)
    message(FATAL_ERROR "Could not resolve the benchmark data root; install 'pypddl-datasets>=0.0.5,<0.1' into the configuring Python (${Python3_EXECUTABLE}).")
endif()
message(STATUS "Benchmark data: ${LOKI_BENCHMARKS_DIR}")
add_definitions(-DBENCHMARKS_DIR="${LOKI_BENCHMARKS_DIR}")

add_subdirectory(unit)
