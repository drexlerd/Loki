#include "../include/parser.hpp"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <fstream>

#include "ast.hpp"
#include "parser_api.hpp"
#include "error_handler.hpp"
#include "config.hpp"


namespace mimir::pddl::parser {
DomainParser::DomainParser(const fs::path& domain_file)
    : m_domain_file(domain_file) { }

void DomainParser::parse(){
    std::ifstream sketch_stream(this->m_domain_file.c_str());
    if (sketch_stream.is_open()) {
        std::stringstream buffer;
        buffer << sketch_stream.rdbuf();
        std::string source = buffer.str();

        using mimir::pddl::parser::iterator_type;
        iterator_type iter(source.begin());
        iterator_type const end(source.end());

        // Our AST
        mimir::pddl::ast::DomainDescription ast;

        // Our error handler
        using boost::spirit::x3::with;
        using mimir::pddl::parser::error_handler_type;
        using mimir::pddl::parser::error_handler_tag;
        std::stringstream out;
        error_handler_type error_handler(iter, end, out); // Our error handler

        // Our parser
        auto const parser =
            // we pass our error handler to the parser so we can access
            // it later on in our on_error and on_sucess handlers
            with<error_handler_tag>(std::ref(error_handler))
            [
                mimir::pddl::domain_description()
            ];

        // Go forth and parse!
        using boost::spirit::x3::ascii::space;
        bool success = phrase_parse(iter, end, parser, space, ast) && iter != end;
        if (!success)
        {
            std::cerr << out.str() << std::endl;
        }

        return;
    }
    throw std::runtime_error("domain file does not exist");
}

}

