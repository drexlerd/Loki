#include "include/loki/domain/printer.hpp"

#include <sstream>
#include <vector>

using namespace std;

namespace loki::domain
{

    // Printer for std::vector
    template <typename T>
    inline std::string parse_text(const std::vector<T>& nodes, const FormattingOptions& options);

    // Printer for boost::variant
    class NodeVisitorPrinter : public boost::static_visitor<std::string> {
    private:
        const FormattingOptions* options;

    public:
        NodeVisitorPrinter(const FormattingOptions& options) : options(&options) { }

        template <typename Node>
        std::string operator()(const Node& node) const
        {
            return parse_text(node, *options);
        }
    };

    string parse_text(const ast::Name& node, const FormattingOptions&)
    {
        stringstream ss;
        ss << node.alpha << node.suffix;
        return ss.str();
    }

    string parse_text(const ast::Variable& node, const FormattingOptions&)
    {
        stringstream ss;
        ss << node.question_mark << parse_text(node.name);
        return ss.str();
    }

    string parse_text(const ast::Number& node, const FormattingOptions&)
    {
        stringstream ss;
        ss << node.value;
        return ss.str();
    }

    string parse_text(const ast::Term& node, const FormattingOptions& options)
    {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::Undefined& node, const FormattingOptions& options) { return "undefined"; }

    std::string parse_text(const ast::RequirementStrips&, const FormattingOptions&) { return ":strips"; }
    std::string parse_text(const ast::RequirementTyping&, const FormattingOptions&) { return ":typing"; }
    std::string parse_text(const ast::RequirementNegativePreconditions&, const FormattingOptions&) { return ":negative-preconditions"; }
    std::string parse_text(const ast::RequirementDisjunctivePreconditions&, const FormattingOptions&) { return ":disjunctive-preconditions"; }
    std::string parse_text(const ast::RequirementEquality&, const FormattingOptions&) { return ":equality"; }
    std::string parse_text(const ast::RequirementExistentialPreconditions&, const FormattingOptions&) { return ":existential-preconditions"; }
    std::string parse_text(const ast::RequirementUniversalPreconditions&, const FormattingOptions&) { return ":universal-preconditions"; }
    std::string parse_text(const ast::RequirementQuantifiedPreconditions&, const FormattingOptions&) { return ":quantified-preconditions"; }
    std::string parse_text(const ast::RequirementConditionalEffects&, const FormattingOptions&) { return ":conditional-effects"; }
    std::string parse_text(const ast::RequirementFluents&, const FormattingOptions&) { return ":fluents"; }
    std::string parse_text(const ast::RequirementObjectFluents&, const FormattingOptions&) { return ":object-fluents"; }
    std::string parse_text(const ast::RequirementNumericFluents&, const FormattingOptions&) { return ":numeric-fluents"; }
    std::string parse_text(const ast::RequirementAdl&, const FormattingOptions&) { return ":adl"; }
    std::string parse_text(const ast::RequirementDurativeActions&, const FormattingOptions&) { return ":durative-actions"; }
    std::string parse_text(const ast::RequirementDerivedPredicates&, const FormattingOptions&) { return ":derived-predicates"; }
    std::string parse_text(const ast::RequirementTimedInitialLiterals&, const FormattingOptions&) { return ":timed-initial-literals"; }
    std::string parse_text(const ast::RequirementPreferences&, const FormattingOptions&) { return ":preferences"; }
    std::string parse_text(const ast::RequirementConstraints&, const FormattingOptions&) { return ":constraints"; }

    std::string parse_text(const ast::Requirement& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }


    string parse_text(const ast::Type& node, const FormattingOptions& options)
    {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::TypeObject&, const FormattingOptions&)
    {
        return "object";
    }

    string parse_text(const ast::TypeEither& node, const FormattingOptions&)
    {
        stringstream ss;
        ss << "(either ";
        for (size_t i = 0; i < node.types.size(); ++i)
        {
            if (i != 0)
                ss << " ";
            ss << parse_text(node.types[i]);
        }
        ss << ")";
        return ss.str();
    }

    string parse_text(const ast::TypedListOfNamesRecursively& node, const FormattingOptions& options)
    {
        stringstream ss;
        for (size_t i = 0; i < node.names.size(); ++i)
        {
            if (i != 0)
                ss << " ";
            ss << parse_text(node.names[i]);
        }
        ss << " - " << parse_text(node.type);
        // lookahead
        auto nested_options = FormattingOptions{options.indent + options.add_indent, options.add_indent, options.newline};
        auto nested_text = parse_text(node.typed_list_of_names, nested_options);
        if (nested_text.size() > 0) {
            ss << (options.newline) ? "\n" : " ";
            ss << string(nested_options.indent, ' ')
               << nested_text;
        }
        return ss.str();
    }

    string parse_text(const ast::TypedListOfNames& node, const FormattingOptions& options)
    {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::TypedListOfVariablesRecursively& node, const FormattingOptions& options)
    {
        stringstream ss;
        for (size_t i = 0; i < node.variables.size(); ++i)
        {
            if (i != 0)
                ss << " ";
            ss << parse_text(node.variables[i]);
        }
        ss << " - " << parse_text(node.type);
        auto nested_options = FormattingOptions{options.indent + options.add_indent, options.add_indent, options.newline};
        auto nested_text = parse_text(node.typed_list_of_variables, options);
        if (nested_text.size() > 0) {
            ss << (options.newline) ? "\n" : " ";
            ss << string(nested_options.indent, ' ')
               << nested_text;
        }
        return ss.str();
    }

    std::string parse_text(const ast::TypedListOfVariables& node, const FormattingOptions& options)
    {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::Predicate& node, const FormattingOptions& options)
    {
        return parse_text(node.name, options);
    }

    std::string parse_text(const ast::AtomicFormulaSkeleton& node, const FormattingOptions& options)
    {
        std::stringstream ss;
        ss << "("
           << parse_text(node.predicate, options) << " "
           << parse_text(node.typed_list_of_variables, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::FunctionSymbol& node, const FormattingOptions& options)
    {
        return parse_text(node.name, options);
    }

    std::string parse_text(const ast::FunctionTerm& node, const FormattingOptions& options)
    {
        std::stringstream ss;
        ss << "("
           << parse_text(node.function_symbol, options) << " "
           << parse_text(node.terms, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::FunctionTypeNumber& node, const FormattingOptions& options)
    {
        return parse_text(node.number, options);
    }

    std::string parse_text(const ast::FunctionTypeType& node, const FormattingOptions& options)
    {
        return parse_text(node.type, options);
    }

    std::string parse_text(const ast::FunctionType& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::AtomicFunctionSkeleton& node, const FormattingOptions& options)
    {
        std::stringstream ss;
        ss << "("
           << parse_text(node.function_symbol, options) << " "
           << parse_text(node.arguments, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& node, const FormattingOptions& options) {
        stringstream ss;
        for (size_t i = 0; i < node.atomic_function_skeletons.size(); ++i)
        {
            if (i != 0)
                ss << " ";
            ss << parse_text(node.atomic_function_skeletons[i]);
        }
        ss << " - " << parse_text(node.function_type);
        auto nested_options = FormattingOptions{options.indent + options.add_indent, options.add_indent, options.newline};
        if (node.function_typed_list_of_atomic_function_skeletons.has_value()) {
            auto nested_text = parse_text(node.function_typed_list_of_atomic_function_skeletons.value(), options);
            if (nested_text.size() > 0) {
                ss << (options.newline) ? "\n" : " ";
                ss << string(nested_options.indent, ' ')
                << nested_text;
            }
        }
        return ss.str();
    }

    std::string parse_text(const ast::FunctionTypedListOfAtomicFunctionSkeletons& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }


    std::string parse_text(const ast::AtomicFormulaOfTermsPredicate& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(" << parse_text(node.predicate, options) << " " << parse_text(node.terms, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::AtomicFormulaOfTermsEquality& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(" << "= " << parse_text(node.term_left, options) << " " << parse_text(node.term_right, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::AtomicFormulaOfTerms& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::Atom& node, const FormattingOptions& options) {
        return parse_text(node.atomic_formula_of_terms, options);
    }

    std::string parse_text(const ast::NegatedAtom& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(not " << parse_text(node.atomic_formula_of_terms, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::Literal& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::MultiOperatorMul&, const FormattingOptions&) { return "*"; }
    std::string parse_text(const ast::MultiOperatorPlus&, const FormattingOptions&) { return "+"; }
    std::string parse_text(const ast::MultiOperator& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::BinaryOperatorMinus&, const FormattingOptions&) { return "-"; }
    std::string parse_text(const ast::BinaryOperatorDiv&, const FormattingOptions&) { return "/"; }
    std::string parse_text(const ast::BinaryOperator& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }


    std::string parse_text(const ast::BinaryComparatorGreater&, const FormattingOptions&) { return ">"; }
    std::string parse_text(const ast::BinaryComparatorLess&, const FormattingOptions&) { return "<"; }
    std::string parse_text(const ast::BinaryComparatorEqual&, const FormattingOptions&) { return "="; }
    std::string parse_text(const ast::BinaryComparatorGreaterEqual&, const FormattingOptions&) { return ">="; }
    std::string parse_text(const ast::BinaryComparatorLessEqual&, const FormattingOptions&) { return "<="; }
    std::string parse_text(const ast::BinaryComparator& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }


    std::string parse_text(const ast::FunctionHead& node, const FormattingOptions& options) {
        std::stringstream ss;
        if (node.terms.size() > 0) {
            ss << "(" << parse_text(node.function_symbol, options) << " " << parse_text(node.terms, options) << ")";
        } else {
            ss << parse_text(node.function_symbol, options);
        }
        return ss.str();
    }

    std::string parse_text(const ast::FunctionExpression& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::FunctionExpressionNumber& node, const FormattingOptions& options) {
        return parse_text(node.number, options);
    }

    std::string parse_text(const ast::FunctionExpressionBinaryOp& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(" << parse_text(node.binary_operator, options) << " "
                  << parse_text(node.function_expression_left, options) << " "
                  << parse_text(node.function_expression_right, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::FunctionExpressionMinus& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(- " << parse_text(node.function_expression, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::FunctionExpressionHead& node, const FormattingOptions& options) {
        return parse_text(node.function_head, options);
    }


    std::string parse_text(const ast::GoalDescriptor& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::GoalDescriptorAtom& node, const FormattingOptions& options) {
        return parse_text(node.atom, options);
    }

    std::string parse_text(const ast::GoalDescriptorLiteral& node, const FormattingOptions& options) {
        return parse_text(node.literal, options);
    }

    std::string parse_text(const ast::GoalDescriptorAnd& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(and " << parse_text(node.goal_descriptors, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::GoalDescriptorOr& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(or " << parse_text(node.goal_descriptors, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::GoalDescriptorNot& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(not " << parse_text(node.goal_descriptor, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::GoalDescriptorImply& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(imply " << parse_text(node.goal_descriptor_left, options) << " "
                        << parse_text(node.goal_descriptor_right, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::GoalDescriptorExists& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(exists " << parse_text(node.typed_list_of_variables, options) << " "
                        << parse_text(node.goal_descriptor, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::GoalDescriptorForall& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(forall " << parse_text(node.typed_list_of_variables, options) << " "
                        << parse_text(node.goal_descriptor, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::GoalDescriptorFunctionComparison& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(" << parse_text(node.binary_comparator, options) << " "
                  << parse_text(node.function_expression_left, options) << " "
                  << parse_text(node.function_expression_right, options) << ")";
        return ss.str();
    }


    std::string parse_text(const ast::ConstraintGoalDescriptor& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::ConstraintGoalDescriptorAnd& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(and " << parse_text(node.constraint_goal_descriptors, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::ConstraintGoalDescriptorForall& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(forall " << parse_text(node.typed_list_of_variables, options) << " "
                        << parse_text(node.constraint_goal_descriptor, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::ConstraintGoalDescriptorAtEnd& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(at end " << parse_text(node.goal_descriptor, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::ConstraintGoalDescriptorAlways& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(always " << parse_text(node.goal_descriptor, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::ConstraintGoalDescriptorSometime& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(sometime " << parse_text(node.goal_descriptor, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::ConstraintGoalDescriptorWithin& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(within " << parse_text(node.number, options) << " "
                         << parse_text(node.goal_descriptor, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::ConstraintGoalDescriptorAtMostOnce& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(at-most-once " << parse_text(node.goal_descriptor, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::ConstraintGoalDescriptorSometimeAfter& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(sometime-after " << parse_text(node.goal_descriptor_left, options) << " "
                                 << parse_text(node.goal_descriptor_right, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::ConstraintGoalDescriptorSometimeBefore& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(sometime-before " << parse_text(node.goal_descriptor_left, options) << " "
                                  << parse_text(node.goal_descriptor_right, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::ConstraintGoalDescriptorAlwaysWithin& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(always-within " << parse_text(node.number, options) << " "
                                << parse_text(node.goal_descriptor_left, options) << " "
                                << parse_text(node.goal_descriptor_right, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::ConstraintGoalDescriptorHoldDuring& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(hold-during " << parse_text(node.number_left, options) << " "
                              << parse_text(node.number_right, options) << " "
                              << parse_text(node.goal_descriptor, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::ConstraintGoalDescriptorHoldAfter& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(hold-after " << parse_text(node.number, options) << " "
                            << parse_text(node.goal_descriptor, options) << ")";
        return ss.str();
    }


    std::string parse_text(const ast::PreferenceName& node, const FormattingOptions& options) {
        return parse_text(node.name, options);
    }

    std::string parse_text(const ast::PreconditionGoalDescriptor& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::PreconditionGoalDescriptorSimple& node, const FormattingOptions& options) {
        return parse_text(node.goal_descriptor, options);
    }

    std::string parse_text(const ast::PreconditionGoalDescriptorAnd& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(and " << parse_text(node.precondition_goal_descriptors, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::PreconditionGoalDescriptorPreference& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(preference " << parse_text(node.preference_name, options) << " "
                             << parse_text(node.goal_descriptor, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::PreconditionGoalDescriptorForall& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(forall " << parse_text(node.typed_list_of_variables, options) << " "
                         << parse_text(node.precondition_goal_descriptor, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::AssignOperatorAssign&, const FormattingOptions&) { return "assign"; }
    std::string parse_text(const ast::AssignOperatorScaleUp&, const FormattingOptions&) { return "scale-up"; }
    std::string parse_text(const ast::AssignOperatorScaleDown&, const FormattingOptions&) { return "scale-down"; }
    std::string parse_text(const ast::AssignOperatorIncrease&, const FormattingOptions&) { return "increase"; }
    std::string parse_text(const ast::AssignOperatorDecrease&, const FormattingOptions&) { return "decrease"; }
    std::string parse_text(const ast::AssignOperator& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }


    std::string parse_text(const ast::Effect& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::EffectProductionLiteral& node, const FormattingOptions& options) {
        return parse_text(node.literal, options);
    }

    std::string parse_text(const ast::EffectProductionNumericFluent& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(" << parse_text(node.assign_operator, options) << " "
                  << parse_text(node.function_head, options) << " "
                  << parse_text(node.function_expression, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::EffectProductionObjectFluent& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(assign " << parse_text(node.function_term, options) << " "
                         << boost::apply_visitor(NodeVisitorPrinter(options), node.term) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::EffectProduction& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::EffectConditionalForall& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(forall " << parse_text(node.typed_list_of_variables, options) << " "
                         << parse_text(node.effect, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::EffectConditionalWhen& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(when " << parse_text(node.goal_descriptor, options) << " "
                       << parse_text(node.effect, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::EffectConditional& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }


    std::string parse_text(const ast::ActionSymbol& node, const FormattingOptions& options) {
        return parse_text(node.name, options);
    }

    std::string parse_text(const ast::ActionBody& node, const FormattingOptions& options) {
        std::stringstream ss;
        if (node.precondition_goal_descriptor.has_value()) {
            ss << std::string(options.indent, ' ') << ":precondition " << parse_text(node.precondition_goal_descriptor.value(), options);
            ss << ((options.newline) ? "\n" : " ");
        }
        if (node.effect.has_value()) {
            ss << std::string(options.indent, ' ') << ":effect " << parse_text(node.effect.value(), options);
        }
        return ss.str();
    }


    std::string parse_text(const ast::Action& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << std::string(options.indent, ' ') << "(action " << parse_text(node.action_symbol, options);
        FormattingOptions nested_options{options.indent + options.add_indent, options.add_indent, options.newline};
        ss << ((nested_options.newline) ? "\n" : " ");
        ss << std::string(nested_options.indent, ' ') << ":parameters (" << parse_text(node.typed_list_of_variables, nested_options) << ")";
        ss << ((nested_options.newline) ? "\n" : " ");
        ss << parse_text(node.action_body, nested_options);
        return ss.str();
    }

    std::string parse_text(const ast::DerivedPredicate& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << std::string(options.indent, ' ') << "(:derived "
            << parse_text(node.typed_list_of_variables, options) << " "
            << parse_text(node.goal_descriptor, options) << ")";
        return ss.str();
    }



    std::string parse_text(const ast::DomainName& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(domain " << parse_text(node.name, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::Requirements& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << string(options.indent, ' ') << "(:requirements ";
        for (size_t i = 0; i < node.requirements.size(); ++i) {
            if (i != 0) ss << " ";
            ss << parse_text(node.requirements[i], options);
        }
        ss << ")";
        return ss.str();
    }

    std::string parse_text(const ast::Types& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << string(options.indent, ' ') << "(:types " << parse_text(node.typed_list_of_names, options);
        ss << ")";
        return ss.str();
    }

    std::string parse_text(const ast::Constants& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << string(options.indent, ' ') << "(:constants " << parse_text(node.typed_list_of_names, options);
        ss << ")";
        return ss.str();
    }

    std::string parse_text(const ast::Predicates& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << string(options.indent, ' ') << "(:predicates " << parse_text(node.atomic_formula_skeletons, options);
        ss << ")";
        return ss.str();
    }

    std::string parse_text(const ast::Functions& node, const FormattingOptions& options) {

    }

    std::string parse_text(const ast::Constraints& node, const FormattingOptions& options) {

    }

    std::string parse_text(const ast::Structure& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }


    std::string parse_text(const ast::Domain& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(define " << parse_text(node.domain_name, options) << "\n";
        auto nested_options = FormattingOptions{options.indent + options.add_indent, options.add_indent, options.newline};
        if (node.requirements.has_value()) {
            ss << parse_text(node.requirements.value(), nested_options) << "\n";
        }
        if (node.types.has_value()) {
            ss << parse_text(node.types.value(), nested_options) << "\n";
        }
        if (node.constants.has_value()) {
            ss << parse_text(node.constants.value(), nested_options) << "\n";
        }
        if (node.predicates.has_value()) {
            ss << parse_text(node.predicates.value(), nested_options) << "\n";
        }
        // TODO: functions
        // TODO: constraints
        for (size_t i = 0; i < node.structures.size(); ++i) {
            if (i != 0) {
                ss << ((options.newline) ? "\n" : " ");
            }
            ss << parse_text(node.structures[i], nested_options);
        }
        ss << ")";
        return ss.str();
    }


    template <typename T>
    inline std::string parse_text(const std::vector<T>& nodes, const FormattingOptions& options)
    {
        std::stringstream ss;
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            if (i != 0)
                ss << " ";
            ss << parse_text(nodes[i], options);
        }
        return ss.str();
    }

}