/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <argparse/argparse.hpp>
#include <fstream>
#include <iostream>
#include <loki/loki.hpp>

static constexpr std::string version = "1.0.0";

static void add_version(argparse::ArgumentParser& program)
{
    program.add_argument("-V", "--version")
        .nargs(0)
        .action(
            [&](const auto&)
            {
                std::cout << version << std::endl;
                std::exit(0);
            })
        .help("Print version information and exit.");
}

int main(int argc, char** argv)
{
    auto program = argparse::ArgumentParser("loki", version, argparse::default_arguments::help);
    add_version(program);

    program.add_argument("domain").required().help("The path to the PDDL domain file.");
    program.add_argument("problem").default_value(std::string {}).help("The path to the PDDL problem file.");
    program.add_argument("-d", "--out-domain").help("The path to the output PDDL domain file.");
    program.add_argument("-p", "--out-problem").help("The path to the output PDDL problem file.");
    program.add_argument("-s", "--strict")
        .default_value(false)
        .implicit_value(true)
        .help("Enable strict parsing mode to catch unused objects, predicates, functions, and parameters.");
    program.add_argument("-v", "--verbose").default_value(false).implicit_value(true).help("Enable verbose console prints.");
    program.add_argument("--remove-typing").default_value(false).implicit_value(true).help("Enable the removal of type annotations.");

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err)
    {
        std::cerr << err.what() << "\n";
        std::cerr << program;
        std::exit(1);
    }

    auto domain_filepath = program.get<std::string>("domain");
    auto problem_filepath = program.get<std::string>("problem");

    auto parser_options = loki::ParserOptions();
    parser_options.verbose = program.get<bool>("--verbose");
    parser_options.strict = program.get<bool>("--strict");

    auto translator_options = loki::TranslatorOptions();
    translator_options.remove_typing = program.get<bool>("--remove-typing");

    auto parser = loki::Parser(domain_filepath, parser_options);
    const auto domain = parser.get_domain();

    const auto domain_translation_result = loki::translate(domain, translator_options);
    if (parser_options.verbose)
        std::cout << *domain_translation_result.get_translated_domain() << std::endl;

    if (program.is_used("--out-domain"))
    {
        auto out_domain_filepath = program.get<std::string>("--out-domain");
        auto out_domain_file = std::ofstream(out_domain_filepath);
        out_domain_file << *domain_translation_result.get_translated_domain();
        out_domain_file.close();
    }

    if (!problem_filepath.empty())
    {
        auto problem = parser.parse_problem(problem_filepath, parser_options);

        const auto translated_problem = loki::translate(problem, domain_translation_result, translator_options);
        if (parser_options.verbose)
            std::cout << *translated_problem << std::endl;

        if (program.is_used("--out-problem"))
        {
            auto out_problem_filepath = program.get<std::string>("--out-problem");
            auto out_problem_file = std::ofstream(out_problem_filepath);
            out_problem_file << *translated_problem;
            out_problem_file.close();
        }
    }

    return 0;
}
