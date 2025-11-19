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

int main(int argc, char** argv)
{
    auto program = argparse::ArgumentParser("AStar search.");
    program.add_argument("-D", "--domain-filepath").required().help("The path to the PDDL domain file.");
    program.add_argument("-P", "--problem-filepath").default_value("").help("The path to the PDDL problem file.");
    program.add_argument("-OD", "--out-domain-filepath").default_value("").help("The path to the output PDDL domain file.");
    program.add_argument("-OP", "--out-problem-filepath").default_value("").help("The path to the output PDDL problem file.");
    program.add_argument("-S", "--strict").default_value(false).implicit_value(true).help("Enable strict parsing mode.");
    program.add_argument("-Q", "--quiet").default_value(true).implicit_value(false).help("Disable quiet mode.");
    /* TODO(Dominik): add translator options */
    program.add_argument("-T", "--remove-typing").default_value(false).implicit_value(true).help("Enable removal of typing.");

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

    auto domain_filepath = program.get<std::string>("--domain-filepath");
    auto problem_filepath = program.get<std::string>("--problem-filepath");
    auto out_domain_filepath = program.get<std::string>("--out-domain-filepath");
    auto out_problem_filepath = program.get<std::string>("--out-problem-filepath");

    auto parser_options = loki::ParserOptions();
    parser_options.quiet = program.get<bool>("--quiet");
    parser_options.strict = program.get<bool>("--strict");

    auto translator_options = loki::TranslatorOptions();
    translator_options.remove_typing = program.get<bool>("--remove-typing");

    auto parser = loki::Parser(domain_filepath, parser_options);
    const auto domain = parser.get_domain();

    const auto domain_translation_result = loki::translate(domain, translator_options);
    std::cout << *domain_translation_result.get_translated_domain() << std::endl;

    if (!out_domain_filepath.empty())
    {
        auto out_domain_file = std::ofstream(out_domain_filepath);
        out_domain_file << *domain_translation_result.get_translated_domain();
        out_domain_file.close();
    }

    if (!problem_filepath.empty())
    {
        auto problem = parser.parse_problem(problem_filepath, parser_options);

        const auto translated_problem = loki::translate(problem, domain_translation_result, translator_options);
        std::cout << *translated_problem << std::endl;

        if (!out_problem_filepath.empty())
        {
            auto out_problem_file = std::ofstream(out_problem_filepath);
            out_problem_file << *translated_problem;
            out_problem_file.close();
        }
    }

    return 0;
}
