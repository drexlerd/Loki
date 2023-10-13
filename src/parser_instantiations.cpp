#include "parser_def.hpp"
#include "config.hpp"


namespace mimir::pddl { namespace parser
{
    BOOST_SPIRIT_INSTANTIATE(
        domain_description_type, iterator_type, context_type)
}}
