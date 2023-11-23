#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_ARGUMENT_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_ARGUMENT_HPP_


#include "declarations.hpp"

#include "../../common/pddl/base.hpp"


namespace loki {

class ArgumentVisitor {

};

class Argument : public Base<Argument> {
protected:
public:
};


class ArgumentConstant : public Argument {
private:
    pddl::Object constant;
};


class ArgumentVariable : public Argument {
    // It is a bit weird that variables are previously parsed as Parameters,
    // now we call them variables, variable is a bit more general.
    pddl::Parameter variable;
};


// :object-fluents
//class ArgumentFunctionTerm : public Argument {
//};

}

#endif
