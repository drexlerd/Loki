#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_CONDITIONS_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_CONDITIONS_HPP_

#include "declarations.hpp"


#include <string>

namespace loki::pddl {
/// @brief Provides an interface for visiting nodes in a DAG of conditions.
class ConditionVisitor {
public:
    virtual void visit(const ConditionLiteral& condition) = 0;
};


class ConditionImpl {
private:
    int m_index;

public:
    ConditionImpl(int index);
    virtual ~ConditionImpl();

    /// @brief Accepts the visitor by calling the visit overload.
    virtual void accept(ConditionVisitor& visitor) const = 0;
};


class ConditionLiteralImpl : public ConditionImpl {
private:
    Literal m_literal;

public:
    ConditionLiteralImpl(int index);
    ~ConditionLiteralImpl() override;

    void accept(ConditionVisitor& visitor) const override;

    const Literal& get_literal() const;
};

}

#endif
