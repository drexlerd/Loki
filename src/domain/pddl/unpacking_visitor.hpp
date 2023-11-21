#ifndef LOKI_SRC_DOMAIN_PDDL_UNPACK_VISITOR_HPP_
#define LOKI_SRC_DOMAIN_PDDL_UNPACK_VISITOR_HPP_

#include "../../../include/loki/common/ast/config.hpp"
#include "../../../include/loki/domain/pddl/parser.hpp"

#include <boost/variant.hpp>

#include <functional>
#include <iostream>
#include <vector>
#include <tuple>

namespace loki {

/// @brief Given a variant<T1,...,TN> this function allows visiting the variant
///        and pushing back in a respective vector<Ti>.
///        In the constructor, there must be given a vector<Ti> for all 1...N.
template<typename... Ts>
class UnpackingVisitor {
private:
    const error_handler_type& error_handler;
    domain::Context& context;
    std::tuple<std::reference_wrapper<Ts>...> result;

public:
    UnpackingVisitor(const error_handler_type& error_handler_, domain::Context& context_, Ts& ...result)
        : error_handler(error_handler_), context(context_), result(std::make_tuple(std::ref(result)...)) { }

    template<typename T>
    void operator()(const T& element) {
        std::get<std::reference_wrapper<std::vector<T>>>(result).get().push_back(element);
    }
};

};

#endif  // LOKI_SRC_DOMAIN_PDDL_INSERT_VISITOR_HPP_
