#ifndef LOKI_INCLUDE_LOKI_COMMON_PRINTER_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_PRINTER_HPP_


namespace loki {

struct FormattingOptions {
    // The indentation in the current level.
    int indent = 0;
    // The amount of indentation added per nesting
    int add_indent = 0;
};

}

#endif