#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PRINTER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PRINTER_HPP_

#include "ast.hpp"
#include "include/loki/common/printer.hpp"

#include <sstream>

namespace loki::domain {

// create string representations from ast nodes.
extern std::string parse_text(const ast::Name& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Variable& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Number& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Term& node, const FormattingOptions& options={});

extern std::string parse_text(const ast::RequirementStrips& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::RequirementTyping& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::RequirementNegativePreconditions& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::RequirementDisjunctivePreconditions& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::RequirementEquality& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::RequirementExistentialPreconditions& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::RequirementUniversalPreconditions& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::RequirementQuantifiedPreconditions& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::RequirementConditionalEffects& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::RequirementFluents& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::RequirementObjectFluents& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::RequirementNumericFluents& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::RequirementAdl& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::RequirementDurativeActions& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::RequirementDerivedPredicates& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::RequirementTimedInitialLiterals& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::RequirementPreferences& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::RequirementConstraints& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Requirement& node, const FormattingOptions& options={});


extern std::string parse_text(const ast::Type& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::TypeObject& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::TypeEither& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::TypedListOfNamesRecursively& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::TypedListOfNames& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::TypedListOfVariablesRecursively& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::TypedListOfVariables& node, const FormattingOptions& options={});

extern std::string parse_text(const ast::Predicate& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::AtomicFormulaSkeleton& node, const FormattingOptions& options={});

extern std::string parse_text(const ast::FunctionSymbol& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::FunctionTerm& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::FunctionTypeNumber& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::FunctionTypeType& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::FunctionType& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::AtomicFunctionSkeleton& node, const FormattingOptions& options={});

extern std::string parse_text(const ast::DomainName& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Requirements& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Types& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Constants& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Predicates& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Functions& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Constraints& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Structure& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Domain& node, const FormattingOptions& options={});

}

#endif