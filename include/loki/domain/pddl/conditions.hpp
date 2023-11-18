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
    virtual void visit(const ConditionAnd& condition) = 0;
};


/* BaseCondition */
class ConditionImpl {
protected:
    int m_identifier;

    ConditionImpl(int identifier);

public:
    virtual ~ConditionImpl();

    // We never need to compare base types
    bool operator==(const ConditionImpl& other) const = delete;
    bool operator!=(const ConditionImpl& other) const = delete;

    bool operator<(const ConditionImpl& other) const;
    bool operator>(const ConditionImpl& other) const;

    virtual size_t hash() const = 0;

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

    size_t hash() const override;

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

    size_t hash() const override;

    void accept(ConditionVisitor& visitor) const override;

    const ConditionList& get_conditions() const;
};

}


namespace std {
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
