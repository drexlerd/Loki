include(FetchContent)

FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.8.3
)

# Make Google Benchmark available
FetchContent_MakeAvailable(googlebenchmark)

# Now declare your executable
add_executable(persistent_factory "persistent_factory.cpp")
target_link_libraries(persistent_factory parsers)
target_link_libraries(persistent_factory benchmark::benchmark)