import operator

from pypddl import formalism as pypddl


def test_parser_binding_parses_domain_and_task():
    parser = pypddl.Parser(
        """
(define (domain py-gripper)
  (:predicates (at ?x) (carry ?x))
  (:action move
    :parameters (?x)
    :precondition (at ?x)
    :effect (and (carry ?x)))
)
"""
    )
    domain = parser.domain()
    task = parser.parse_task(
        """
(define (problem py-gripper-task)
  (:domain py-gripper)
  (:objects ball)
  (:init (at ball))
  (:goal (carry ball))
)
"""
    )

    assert domain.get_name() == "py-gripper"
    assert domain.get_num_actions() == 1
    assert [action.get_name() for action in domain.get_actions()] == ["move"]
    assert domain.get_predicates()[0].get_parameters()[0].get_variable().get_name() == "x"
    assert task.get_name() == "py-gripper-task"
    assert task.get_domain().get_name() == domain.get_name()
    assert [obj.get_name() for obj in task.get_objects()] == ["ball"]
    assert task.get_goal() is not None


def test_high_level_bindings_expose_useful_docstrings():
    assert "Parse a PDDL domain" in pypddl.Parser.__doc__
    assert "Return the parsed domain" in pypddl.Parser.domain.__doc__
    assert "Parse a PDDL problem" in pypddl.Parser.parse_task.__doc__
    assert "Translate and normalize" in pypddl.translate_domain.__doc__
    assert "matching domain translation" in pypddl.translate_task.__doc__
    assert "reparseable PDDL" in pypddl.format_domain.__doc__
    assert "Factory for creating" in pypddl.RepositoryFactory.__doc__
    assert "Create a repository" in pypddl.RepositoryFactory.create.__doc__


def test_format_bindings_emit_reparseable_pddl():
    parser = pypddl.Parser(
        """
(define (domain py-format)
  (:requirements :typing :numeric-fluents)
  (:types item)
  (:predicates (ready ?x - item))
  (:functions (cost ?x - item))
  (:action spend
    :parameters (?x - item)
    :precondition (ready ?x)
    :effect (and (ready ?x) (increase (cost ?x) 1.5)))
)
"""
    )
    task = parser.parse_task(
        """
(define (problem py-format-task)
  (:domain py-format)
  (:objects package - item)
  (:init (ready package) (= (cost package) 0))
  (:goal (ready package))
  (:metric minimize (cost package))
)
"""
    )

    domain_text = pypddl.format_domain(parser.domain())
    task_text = pypddl.format_task(task)

    assert "(:types item - object)" in domain_text
    assert "(increase (cost ?x) 1.5)" in domain_text
    assert "(:metric minimize (cost package))" in task_text

    reparsed = pypddl.Parser(domain_text)
    reparsed_task = reparsed.parse_task(task_text)

    assert reparsed.domain().get_name() == parser.domain().get_name()
    assert reparsed_task.get_name() == task.get_name()
    assert reparsed_task.has_metric()


def test_parser_views_keep_temporary_parser_alive():
    domain = pypddl.Parser(
        """
(define (domain py-temporary-parser)
  (:predicates (p))
)
"""
    ).domain()
    task = pypddl.Parser(
        """
(define (domain py-temporary-parser-task-domain)
  (:predicates (p))
)
"""
    ).parse_task(
        """
(define (problem py-temporary-parser-task)
  (:domain py-temporary-parser-task-domain)
  (:init)
  (:goal (and))
)
"""
    )

    assert domain.get_name() == "py-temporary-parser"
    assert task.get_name() == "py-temporary-parser-task"
    assert task.get_domain().get_name() == "py-temporary-parser-task-domain"


def test_child_view_getters_keep_parent_views_alive():
    domain = pypddl.Parser(
        """
(define (domain py-temporary-child-view-domain)
  (:predicates (p))
)
"""
    ).parse_task(
        """
(define (problem py-temporary-child-view-task)
  (:domain py-temporary-child-view-domain)
  (:init)
  (:goal (p))
)
"""
    ).get_domain()

    assert domain.get_name() == "py-temporary-child-view-domain"


def test_optional_child_view_getters_keep_parent_views_alive():
    domain = pypddl.Parser(
        """
(define (domain py-temporary-optional-action)
  (:predicates (p))
  (:action a :parameters () :precondition (p) :effect (and (p)))
)
"""
    ).domain()
    action = domain.get_actions()[0]
    action_precondition = action.get_precondition()
    action_effect = action.get_effect()
    del action

    assert isinstance(action_precondition.get_variant(), pypddl.ConditionLiteral)
    assert isinstance(action_effect.get_variant(), pypddl.EffectAnd)

    task = pypddl.Parser(
        """
(define (domain py-temporary-optional-task)
  (:requirements :numeric-fluents)
  (:predicates (p))
  (:functions (total-cost))
)
"""
    ).parse_task(
        """
(define (problem py-temporary-optional-task-problem)
  (:domain py-temporary-optional-task)
  (:init (= (total-cost) 0))
  (:goal (p))
  (:metric minimize (total-cost))
)
"""
    )
    task_goal = task.get_goal()
    task_metric = task.get_metric()
    del task

    assert isinstance(task_goal.get_variant(), pypddl.ConditionLiteral)
    assert task_metric.is_minimize()
    assert isinstance(task_metric.get_expression().get_variant(), pypddl.FunctionTerm)


def test_translation_views_keep_temporary_inputs_alive():
    original_domain = pypddl.translate_domain(
        pypddl.Parser(
            """
(define (domain py-temporary-translation-domain)
  (:predicates (p))
)
"""
        ).domain()
    ).original_domain

    assert original_domain.get_name() == "py-temporary-translation-domain"

    parser = pypddl.Parser(
        """
(define (domain py-temporary-translation-task-domain)
  (:predicates (p))
)
"""
    )
    domain_translation = pypddl.translate_domain(parser.domain())
    original_task = pypddl.translate_task(
        parser.parse_task(
            """
(define (problem py-temporary-translation-task)
  (:domain py-temporary-translation-task-domain)
  (:init)
  (:goal (p))
)
"""
        ),
        domain_translation,
    ).original_task
    del parser
    del domain_translation

    assert original_task.get_name() == "py-temporary-translation-task"
    assert original_task.get_domain().get_name() == "py-temporary-translation-task-domain"


def test_translation_bindings_return_translated_views():
    parser = pypddl.Parser(
        """
(define (domain py-translate)
  (:predicates (p) (q))
  (:action a :parameters () :precondition (or (p) (q)) :effect (and (p)))
)
"""
    )
    domain = parser.domain()
    task = parser.parse_task(
        """
(define (problem py-translate-task)
  (:domain py-translate)
  (:init)
  (:goal (p))
)
"""
    )

    options = pypddl.TranslatorOptions()
    assert options.remove_typing is True

    domain_translation = pypddl.translate_domain(domain, options)
    problem_translation = pypddl.translate_task(task, domain_translation, options)
    temporary_translated_task = pypddl.translate_task(task, domain_translation, options).translated_task

    assert domain_translation.original_domain.get_name() == "py-translate"
    assert domain_translation.translated_domain.get_num_actions() >= 1
    assert len(domain_translation.translated_domain.get_actions()) >= 1
    assert problem_translation.original_task.get_name() == "py-translate-task"
    assert problem_translation.translated_task.get_domain().get_name() == domain_translation.translated_domain.get_name()
    assert temporary_translated_task.get_domain().get_name() == domain_translation.translated_domain.get_name()


def test_translator_options_control_typing_removal():
    parser = pypddl.Parser(
        """
(define (domain py-typed-translate)
  (:requirements :strips :typing :numeric-fluents)
  (:types thing)
  (:predicates (p ?x - thing) (done ?x - thing))
  (:functions (fuel ?x - thing))
  (:action a
    :parameters (?x - thing)
    :precondition (p ?x)
    :effect (and (done ?x) (increase (fuel ?x) 1)))
)
"""
    )

    keep_typing = pypddl.TranslatorOptions()
    keep_typing.remove_typing = False

    kept = pypddl.translate_domain(parser.domain(), keep_typing).translated_domain
    stripped = pypddl.translate_domain(parser.domain()).translated_domain

    assert kept.get_num_types() > 0
    assert stripped.get_num_types() == 0

    for predicate in kept.get_predicates():
        if predicate.get_name() == "thing":
            continue
        for parameter in predicate.get_parameters():
            assert parameter.get_num_types() > 0

    for predicate in stripped.get_predicates():
        for parameter in predicate.get_parameters():
            assert parameter.get_num_types() == 0


def test_repository_factory_binding_creates_repositories():
    factory = pypddl.RepositoryFactory()
    repository = factory.create()

    assert isinstance(repository, pypddl.Repository)


def test_repository_view_keeps_temporary_repository_alive():
    view = pypddl.RepositoryFactory().create().get_or_create(pypddl.TypeBuilder("temporary-type"))

    assert view.get_name() == "temporary-type"


def test_semantic_exceptions_have_expected_hierarchy():
    semantic_error_names = [
        "ParseError",
        "MissingDomainError",
        "MismatchedDomainError",
        "UnsupportedRequirementError",
        "MissingRequirementError",
        "UndefinedTypeError",
        "UndefinedPredicateError",
        "UndefinedObjectError",
        "UndefinedVariableError",
        "UndefinedFunctionError",
        "ArityMismatchError",
        "TypeMismatchError",
        "InvalidMetricError",
        "InvalidNumericConstraintError",
        "InvalidNumericEffectError",
        "InvalidProbabilisticEffectError",
        "InvalidEqualityError",
    ]
    for name in semantic_error_names:
        assert issubclass(getattr(pypddl, name), pypddl.SemanticError)

    duplicate_error_names = [
        "DuplicateTypeError",
        "DuplicatePredicateError",
        "DuplicateObjectError",
        "DuplicateVariableError",
        "DuplicateFunctionError",
    ]
    for name in duplicate_error_names:
        error = getattr(pypddl, name)
        assert issubclass(error, pypddl.DuplicateDefinitionError)
        assert issubclass(error, pypddl.SemanticError)


def build(repository, builder):
    return repository.get_or_create(builder)


def make_literal(repository, predicate, terms, positive=True):
    atom = build(repository, pypddl.AtomBuilder(predicate, list(terms)))
    return build(repository, pypddl.LiteralBuilder(positive, atom))


def make_condition(repository, predicate, terms, positive=True):
    condition_literal = build(repository, pypddl.ConditionLiteralBuilder(make_literal(repository, predicate, terms, positive)))
    return build(repository, pypddl.ConditionBuilder(condition_literal))


def make_effect(repository, literal):
    effect_literal = build(repository, pypddl.EffectLiteralBuilder(literal))
    return build(repository, pypddl.EffectBuilder(effect_literal))


def make_condition_and(repository, conditions):
    conjunction = build(repository, pypddl.ConditionAndBuilder(list(conditions)))
    return build(repository, pypddl.ConditionBuilder(conjunction))


def make_effect_and(repository, effects):
    conjunction = build(repository, pypddl.EffectAndBuilder(list(effects)))
    return build(repository, pypddl.EffectBuilder(conjunction))


def make_number_expression(repository, value):
    number = build(repository, pypddl.FunctionExpressionNumberBuilder(value))
    return build(repository, pypddl.FunctionExpressionBuilder(number))


def test_builders_expose_defaulted_mutable_fields():
    factory = pypddl.RepositoryFactory()
    repository = factory.create()

    object_type = build(repository, pypddl.TypeBuilder("object"))
    variable = build(repository, pypddl.VariableBuilder("x"))
    parameter = build(repository, pypddl.ParameterBuilder(variable))
    predicate = build(repository, pypddl.PredicateBuilder("p"))
    term = build(repository, pypddl.TermBuilder(variable))
    literal = make_literal(repository, predicate, [term])
    condition = make_condition(repository, predicate, [term])
    effect = make_effect(repository, literal)

    parameter_builder = pypddl.ParameterBuilder(variable)
    assert parameter_builder.variable == variable.get_index()
    assert parameter_builder.types == []
    parameter_builder.types = [object_type.get_index()]
    assert parameter_builder.types == [object_type.get_index()]

    action_builder = pypddl.ActionBuilder("a")
    assert action_builder.name == "a"
    assert action_builder.parameters == []
    assert action_builder.precondition is None
    assert action_builder.effect is None
    action_builder.name = "mutated-action"
    action_builder.parameters = [parameter.get_index()]
    action_builder.precondition = condition.get_index()
    action_builder.effect = effect.get_index()
    assert action_builder.parameters == [parameter.get_index()]
    assert action_builder.precondition == condition.get_index()
    assert action_builder.effect == effect.get_index()
    action = build(repository, action_builder)

    domain_builder = pypddl.DomainBuilder("d")
    assert domain_builder.requirements == []
    assert domain_builder.types == []
    assert domain_builder.constants == []
    assert domain_builder.predicates == []
    assert domain_builder.functions == []
    assert domain_builder.actions == []
    assert domain_builder.axioms == []
    domain_builder.name = "mutated-domain"
    domain_builder.types = [object_type.get_index()]
    domain_builder.predicates = [predicate.get_index()]
    domain_builder.actions = [action.get_index()]
    assert domain_builder.types == [object_type.get_index()]
    assert domain_builder.predicates == [predicate.get_index()]
    assert domain_builder.actions == [action.get_index()]
    domain = build(repository, domain_builder)

    task_builder = pypddl.TaskBuilder("t", domain)
    assert task_builder.name == "t"
    assert task_builder.domain == domain.get_index()
    assert task_builder.requirements == []
    assert task_builder.objects == []
    assert task_builder.initial_literals == []
    assert task_builder.initial_function_values == []
    assert task_builder.goal is None
    assert task_builder.metric is None
    assert task_builder.predicates == []
    assert task_builder.axioms == []
    task_builder.name = "mutated-task"
    task_builder.goal = condition.get_index()
    assert task_builder.goal == condition.get_index()
    task = build(repository, task_builder)

    assert action.get_name() == "mutated-action"
    assert action.get_num_parameters() == 1
    assert action.has_precondition()
    assert action.has_effect()
    assert domain.get_name() == "mutated-domain"
    assert domain.get_num_types() == 1
    assert domain.get_num_predicates() == 1
    assert domain.get_num_actions() == 1
    assert task.get_name() == "mutated-task"
    assert task.get_domain() == domain
    assert task.has_goal()


def test_repository_constructs_domain_and_task_programmatically():
    factory = pypddl.RepositoryFactory()
    repository = factory.create()
    strips = build(repository, pypddl.RequirementBuilder(pypddl.RequirementKind.Strips))
    object_type = build(repository, pypddl.TypeBuilder("object"))
    ball = build(repository, pypddl.ObjectBuilder("ball", [object_type]))
    variable = build(repository, pypddl.VariableBuilder("x"))
    parameter = build(repository, pypddl.ParameterBuilder(variable, [object_type]))
    predicate = build(repository, pypddl.PredicateBuilder("holding", [parameter]))
    variable_term = build(repository, pypddl.TermBuilder(variable))
    literal = make_literal(repository, predicate, [variable_term])
    precondition = make_condition(repository, predicate, [variable_term])
    effect = make_effect(repository, literal)
    action = build(repository, pypddl.ActionBuilder("pick", [parameter], precondition, effect))
    domain = build(
        repository,
        pypddl.DomainBuilder(
            "programmatic",
            requirements=[strips],
            types=[object_type],
            constants=[ball],
            predicates=[predicate],
            actions=[action],
        ),
    )
    task = build(repository, pypddl.TaskBuilder("programmatic-task", domain))

    assert domain.get_name() == "programmatic"
    assert domain.get_num_requirements() == 1
    assert domain.get_num_types() == 1
    assert domain.get_num_constants() == 1
    assert domain.get_num_predicates() == 1
    assert domain.get_num_actions() == 1
    assert [action.get_name() for action in domain.get_actions()] == ["pick"]
    assert domain.get_types()[0].get_name() == "object"
    assert domain.get_types()[0].get_num_bases() == 0
    assert domain.get_constants()[0].get_name() == "ball"
    assert domain.get_constants()[0].get_num_types() == 1
    assert predicate.get_name() == "holding"
    assert predicate.get_num_parameters() == 1
    assert predicate.get_parameters()[0].get_variable().get_name() == "x"
    assert predicate.get_parameters()[0].get_num_types() == 1
    assert literal.is_positive()
    assert literal.get_atom().get_predicate().get_name() == "holding"
    assert literal.get_atom().get_num_terms() == 1
    assert int(precondition.get_index()) >= 0
    assert int(effect.get_index()) >= 0
    assert action.has_precondition()
    assert action.get_precondition() is not None
    assert action.has_effect()
    assert action.get_effect() is not None
    assert task.get_name() == "programmatic-task"
    assert task.get_domain().get_name() == domain.get_name()
    assert task.get_num_requirements() == 0
    assert task.get_num_objects() == 0
    assert task.get_num_initial_literals() == 0
    assert task.get_num_initial_function_values() == 0
    assert task.get_num_predicates() == 0
    assert task.get_num_axioms() == 0
    assert not task.has_goal()
    assert not task.has_metric()


def test_repository_constructs_numeric_function_task_bits():
    factory = pypddl.RepositoryFactory()
    repository = factory.create()
    numeric = build(repository, pypddl.RequirementBuilder(pypddl.RequirementKind.NumericFluents))
    object_type = build(repository, pypddl.TypeBuilder("object"))
    number_type = build(repository, pypddl.TypeBuilder("number"))
    variable = build(repository, pypddl.VariableBuilder("x"))
    parameter = build(repository, pypddl.ParameterBuilder(variable, [object_type]))
    location = build(repository, pypddl.ObjectBuilder("l1", [object_type]))
    fluent = build(repository, pypddl.FunctionSkeletonBuilder("fuel", [parameter], number_type))
    term = build(repository, pypddl.TermBuilder(variable))
    function_term = build(repository, pypddl.FunctionTermBuilder(fluent, [term]))
    zero = make_number_expression(repository, 0.0)
    one = make_number_expression(repository, 1.0)
    numeric_node = build(repository, pypddl.ConditionNumericConstraintBuilder(pypddl.BinaryComparator.GreaterEqual, zero, zero))
    condition = build(repository, pypddl.ConditionBuilder(numeric_node))
    numeric_effect_node = build(repository, pypddl.EffectNumericBuilder(pypddl.NumericEffectOperator.Assign, fluent, [term], one))
    effect = build(repository, pypddl.EffectBuilder(numeric_effect_node))
    action = build(repository, pypddl.ActionBuilder("refuel", [parameter], condition, effect))
    initial_value = build(repository, pypddl.InitialFunctionValueBuilder(function_term, zero))
    metric = build(repository, pypddl.MetricBuilder(True, one))
    domain = build(
        repository,
        pypddl.DomainBuilder(
            "numeric-programmatic",
            requirements=[numeric],
            types=[object_type, number_type],
            functions=[fluent],
            actions=[action],
        ),
    )
    task = build(
        repository,
        pypddl.TaskBuilder(
            "numeric-programmatic-task",
            domain,
            objects=[location],
            initial_function_values=[initial_value],
            goal=condition,
            metric=metric,
        ),
    )

    assert domain.get_functions()[0].get_name() == "fuel"
    assert domain.get_functions()[0].get_num_parameters() == 1
    assert domain.get_functions()[0].get_type().get_name() == "number"
    assert task.get_initial_function_values()[0].get_function().get_function().get_name() == "fuel"
    assert task.get_initial_function_values()[0].get_function().get_num_terms() == 1
    assert condition.get_variant().get_comparator() == pypddl.BinaryComparator.GreaterEqual
    assert condition.get_variant().get_left().get_variant().get_value() == 0.0
    assert condition.get_variant().get_right().get_variant().get_value() == 0.0
    assert effect.get_variant().get_operator() == pypddl.NumericEffectOperator.Assign
    assert effect.get_variant().get_expression().get_variant().get_value() == 1.0
    assert task.has_goal()
    assert task.has_metric()
    assert task.get_metric().is_minimize()
    assert task.get_metric().get_expression().get_variant().get_value() == 1.0


def test_parser_path_entry_points_and_strict_options():
    from pathlib import Path
    from tempfile import TemporaryDirectory

    with TemporaryDirectory() as tmp_dir:
        tmp_path = Path(tmp_dir)
        domain_path = tmp_path / "domain.pddl"
        task_path = tmp_path / "task.pddl"
        domain_path.write_text("""
(define (domain py-path)
  (:predicates (p))
  (:action a :parameters () :precondition (p) :effect (and (p)))
)
""")
        task_path.write_text("""
(define (problem py-path-task)
  (:domain py-path)
  (:init (p))
  (:goal (p))
)
""")

        options = pypddl.ParserOptions()
        assert options.strict is False
        parser = pypddl.Parser(domain_path, options)
        domain = parser.domain()
        task = parser.parse_task(task_path)

        assert domain.get_name() == "py-path"
        assert task.get_domain().get_name() == "py-path"


def test_views_expose_typed_indices():
    parser = pypddl.Parser("""
(define (domain py-index)
  (:predicates (p))
  (:action a :parameters () :precondition (p) :effect (and (p)))
)
""")
    domain = parser.domain()

    domain_index = domain.get_index()
    next_domain_index = pypddl.DomainIndex(1)

    assert isinstance(domain_index, pypddl.DomainIndex)
    assert int(domain_index) == 0
    assert operator.index(domain_index) == 0
    assert str(domain_index) == repr(domain_index) == "DomainIndex(0)"
    assert hash(domain_index) == 0
    assert domain_index == domain.get_index()
    assert domain_index < next_domain_index
    assert next_domain_index > domain_index

    predicate_index = domain.get_predicates()[0].get_index()
    assert isinstance(predicate_index, pypddl.PredicateIndex)
    assert int(predicate_index) == 0
    assert str(predicate_index) == repr(predicate_index) == "PredicateIndex(0)"


def test_views_are_hashable_and_compare_by_identity():
    parser = pypddl.Parser("""
(define (domain py-view-identity)
  (:predicates (p) (q))
)
""")
    domain = parser.domain()
    same_domain = parser.domain()
    other_domain = pypddl.Parser("""
(define (domain py-other-view-identity)
  (:predicates (p))
)
""").domain()

    assert domain == same_domain
    assert hash(domain) == hash(same_domain)
    assert domain != other_domain
    assert len({domain, same_domain, other_domain}) == 2
    assert str(domain) == repr(domain)
    assert "py-view-identity" in str(domain)

    first_predicate = domain.get_predicates()[0]
    same_predicate = same_domain.get_predicates()[0]
    second_predicate = domain.get_predicates()[1]

    assert first_predicate == same_predicate
    assert hash(first_predicate) == hash(same_predicate)
    assert first_predicate != second_predicate
    assert len({first_predicate, same_predicate, second_predicate}) == 2
    assert str(first_predicate) == repr(first_predicate)
    assert "p" in str(first_predicate)


def test_semantic_error_uses_typed_exception():
    try:
        parser = pypddl.Parser("""
(define (domain py-error)
  (:predicates (p) (p))
)
""")
    except pypddl.DuplicatePredicateError as error:
        assert "Duplicate predicate" in str(error)
    else:
        raise AssertionError("expected duplicate predicate diagnostic")


def test_strict_mode_missing_requirement_uses_typed_exception():
    options = pypddl.ParserOptions()
    options.strict = True

    try:
        pypddl.Parser("""
(define (domain py-missing-requirement)
  (:predicates (p) (q))
  (:action a
    :parameters ()
    :precondition (or (p) (q))
    :effect (and))
)
""", options)
    except pypddl.MissingRequirementError as error:
        message = str(error)
        assert ":disjunctive-preconditions" in message
        assert "line 6, column 19" in message
    else:
        raise AssertionError("expected missing requirement diagnostic")


def test_mismatched_task_domain_uses_typed_exception():
    parser = pypddl.Parser("""
(define (domain py-domain)
  (:predicates (p))
)
""")

    try:
        parser.parse_task("""
(define (problem py-domain-task)
  (:domain other-domain)
  (:init)
  (:goal (and))
)
""")
    except pypddl.MismatchedDomainError as error:
        message = str(error)
        assert "py-domain" in message
        assert "other-domain" in message
    else:
        raise AssertionError("expected mismatched domain diagnostic")


def test_translate_task_from_different_domain_translation_uses_typed_exception():
    first_parser = pypddl.Parser("""
(define (domain py-first-translation-domain)
  (:predicates (p))
)
""")
    second_parser = pypddl.Parser("""
(define (domain py-second-translation-domain)
  (:predicates (q))
)
""")
    first_translation = pypddl.translate_domain(first_parser.domain())
    second_task = second_parser.parse_task("""
(define (problem py-second-translation-task)
  (:domain py-second-translation-domain)
  (:init)
  (:goal (q))
)
""")

    try:
        pypddl.translate_task(second_task, first_translation)
    except pypddl.MismatchedDomainError as error:
        message = str(error)
        assert "py-first-translation-domain" in message
        assert "py-second-translation-domain" in message
    else:
        raise AssertionError("expected mismatched domain diagnostic")


def test_translate_task_task_only_equality_uses_typed_exception():
    parser = pypddl.Parser("""
(define (domain py-task-only-equality)
  (:requirements :strips)
  (:predicates (p))
)
""")
    domain_translation = pypddl.translate_domain(parser.domain())
    task = parser.parse_task("""
(define (problem py-task-only-equality-task)
  (:domain py-task-only-equality)
  (:requirements :equality)
  (:objects o)
  (:init)
  (:goal (p))
)
""")

    try:
        pypddl.translate_task(task, domain_translation)
    except pypddl.InvalidEqualityError as error:
        assert "equality predicate" in str(error)
    else:
        raise AssertionError("expected invalid equality diagnostic")


def test_parse_error_uses_typed_exception():
    try:
        pypddl.Parser("(define (domain broken) (:predicates (p))")
    except pypddl.ParseError as error:
        assert "line 1, column" in str(error)
    else:
        raise AssertionError("expected parse diagnostic")


def test_recursive_variant_views_are_inspectable():
    parser = pypddl.Parser("""
(define (domain py-recursive)
  (:requirements :disjunctive-preconditions :conditional-effects)
  (:predicates (p) (q))
  (:action a
    :parameters ()
    :precondition (or (p) (q))
    :effect (and (when (p) (q))))
)
""")
    domain = parser.domain()

    action = domain.get_actions()[0]
    assert isinstance(action.get_precondition().get_value(), pypddl.ConditionOr)
    assert action.get_precondition().get_value() == action.get_precondition().get_variant()
    assert action.get_precondition().get_variant().get_num_conditions() == 2
    assert [type(child.get_variant()) for child in action.get_precondition().get_variant().get_conditions()] == [pypddl.ConditionLiteral, pypddl.ConditionLiteral]
    assert isinstance(action.get_effect().get_value(), pypddl.EffectAnd)
    assert action.get_effect().get_value() == action.get_effect().get_variant()
    assert action.get_effect().get_variant().get_num_effects() == 1
    assert isinstance(action.get_effect().get_variant().get_effects()[0].get_variant(), pypddl.EffectWhen)


def test_numeric_expression_variant_views_are_inspectable():
    factory = pypddl.RepositoryFactory()
    repository = factory.create()
    number = make_number_expression(repository, 2.0)

    assert isinstance(number.get_value(), pypddl.FunctionExpressionNumber)
    assert number.get_value() == number.get_variant()
    assert number.get_variant().get_value() == 2.0


def test_repository_exposes_recursive_constructors_and_accessors():
    factory = pypddl.RepositoryFactory()
    repository = factory.create()
    object_type = build(repository, pypddl.TypeBuilder("object"))
    number_type = build(repository, pypddl.TypeBuilder("number"))
    variable = build(repository, pypddl.VariableBuilder("x"))
    parameter = build(repository, pypddl.ParameterBuilder(variable, [object_type]))
    predicate = build(repository, pypddl.PredicateBuilder("p", [parameter]))
    term = build(repository, pypddl.TermBuilder(variable))
    assert term.get_value() == term.get_variant()
    assert term.get_value().get_name() == "x"
    literal = make_literal(repository, predicate, [term])
    base_condition = make_condition(repository, predicate, [term])
    condition_not_node = build(repository, pypddl.ConditionNotBuilder(base_condition))
    condition_not = build(repository, pypddl.ConditionBuilder(condition_not_node))
    condition_or_node = build(repository, pypddl.ConditionOrBuilder([base_condition, condition_not]))
    condition_or = build(repository, pypddl.ConditionBuilder(condition_or_node))
    condition_and_node = build(repository, pypddl.ConditionAndBuilder([base_condition, condition_not]))
    condition_and = build(repository, pypddl.ConditionBuilder(condition_and_node))
    condition_imply_node = build(repository, pypddl.ConditionImplyBuilder(base_condition, condition_or))
    condition_imply = build(repository, pypddl.ConditionBuilder(condition_imply_node))
    condition_exists_node = build(repository, pypddl.ConditionExistsBuilder([parameter], condition_imply))
    condition_exists = build(repository, pypddl.ConditionBuilder(condition_exists_node))
    condition_forall_node = build(repository, pypddl.ConditionForallBuilder([parameter], condition_exists))
    condition_forall = build(repository, pypddl.ConditionBuilder(condition_forall_node))

    number = make_number_expression(repository, 1.0)
    unary_node = build(repository, pypddl.UnaryFunctionExpressionBuilder(pypddl.UnaryArithmeticOperator.Minus, number))
    unary = build(repository, pypddl.FunctionExpressionBuilder(unary_node))
    binary_node = build(repository, pypddl.BinaryFunctionExpressionBuilder(pypddl.BinaryArithmeticOperator.Add, number, unary))
    binary = build(repository, pypddl.FunctionExpressionBuilder(binary_node))
    multi_node = build(repository, pypddl.MultiFunctionExpressionBuilder(pypddl.MultiArithmeticOperator.Add, [number, binary]))
    multi = build(repository, pypddl.FunctionExpressionBuilder(multi_node))

    fluent = build(repository, pypddl.FunctionSkeletonBuilder("f", [parameter], number_type))
    numeric_effect_node = build(repository, pypddl.EffectNumericBuilder(pypddl.NumericEffectOperator.Assign, fluent, [term], multi))
    numeric_effect = build(repository, pypddl.EffectBuilder(numeric_effect_node))
    literal_effect = make_effect(repository, literal)
    effect_when_node = build(repository, pypddl.EffectWhenBuilder(condition_forall, literal_effect))
    effect_when = build(repository, pypddl.EffectBuilder(effect_when_node))
    effect_forall_node = build(repository, pypddl.EffectForallBuilder([parameter], effect_when))
    effect_forall = build(repository, pypddl.EffectBuilder(effect_forall_node))
    effect_one_of_node = build(repository, pypddl.EffectOneOfBuilder([literal_effect, numeric_effect]))
    effect_one_of = build(repository, pypddl.EffectBuilder(effect_one_of_node))
    alt1 = build(repository, pypddl.EffectProbabilisticAlternativeBuilder(0.4, literal_effect))
    alt2 = build(repository, pypddl.EffectProbabilisticAlternativeBuilder(0.6, effect_one_of))
    effect_probabilistic_node = build(repository, pypddl.EffectProbabilisticBuilder([alt1, alt2]))
    effect_probabilistic = build(repository, pypddl.EffectBuilder(effect_probabilistic_node))
    axiom = build(repository, pypddl.AxiomBuilder([parameter], literal, condition_forall))
    action = build(repository, pypddl.ActionBuilder("a", [parameter], condition_forall, effect_probabilistic))
    domain = build(repository, pypddl.DomainBuilder("full-builder", types=[object_type, number_type], predicates=[predicate], functions=[fluent], actions=[action], axioms=[axiom]))
    task = build(repository, pypddl.TaskBuilder("full-builder-task", domain, goal=condition_forall, axioms=[axiom]))

    assert isinstance(condition_forall.get_variant(), pypddl.ConditionForall)
    assert condition_forall.get_variant().get_num_parameters() == 1
    assert condition_imply.get_variant().get_left() == base_condition
    assert condition_imply.get_variant().get_right() == condition_or
    assert isinstance(condition_exists.get_variant(), pypddl.ConditionExists)
    assert condition_exists.get_variant().get_num_parameters() == 1
    assert isinstance(condition_or.get_variant(), pypddl.ConditionOr)
    assert condition_or.get_variant().get_num_conditions() == len(condition_or.get_variant().get_conditions())
    assert isinstance(condition_and.get_variant(), pypddl.ConditionAnd)
    assert condition_and.get_variant().get_num_conditions() == len(condition_and.get_variant().get_conditions())
    assert isinstance(effect_forall.get_variant(), pypddl.EffectForall)
    assert effect_forall.get_variant().get_num_parameters() == 1
    assert isinstance(effect_one_of.get_variant(), pypddl.EffectOneOf)
    assert effect_one_of.get_variant().get_num_effects() == 2
    assert isinstance(effect_probabilistic.get_variant(), pypddl.EffectProbabilistic)
    assert effect_probabilistic.get_variant().get_num_alternatives() == 2
    assert isinstance(numeric_effect.get_variant(), pypddl.EffectNumeric)
    assert numeric_effect.get_variant().get_operator() == pypddl.NumericEffectOperator.Assign
    assert numeric_effect.get_variant().get_num_terms() == 1
    assert isinstance(unary.get_variant(), pypddl.UnaryFunctionExpression)
    assert unary.get_variant().get_operator() == pypddl.UnaryArithmeticOperator.Minus
    assert isinstance(binary.get_variant(), pypddl.BinaryFunctionExpression)
    assert binary.get_variant().get_operator() == pypddl.BinaryArithmeticOperator.Add
    assert isinstance(multi.get_variant(), pypddl.MultiFunctionExpression)
    assert multi.get_variant().get_operator() == pypddl.MultiArithmeticOperator.Add
    assert multi.get_variant().get_num_expressions() == 2
    assert len(effect_probabilistic.get_variant().get_alternatives()) == 2
    assert effect_probabilistic.get_variant().get_alternatives()[0].get_probability() == 0.4
    assert domain.get_axioms()[0].get_num_parameters() == 1
    assert isinstance(domain.get_axioms()[0].get_condition().get_variant(), pypddl.ConditionForall)
    assert isinstance(task.get_goal().get_variant(), pypddl.ConditionForall)

if __name__ == "__main__":
    from direct_runner import run_tests

    run_tests(globals())
