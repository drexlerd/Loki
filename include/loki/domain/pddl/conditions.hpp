#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_CONDITIONS_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_CONDITIONS_HPP_

#include "declarations.hpp"

#include <string>


namespace loki {
template<typename T>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
/// @brief Provides an interface for visiting nodes in a DAG of conditions.
class ConditionVisitor {
public:
    virtual void visit(const ConditionLiteral& condition) = 0;
};


class ConditionImpl {
protected:
    int m_identifier;

    ConditionImpl(int identifier);

public:
    virtual ~ConditionImpl();

    virtual size_t hash() const = 0;

    /// @brief Accepts the visitor by calling the visit overload.
    virtual void accept(ConditionVisitor& visitor) const = 0;
};


class ConditionLiteralImpl : public ConditionImpl, std::enable_shared_from_this<ConditionLiteralImpl> {
private:
    Literal m_literal;

    ConditionLiteralImpl(int identifier, const Literal& literal);

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    ~ConditionLiteralImpl() override;

    bool operator==(const ConditionLiteralImpl& other) const;
    bool operator!=(const ConditionLiteralImpl& other) const;
    bool operator<(const ConditionLiteralImpl& other) const;
    bool operator>(const ConditionLiteralImpl& other) const;

    size_t hash() const override;

    void accept(ConditionVisitor& visitor) const override;

    const Literal& get_literal() const;
};

}


namespace std {
    template<>
    struct hash<loki::pddl::ConditionLiteralImpl>
    {
        std::size_t operator()(const loki::pddl::ConditionLiteralImpl& condition) const;
    };
}


#endif
