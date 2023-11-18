#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_CONDITIONS_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_CONDITIONS_HPP_

#include "../../common/pddl/base.hpp"
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
    virtual void visit(const ConditionAnd& condition) = 0;
};


/* BaseCondition */
class ConditionImpl : public Base {
protected:
    ConditionImpl(int identifier);

public:
    virtual ~ConditionImpl();

    // We never need to compare base types
    bool operator==(const ConditionImpl& other) const = delete;
    bool operator!=(const ConditionImpl& other) const = delete;

    /// @brief Accepts the visitor by calling the visit overload.
    virtual void accept(ConditionVisitor& visitor) const = 0;
};


/* Literal */
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

    size_t hash() const;

    void accept(ConditionVisitor& visitor) const override;

    const Literal& get_literal() const;
};


/* And */
class ConditionAndImpl : public ConditionImpl, std::enable_shared_from_this<ConditionAndImpl> {
private:
    ConditionList m_conditions;

    ConditionAndImpl(int identifier, const ConditionList& conditions);

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    ~ConditionAndImpl() override;

    bool operator==(const ConditionAndImpl& other) const;
    bool operator!=(const ConditionAndImpl& other) const;

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
