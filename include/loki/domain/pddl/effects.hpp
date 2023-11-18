#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_EFFECTS_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_EFFECTS_HPP_

#include "../../common/pddl/base.hpp"
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
class EffectImpl : public Base {
protected:
    EffectImpl(int identifier);

public:
    virtual ~EffectImpl();

    // We never need to compare base types
    bool operator==(const EffectImpl& other) const = delete;
    bool operator!=(const EffectImpl& other) const = delete;

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

    size_t hash() const;

    void accept(EffectVisitor& visitor) const override;

    const Literal& get_literal() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Effect>
    {
        bool operator()(const loki::pddl::Effect& left_effect, const loki::pddl::Effect& right_effect) const;
    };

    template<>
    struct hash<loki::pddl::EffectLiteralImpl>
    {
        std::size_t operator()(const loki::pddl::EffectLiteralImpl& effect) const;
    };
}


#endif
