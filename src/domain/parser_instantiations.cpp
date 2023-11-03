#include "parser_def.hpp"

#include "include/loki/common/config.hpp"


namespace loki::domain::parser
{
    using iterator_type = loki::iterator_type;
    using phrase_context_type = loki::phrase_context_type;
    using context_type = loki::context_type;

    BOOST_SPIRIT_INSTANTIATE(
        name_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        variable_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        number_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        term_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        type_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        type_either_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        typed_list_of_names_recursively_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        typed_list_of_names_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        typed_list_of_variables_recursively_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(
        typed_list_of_variables_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(
        domain_description_type, iterator_type, context_type)
}
