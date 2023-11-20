#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_CONDITIONS_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_CONDITIONS_HPP_

#include "declarations.hpp"
#include "../../problem/pddl/declarations.hpp"

#include <string>


namespace loki {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
/// @brief Defines an interface for visiting nodes in a DAG of conditions.
class ConditionVisitor {
public:
    virtual void visit(const ConditionLiteral& condition) = 0;
    virtual void visit(const ConditionAnd& condition) = 0;
};


/* BaseCondition */
/// @brief Defines the common base class for conditions.
///        We use polymorphism instead of variant,
///        since we wrap conditions into shared_ptr.
class ConditionImpl {
protected:
    int m_identifier;

    ConditionImpl(int identifier);

public:
    virtual ~ConditionImpl();

    /// @brief Test for structural equivalence
    virtual bool operator==(const ConditionImpl& other) const = 0;
    virtual bool operator!=(const ConditionImpl& other) const = 0;

    bool operator<(const ConditionImpl& other) const;
    bool operator>(const ConditionImpl& other) const;

    /// @brief Accepts the visitor by calling the visit overload.
    virtual void accept(ConditionVisitor& visitor) const = 0;
};


/* Literal */
class ConditionLiteralImpl : public ConditionImpl, std::enable_shared_from_this<ConditionLiteralImpl> {
private:
    Literal m_literal;

    ConditionLiteralImpl(int identifier, const Literal& literal);

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    ~ConditionLiteralImpl() override;

    bool operator==(const ConditionImpl& other) const override;
    bool operator!=(const ConditionImpl& other) const override;

    size_t hash() const;

    void accept(ConditionVisitor& visitor) const override;

    const Literal& get_literal() const;
};


/* And */
class ConditionAndImpl : public ConditionImpl, std::enable_shared_from_this<ConditionAndImpl> {
private:
    ConditionList m_conditions;

    ConditionAndImpl(int identifier, const ConditionList& conditions);

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    ~ConditionAndImpl() override;

    bool operator==(const ConditionImpl& other) const override;
    bool operator!=(const ConditionImpl& other) const override;

    size_t hash() const;

    void accept(ConditionVisitor& visitor) const override;

    const ConditionList& get_conditions() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Condition>
    {
        bool operator()(const loki::pddl::Condition& left_condition, const loki::pddl::Condition& right_condition) const;
    };

    template<>
    struct hash<loki::pddl::ConditionLiteralImpl>
    {
        std::size_t operator()(const loki::pddl::ConditionLiteralImpl& condition) const;
    };

    template<>
    struct hash<loki::pddl::ConditionAndImpl>
    {
        std::size_t operator()(const loki::pddl::ConditionAndImpl& condition) const;
    };
}


#endif
