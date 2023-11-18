#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_BASE_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_BASE_HPP_

#include <memory>

namespace loki::pddl {

/// @brief
class Base {
protected:
    int m_identifier;

    Base(int identifier);
    virtual ~Base() = 0;

public:
    virtual bool operator<(const Base& other) const;
    virtual bool operator>(const Base& other) const;

    virtual size_t hash() const = 0;
};

}

#endif
