/*
 * Copyright (C) 2023 Dominik Drexler
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

#include "parser_def.hpp"

#include "../../../include/loki/common/ast/config.hpp"


namespace loki::common::parser
{
    using iterator_type = loki::iterator_type;
    using phrase_context_type = loki::phrase_context_type;
    using context_type = loki::context_type;

    BOOST_SPIRIT_INSTANTIATE(name_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(variable_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(function_symbol_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(function_term_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(term_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(number_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(predicate_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(undefined_type, iterator_type, context_type)


    BOOST_SPIRIT_INSTANTIATE(define_keyword_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(domain_keyword_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(problem_keyword_type, iterator_type, context_type)
}
