#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_EFFECTS_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_EFFECTS_HPP_

#include "declarations.hpp"

#include <string>


namespace loki {
template<typename T>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
/// @brief Provides an interface for visiting nodes in a DAG of conditions.
class EffectVisitor {
public:
    virtual void visit(const EffectLiteral& condition) = 0;
};


/* BaseEffect */
class EffectImpl {
protected:
    int m_identifier;

    EffectImpl(int identifier);

public:
    virtual ~EffectImpl();

    // We never need to compare base types
    bool operator==(const EffectImpl& other) const = delete;
    bool operator!=(const EffectImpl& other) const = delete;

    virtual bool operator<(const EffectImpl& other) const;
    virtual bool operator>(const EffectImpl& other) const;

    virtual size_t hash() const = 0;

    /// @brief Accepts the visitor by calling the visit overload.
    virtual void accept(EffectVisitor& visitor) const = 0;
};


/* Literal */
class EffectLiteralImpl : public EffectImpl, std::enable_shared_from_this<EffectLiteralImpl> {
private:
    Literal m_literal;

    EffectLiteralImpl(int identifier, const Literal& literal);

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    ~EffectLiteralImpl() override;

    bool operator==(const EffectLiteralImpl& other) const;
    bool operator!=(const EffectLiteralImpl& other) const;

    size_t hash() const override;

    void accept(EffectVisitor& visitor) const override;

    const Literal& get_literal() const;
};

}


namespace std {
    template<>
    struct hash<loki::pddl::EffectLiteralImpl>
    {
        std::size_t operator()(const loki::pddl::EffectLiteralImpl& effect) const;
    };
}


#endif
