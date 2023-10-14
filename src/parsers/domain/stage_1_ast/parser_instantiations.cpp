#include "parser_def.hpp"

#include "src/parsers/common/config.hpp"


namespace mimir::parsers::domain::stage_1::parser
{
    BOOST_SPIRIT_INSTANTIATE(
        domain_description_type, iterator_type, context_type)
}
