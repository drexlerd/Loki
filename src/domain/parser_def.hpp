#ifndef LOKI_SRC_DOMAIN_PARSER_DEF_HPP_
#define LOKI_SRC_DOMAIN_PARSER_DEF_HPP_

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#include "include/loki/domain/ast.hpp"
#include "include/loki/domain/error_handler.hpp"
#include "include/loki/domain/parser.hpp"

#include "ast_adapted.hpp"
#include "parser.hpp"


namespace loki::domain::parser {
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    using x3::lit;
    using x3::lexeme;
    using x3::eps;
    using x3::int_;

    using ascii::alpha;
    using ascii::alnum;
    using ascii::char_;
    using ascii::string;

    ///////////////////////////////////////////////////////////////////////////
    // Rule IDs
    ///////////////////////////////////////////////////////////////////////////

    struct NameClass;
    struct VariableClass;
    struct NumberClass;
    struct TermClass;

    struct StripsRequirementClass;
    struct TypingRequirementClass;
    struct RequirementClass;

    struct FluentTypeClass;
    struct EitherTypeClass;
    struct TypeClass;
    struct TypedListOfNamesRecursivelyClass;
    struct TypedListOfNamesClass;
    struct TypedListOfVariablesRecursivelyClass;
    struct TypedListOfVariablesClass;

    struct PredicateClass;
    struct AtomicFormulaSkeletonClass;

    struct FunctionSymbolClass;
    struct FunctionTypeClass;
    struct AtomicFunctionSkeletonClass;
    struct FunctionTypedListOfAtomicFunctionSkeletonsRecursivelyClass;
    struct FunctionTypedListOfAtomicFunctionSkeletonsClass;

    struct AtomClass;
    struct NegatedAtomClass;
    struct LiteralClass;
    struct AtomicFormulaOfTermsClass;

    struct GoalDescriptorAtomicFormulaClass;
    struct GoalDescriptorLiteralClass;
    struct GoalDescriptorAndClass;
    struct GoalDescriptorOrClass;
    struct GoalDescriptorNotClass;
    struct GoalDescriptorImplyClass;
    struct GoalDescriptorExistsClass;
    struct GoalDescriptorForallClass;
    struct GoalDescriptorClass;

    struct ConstraintGoalDescriptorAndClass;
    struct ConstraintGoalDescriptorForallClass;
    struct ConstraintGoalDescriptorAtEndClass;
    struct ConstraintGoalDescriptorAlwaysClass;
    struct ConstraintGoalDescriptorSometimeClass;
    struct ConstraintGoalDescriptorWithinClass;
    struct ConstraintGoalDescriptorAtMostOnceClass;
    struct ConstraintGoalDescriptorSometimeAfterClass;
    struct ConstraintGoalDescriptorSometimeBeforeClass;
    struct ConstraintGoalDescriptorAlwaysWithinClass;
    struct ConstraintGoalDescriptorHoldDuringClass;
    struct ConstraintGoalDescriptorHoldAfterClass;
    struct ConstraintGoalDescriptorClass;

    struct DomainNameClass;
    struct RequirementsClass;
    struct TypesClass;
    struct ConstantsClass;
    struct PredicatesClass;
    struct FunctionsClass;
    struct ConstraintsClass;


    ///////////////////////////////////////////////////////////////////////////
    // Rules
    ///////////////////////////////////////////////////////////////////////////

    x3::rule<NameClass, ast::Name> const
        name = "name";
    x3::rule<VariableClass, ast::Variable> const
        variable = "variable";
    x3::rule<NumberClass, ast::Number> const
        number = "number";
    x3::rule<TermClass, ast::Term> const
        term = "term";

    x3::rule<StripsRequirementClass, ast::StripsRequirement> const
        strips_requirement = "strips_requirement";
    x3::rule<TypingRequirementClass, ast::TypingRequirement> const
        typing_requirement = "typing_requirement";
    x3::rule<RequirementClass, ast::Requirement> const
        requirement = "requirement";

    x3::rule<FluentTypeClass, ast::FluentType> const
        fluent_type = "fluent_type";
    x3::rule<EitherTypeClass, ast::EitherType> const
        either_type = "either_type";
    x3::rule<TypeClass, ast::Type> const
        type = "type";
    x3::rule<TypedListOfNamesRecursivelyClass, ast::TypedListOfNamesRecursively> const
        typed_list_of_names_recursively = "typed_list_of_names_recursively";
    x3::rule<TypedListOfNamesClass, ast::TypedListOfNames> const
        typed_list_of_names = "typed_list_of_names";

    x3::rule<TypedListOfVariablesRecursivelyClass, ast::TypedListOfVariablesRecursively> const
        typed_list_of_variables_recursively = "typed_list_of_variables_recursively";
    x3::rule<TypedListOfVariablesClass, ast::TypedListOfVariables> const
        typed_list_of_variables = "typed_list_of_variables";

    x3::rule<PredicateClass, ast::Predicate> const
        predicate = "predicate";
    x3::rule<AtomicFormulaSkeletonClass, ast::AtomicFormulaSkeleton> const
        atomic_formula_skeleton = "atomic_formula_skeleton";

    x3::rule<FunctionSymbolClass, ast::FunctionSymbol> const
        function_symbol = "function_symbol";
    x3::rule<FunctionTypeClass, ast::FunctionType> const
        function_type = "function_type";
    x3::rule<AtomicFunctionSkeletonClass, ast::AtomicFunctionSkeleton> const
        atomic_function_skeleton = "atomic_function_skeleton";
    x3::rule<FunctionTypedListOfAtomicFunctionSkeletonsRecursivelyClass, ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively> const
        function_typed_list_of_atomic_function_skeletons_recursively = "function_typed_list_of_atomic_function_skeletons_recursively";
    x3::rule<FunctionTypedListOfAtomicFunctionSkeletonsClass, ast::FunctionTypedListOfAtomicFunctionSkeletons> const
        function_typed_list_of_atomic_function_skeletons = "function_typed_list_of_atomic_function_skeletons";

    x3::rule<AtomicFormulaOfTermsClass, ast::AtomicFormulaOfTerms> const
        atomic_formula_of_terms = "atomic_formula_of_terms";
    x3::rule<AtomClass, ast::Atom> const
        atom = "atom";
    x3::rule<NegatedAtomClass, ast::NegatedAtom> const
        negated_atom = "negated_atom";
    x3::rule<LiteralClass, ast::Literal> const
        literal = "literal";

    x3::rule<GoalDescriptorAtomicFormulaClass, ast::GoalDescriptorAtomicFormula> const
        goal_descriptor_atomic_formula = "goal_descriptor_atomic_formula";
    x3::rule<GoalDescriptorLiteralClass, ast::GoalDescriptorLiteral> const
        goal_descriptor_literal = "goal_descriptor_literal";
    x3::rule<GoalDescriptorAndClass, ast::GoalDescriptorAnd> const
        goal_descriptor_and = "goal_descriptor_and";
    x3::rule<GoalDescriptorOrClass, ast::GoalDescriptorOr> const
        goal_descriptor_or = "goal_descriptor_or";
    x3::rule<GoalDescriptorNotClass, ast::GoalDescriptorNot> const
        goal_descriptor_not = "goal_descriptor_not";
    x3::rule<GoalDescriptorImplyClass, ast::GoalDescriptorImply> const
        goal_descriptor_imply = "goal_descriptor_imply";
    x3::rule<GoalDescriptorExistsClass, ast::GoalDescriptorExists> const
        goal_descriptor_exists = "goal_descriptor_exists";
    x3::rule<GoalDescriptorForallClass, ast::GoalDescriptorForall> const
        goal_descriptor_forall = "goal_descriptor_forall";
    x3::rule<GoalDescriptorClass, ast::GoalDescriptor> const
        goal_descriptor = "goal_descriptor";

    x3::rule<ConstraintGoalDescriptorAndClass, ast::ConstraintGoalDescriptorAnd> const
        constraint_goal_descriptor_and = "constraint_goal_descriptor_and";
    x3::rule<ConstraintGoalDescriptorForallClass, ast::ConstraintGoalDescriptorForall> const
        constraint_goal_descriptor_forall = "constraint_goal_descriptor_forall";
    x3::rule<ConstraintGoalDescriptorAtEndClass, ast::ConstraintGoalDescriptorAtEnd> const
        constraint_goal_descriptor_at_end = "constraint_goal_descriptor_at_end";
    x3::rule<ConstraintGoalDescriptorAlwaysClass, ast::ConstraintGoalDescriptorAlways> const
        constraint_goal_descriptor_always = "constraint_goal_descriptor_always";
    x3::rule<ConstraintGoalDescriptorSometimeClass, ast::ConstraintGoalDescriptorSometime> const
        constraint_goal_descriptor_sometime = "constraint_goal_descriptor_sometime";
    x3::rule<ConstraintGoalDescriptorWithinClass, ast::ConstraintGoalDescriptorWithin> const
        constraint_goal_descriptor_within = "constraint_goal_descriptor_within";
    x3::rule<ConstraintGoalDescriptorAtMostOnceClass, ast::ConstraintGoalDescriptorAtMostOnce> const
        constraint_goal_descriptor_at_most_once = "constraint_goal_descriptor_at_most_once";
    x3::rule<ConstraintGoalDescriptorSometimeAfterClass, ast::ConstraintGoalDescriptorSometimeAfter> const
        constraint_goal_descriptor_sometime_after = "constraint_goal_descriptor_sometime_after";
    x3::rule<ConstraintGoalDescriptorSometimeBeforeClass, ast::ConstraintGoalDescriptorSometimeBefore> const
        constraint_goal_descriptor_sometime_before = "constraint_goal_descriptor_sometime_before";
    x3::rule<ConstraintGoalDescriptorAlwaysWithinClass, ast::ConstraintGoalDescriptorAlwaysWithin> const
        constraint_goal_descriptor_always_within = "constraint_goal_descriptor_always_within";
    x3::rule<ConstraintGoalDescriptorHoldDuringClass, ast::ConstraintGoalDescriptorHoldDuring> const
        constraint_goal_descriptor_hold_during = "constraint_goal_descriptor_hold_during";
    x3::rule<ConstraintGoalDescriptorHoldAfterClass, ast::ConstraintGoalDescriptorHoldAfter> const
        constraint_goal_descriptor_hold_after = "constraint_goal_descriptor_hold_after";
    x3::rule<ConstraintGoalDescriptorClass, ast::ConstraintGoalDescriptor> const
        constraint_goal_descriptor = "constraint_goal_descriptor";

    x3::rule<DomainNameClass, ast::DomainName> const
        domain_name = "domain_name";
    x3::rule<RequirementsClass, ast::Requirements> const
        requirements = "requirements";
    x3::rule<TypesClass, ast::Types> const
        types = "types";
    x3::rule<ConstantsClass, ast::Constants> const
        constants = "constants";
    x3::rule<PredicatesClass, ast::Predicates> const
        predicates = "predicates";
    x3::rule<FunctionsClass, ast::Functions> const
        functions = "functions";
    x3::rule<ConstraintsClass, ast::Constraints> const
        constraints = "constraints";
    domain_description_type const domain_description = "domain_description";


    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    ///////////////////////////////////////////////////////////////////////////

    const auto name_def = alpha >> lexeme[*(alnum | char_('-') | char_('_'))];
    const auto variable_def = char_('?') > name;
    const auto number_def = int_;
    const auto term_def = name | variable;

    const auto strips_requirement_def = lit(":strips") >> x3::attr(ast::StripsRequirement{});
    const auto typing_requirement_def = lit(":typing") >> x3::attr(ast::TypingRequirement{});
    const auto requirement_def = strips_requirement | typing_requirement;

    const auto fluent_type_def = lit('(') >> lit("fluent") > type > lit(')');
    const auto either_type_def = lit('(') >> lit("either") >> +type > lit(')');
    const auto type_def = name | fluent_type | either_type;
    const auto typed_list_of_names_recursively_def = +name > lit('-') > type >> typed_list_of_names_recursively;
    const auto typed_list_of_names_def = ((*name) | typed_list_of_names_recursively);
    const auto typed_list_of_variables_recursively_def = +variable > lit('-') > type >> typed_list_of_variables_recursively;
    const auto typed_list_of_variables_def = ((*variable) | typed_list_of_variables_recursively);

    const auto predicate_def = name;
    const auto atomic_formula_skeleton_def = predicate > typed_list_of_variables;

    const auto function_symbol_def = name;
    const auto function_type_def = number;
    const auto atomic_function_skeleton_def = lit('(') > function_symbol > typed_list_of_variables > lit(')');
    const auto function_typed_list_of_atomic_function_skeletons_recursively_def = +atomic_function_skeleton > lit('-') > function_type > function_typed_list_of_atomic_function_skeletons_recursively;
    const auto function_typed_list_of_atomic_function_skeletons_def = ((*atomic_function_skeleton_def) | function_typed_list_of_atomic_function_skeletons_recursively);

    const auto atomic_formula_of_terms_def = lit('(') >> predicate >> *term > lit(')');
    const auto atom_def = atomic_formula_of_terms;
    const auto negated_atom_def = lit('(') >> lit("not") >> atomic_formula_of_terms > lit(')');
    const auto literal_def = atom | negated_atom;

    const auto goal_descriptor_atomic_formula_def = atomic_formula_of_terms;
    const auto goal_descriptor_literal_def = literal;
    const auto goal_descriptor_and_def = lit('(') >> lit("and") >> *goal_descriptor > lit(')');
    const auto goal_descriptor_or_def = lit('(') >> lit("or") >> *goal_descriptor > lit(')');
    const auto goal_descriptor_not_def = lit('(') >> lit("not") > goal_descriptor > lit(')');
    const auto goal_descriptor_imply_def = lit('(') >> lit("imply") > goal_descriptor > goal_descriptor > lit(')');
    const auto goal_descriptor_exists_def = lit('(') >> lit("exists") >> typed_list_of_variables > goal_descriptor > lit(')');
    const auto goal_descriptor_forall_def = lit('(') >> lit("forall") >> typed_list_of_variables > goal_descriptor > lit(')');
    const auto goal_descriptor_def = goal_descriptor_atomic_formula | goal_descriptor_literal | goal_descriptor_and | goal_descriptor_or
        | goal_descriptor_not | goal_descriptor_imply | goal_descriptor_exists | goal_descriptor_forall;

    const auto constraint_goal_descriptor_and_def = lit('(') >> lit("and") > *constraint_goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_forall_def = lit('(') >> lit("forall") > typed_list_of_variables > constraint_goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_at_end_def = lit('(') >> lit("at") >> lit("end") > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_always_def = lit('(') >> lit("always") > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_sometime_def = lit('(') >> lit("sometime") > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_within_def = lit('(') >> lit("within") > number > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_at_most_once_def = lit('(') >> lit("at-most-once") > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_sometime_after_def = lit('(') >> lit("sometime-after") > goal_descriptor > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_sometime_before_def = lit('(') >> lit("sometime-before") > goal_descriptor > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_always_within_def = lit('(') >> lit("always-within") > number > goal_descriptor > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_hold_during_def = lit('(') >> lit("hold-during") > number > number > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_hold_after_def = lit('(') >> lit("hold-after") > number > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_def = constraint_goal_descriptor_and | constraint_goal_descriptor_forall | constraint_goal_descriptor_at_end
        | constraint_goal_descriptor_always | constraint_goal_descriptor_sometime | constraint_goal_descriptor_within
        | constraint_goal_descriptor_at_most_once | constraint_goal_descriptor_sometime_after | constraint_goal_descriptor_sometime_before
        | constraint_goal_descriptor_always_within | constraint_goal_descriptor_hold_during | constraint_goal_descriptor_hold_after;

    const auto domain_name_def = lit('(') >> lit("domain") > name > lit(')');
    const auto requirements_def = lit('(') >> lit(":requirements") >> *requirement >> lit(')');
    const auto types_def = lit('(') >> lit(":types") >> typed_list_of_names > lit(')');
    const auto constants_def = lit('(') >> lit(":constants") >> typed_list_of_names > lit(')');
    const auto predicates_def = lit('(') >> lit(":predicates") >> *atomic_formula_skeleton > lit(')');
    const auto functions_def = lit('(') >> lit(":functions") >> *function_typed_list_of_atomic_function_skeletons > lit(')');
    const auto constraints_def = lit('(') >> lit(":constraints") > constraint_goal_descriptor > lit(')');

    const auto domain_description_def =
        lit('(') > lit("define")
           > domain_name
           >> -requirements
           >> types
           >> constants
           >> predicates
           >> -functions
           >> -constraints
        > lit(')');

    BOOST_SPIRIT_DEFINE(
        name, variable, number, term,
        strips_requirement, typing_requirement, requirement, requirements,
        fluent_type, either_type, type, typed_list_of_names_recursively, typed_list_of_names, typed_list_of_variables_recursively, typed_list_of_variables,
        predicate, atomic_formula_skeleton,
        function_symbol, function_type, atomic_function_skeleton, function_typed_list_of_atomic_function_skeletons_recursively, function_typed_list_of_atomic_function_skeletons,
        atomic_formula_of_terms, atom, negated_atom, literal,
        goal_descriptor_atomic_formula, goal_descriptor_literal, goal_descriptor_and, goal_descriptor_or,
        goal_descriptor_not, goal_descriptor_imply, goal_descriptor_exists, goal_descriptor_forall, goal_descriptor,
        constraint_goal_descriptor_and, constraint_goal_descriptor_forall, constraint_goal_descriptor_at_end,
        constraint_goal_descriptor_always, constraint_goal_descriptor_sometime, constraint_goal_descriptor_within,
        constraint_goal_descriptor_at_most_once, constraint_goal_descriptor_sometime_after,  constraint_goal_descriptor_sometime_before,
        constraint_goal_descriptor_always_within, constraint_goal_descriptor_hold_during, constraint_goal_descriptor_hold_after, constraint_goal_descriptor,
        domain_name, types, constants, predicates, functions, constraints, domain_description)


    ///////////////////////////////////////////////////////////////////////////
    // Annotation and Error handling
    ///////////////////////////////////////////////////////////////////////////

    struct NameClass : x3::annotate_on_success {};
    struct VariableClass : x3::annotate_on_success {};
    struct NumberClass : x3::annotate_on_success {};
    struct TermClass : x3::annotate_on_success {};

    struct StripsRequirementClass : x3::annotate_on_success {};
    struct TypingRequirementClass : x3::annotate_on_success {};
    struct RequirementClass : x3::annotate_on_success {};

    struct FluentTypeClass : x3::annotate_on_success {};
    struct EitherTypeClass : x3::annotate_on_success {};
    struct TypeClass : x3::annotate_on_success {};
    struct TypedListOfNamesRecursivelyClass : x3::annotate_on_success {};
    struct TypedListOfNamesClass : x3::annotate_on_success {};
    struct TypedListOfVariablesRecursivelyClass : x3::annotate_on_success {};
    struct TypedListOfVariablesClass : x3::annotate_on_success {};

    struct PredicateClass : x3::annotate_on_success {};
    struct AtomicFormulaSkeletonClass : x3::annotate_on_success {};

    struct FunctionSymbolClass : x3::annotate_on_success {};
    struct FunctionTypeClass : x3::annotate_on_success {};
    struct AtomicFunctionSkeletonClass : x3::annotate_on_success {};
    struct FunctionTypedListOfAtomicFunctionSkeletonsRecursivelyClass : x3::annotate_on_success {};
    struct FunctionTypedListOfAtomicFunctionSkeletonsClass : x3::annotate_on_success {};

    struct AtomClass : x3::annotate_on_success {};
    struct NegatedAtomClass : x3::annotate_on_success {};
    struct LiteralClass : x3::annotate_on_success {};
    struct AtomicFormulaOfTermsClass : x3::annotate_on_success {};

    struct GoalDescriptorAtomicFormulaClass : x3::annotate_on_success {};
    struct GoalDescriptorLiteralClass : x3::annotate_on_success {};
    struct GoalDescriptorAndClass : x3::annotate_on_success {};
    struct GoalDescriptorOrClass : x3::annotate_on_success {};
    struct GoalDescriptorNotClass : x3::annotate_on_success {};
    struct GoalDescriptorImplyClass : x3::annotate_on_success {};
    struct GoalDescriptorExistsClass : x3::annotate_on_success {};
    struct GoalDescriptorForallClass : x3::annotate_on_success {};
    struct GoalDescriptorClass : x3::annotate_on_success {};

    struct ConstraintGoalDescriptorAndClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorForallClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorAtEndClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorAlwaysClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorSometimeClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorWithinClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorAtMostOnceClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorSometimeAfterClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorSometimeBeforeClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorAlwaysWithinClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorHoldDuringClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorHoldAfterClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorClass : x3::annotate_on_success {};

    struct DomainNameClass : x3::annotate_on_success {};
    struct RequirementsClass : x3::annotate_on_success {};
    struct TypesClass : x3::annotate_on_success {};
    struct ConstantsClass : x3::annotate_on_success {};
    struct PredicatesClass : x3::annotate_on_success {};
    struct FunctionsClass : x3::annotate_on_success {};
    struct ConstraintsClass : x3::annotate_on_success {};
    struct DomainDescriptionClass : x3::annotate_on_success, error_handler_domain {};
}

namespace loki::domain
{
    parser::domain_description_type const& domain_description() {
        return parser::domain_description;
    }
}

#endif