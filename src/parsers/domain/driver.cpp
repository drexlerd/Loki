#include "driver.hpp"

#include <fstream>

#include "common/error_handler.hpp"
#include "stage_1_ast/parser.hpp"


namespace mimir::parsers::domain {

Driver::Driver(const fs::path& sketch_path)
    : m_sketch_path(sketch_path) { }

formalism::DomainDescription Driver::parse() {
    std::ifstream sketch_stream(this->m_sketch_path.c_str());
    if (sketch_stream.is_open()) {
        std::stringstream buffer;
        buffer << sketch_stream.rdbuf();
        std::string source = buffer.str();

        // Stage 1 parse
        auto result = stage_1::parser::parse_ast(source);

        // Stage 2 parse
        // TODO

        return formalism::DomainDescription(nullptr);
    }
    throw std::runtime_error("extended sketch file does not exist");
}

}
