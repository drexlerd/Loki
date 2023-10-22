#include "parser_def.hpp"

#include "include/loki/common/config.hpp"


namespace loki::domain::parser
{
    using iterator_type = loki::common::iterator_type;
    using phrase_context_type = loki::common::phrase_context_type;
    using context_type = loki::common::context_type;

    BOOST_SPIRIT_INSTANTIATE(
        name_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        domain_description_type, iterator_type, context_type)
}
