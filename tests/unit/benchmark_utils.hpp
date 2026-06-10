/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_TESTS_BENCHMARK_UTILS_HPP_
#define LOKI_TESTS_BENCHMARK_UTILS_HPP_

#include <filesystem>
#include <gtest/gtest.h>

namespace loki::tests
{

inline bool benchmark_file_available(const std::filesystem::path& path) { return std::filesystem::is_regular_file(path); }

inline bool benchmark_tree_available(const std::filesystem::path& path)
{
    if (!std::filesystem::is_directory(path))
        return false;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".pddl")
            return true;
    }
    return false;
}

template<typename Cases>
inline bool benchmark_suite_available(const Cases& cases)
{
    return !cases.empty() && benchmark_file_available(cases.front().domain_file) && benchmark_file_available(cases.front().task_file);
}

#define LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(path) ASSERT_TRUE(::loki::tests::benchmark_file_available(path)) << path

#define LOKI_SKIP_IF_BENCHMARK_FILE_UNAVAILABLE(path)               \
    do                                                              \
    {                                                               \
        if (!::loki::tests::benchmark_file_available(path))         \
            GTEST_SKIP() << "Benchmark data unavailable: " << path; \
    } while (false)

}  // namespace loki::tests

#endif
