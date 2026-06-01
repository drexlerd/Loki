from pypddl import formalism as pypddl


def ix(value):
    return value


def ixs(values):
    return list(values)


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

    domain_translation = pypddl.translate_domain(domain)
    problem_translation = pypddl.translate_task(task, domain_translation)

    assert domain_translation.original_domain.get_name() == "py-translate"
    assert domain_translation.translated_domain.get_num_actions() >= 1
    assert len(domain_translation.translated_domain.get_actions()) >= 1
    assert problem_translation.original_task.get_name() == "py-translate-task"
    assert problem_translation.translated_task.get_domain().get_name() == domain_translation.translated_domain.get_name()


def test_repository_factory_binding_creates_repositories():
    factory = pypddl.RepositoryFactory()
    repository = factory.create()

    assert isinstance(repository, pypddl.Repository)


def build(repository, builder):
    return repository.get_or_create(builder)


def make_literal(repository, predicate, terms, positive=True):
    atom = build(repository, pypddl.AtomBuilder(ix(predicate), ixs(terms)))
    return build(repository, pypddl.LiteralBuilder(positive, ix(atom)))


def make_condition(repository, predicate, terms, positive=True):
    condition_literal = build(repository, pypddl.ConditionLiteralBuilder(ix(make_literal(repository, predicate, terms, positive))))
    return build(repository, pypddl.ConditionBuilder(condition_literal))


def make_effect(repository, literal):
    effect_literal = build(repository, pypddl.EffectLiteralBuilder(ix(literal)))
    return build(repository, pypddl.EffectBuilder(effect_literal))


def make_condition_and(repository, conditions):
    conjunction = build(repository, pypddl.ConditionAndBuilder(ixs(conditions)))
    return build(repository, pypddl.ConditionBuilder(conjunction))


def make_effect_and(repository, effects):
    conjunction = build(repository, pypddl.EffectAndBuilder(ixs(effects)))
    return build(repository, pypddl.EffectBuilder(conjunction))


def make_number_expression(repository, value):
    number = build(repository, pypddl.FunctionExpressionNumberBuilder(value))
    return build(repository, pypddl.FunctionExpressionBuilder(number))


def test_repository_constructs_domain_and_task_programmatically():
    factory = pypddl.RepositoryFactory()
    repository = factory.create()
    strips = build(repository, pypddl.RequirementBuilder(pypddl.RequirementKind.Strips))
    object_type = build(repository, pypddl.TypeBuilder("object"))
    ball = build(repository, pypddl.ObjectBuilder("ball", ixs([object_type])))
    variable = build(repository, pypddl.VariableBuilder("x"))
    parameter = build(repository, pypddl.ParameterBuilder(ix(variable), ixs([object_type])))
    predicate = build(repository, pypddl.PredicateBuilder("holding", ixs([parameter])))
    variable_term = build(repository, pypddl.TermBuilder(variable))
    literal = make_literal(repository, predicate, [variable_term])
    precondition = make_condition(repository, predicate, [variable_term])
    effect = make_effect(repository, literal)
    action = build(repository, pypddl.ActionBuilder("pick", ixs([parameter]), ix(precondition), ix(effect)))
    domain = build(
        repository,
        pypddl.DomainBuilder(
            "programmatic",
            requirements=ixs([strips]),
            types=ixs([object_type]),
            constants=ixs([ball]),
            predicates=ixs([predicate]),
            actions=ixs([action]),
        ),
    )
    task = build(repository, pypddl.TaskBuilder("programmatic-task", ix(domain)))

    assert domain.get_name() == "programmatic"
    assert domain.get_num_requirements() == 1
    assert domain.get_num_types() == 1
    assert domain.get_num_constants() == 1
    assert domain.get_num_predicates() == 1
    assert domain.get_num_actions() == 1
    assert [action.get_name() for action in domain.get_actions()] == ["pick"]
    assert domain.get_types()[0].get_name() == "object"
    assert domain.get_constants()[0].get_name() == "ball"
    assert predicate.get_name() == "holding"
    assert predicate.get_num_parameters() == 1
    assert predicate.get_parameters()[0].get_variable().get_name() == "x"
    assert literal.is_positive()
    assert literal.get_atom().get_predicate().get_name() == "holding"
    assert int(precondition.get_index()) >= 0
    assert int(effect.get_index()) >= 0
    assert action.has_precondition()
    assert action.get_precondition() is not None
    assert action.has_effect()
    assert action.get_effect() is not None
    assert task.get_name() == "programmatic-task"
    assert task.get_domain().get_name() == domain.get_name()


def test_repository_constructs_numeric_function_task_bits():
    factory = pypddl.RepositoryFactory()
    repository = factory.create()
    numeric = build(repository, pypddl.RequirementBuilder(pypddl.RequirementKind.NumericFluents))
    object_type = build(repository, pypddl.TypeBuilder("object"))
    number_type = build(repository, pypddl.TypeBuilder("number"))
    variable = build(repository, pypddl.VariableBuilder("x"))
    parameter = build(repository, pypddl.ParameterBuilder(ix(variable), ixs([object_type])))
    location = build(repository, pypddl.ObjectBuilder("l1", ixs([object_type])))
    fluent = build(repository, pypddl.FunctionSkeletonBuilder("fuel", ixs([parameter]), ix(number_type)))
    term = build(repository, pypddl.TermBuilder(variable))
    function_term = build(repository, pypddl.FunctionTermBuilder(ix(fluent), ixs([term])))
    zero = make_number_expression(repository, 0.0)
    one = make_number_expression(repository, 1.0)
    numeric_node = build(repository, pypddl.ConditionNumericConstraintBuilder(pypddl.BinaryComparator.GreaterEqual, ix(zero), ix(zero)))
    condition = build(repository, pypddl.ConditionBuilder(numeric_node))
    numeric_effect_node = build(repository, pypddl.EffectNumericBuilder(pypddl.NumericEffectOperator.Assign, ix(fluent), ixs([term]), ix(one)))
    effect = build(repository, pypddl.EffectBuilder(numeric_effect_node))
    action = build(repository, pypddl.ActionBuilder("refuel", ixs([parameter]), ix(condition), ix(effect)))
    initial_value = build(repository, pypddl.InitialFunctionValueBuilder(ix(function_term), ix(zero)))
    metric = build(repository, pypddl.MetricBuilder(True, ix(one)))
    domain = build(
        repository,
        pypddl.DomainBuilder(
            "numeric-programmatic",
            requirements=ixs([numeric]),
            types=ixs([object_type, number_type]),
            functions=ixs([fluent]),
            actions=ixs([action]),
        ),
    )
    task = build(
        repository,
        pypddl.TaskBuilder(
            "numeric-programmatic-task",
            ix(domain),
            objects=ixs([location]),
            initial_function_values=ixs([initial_value]),
            goal=ix(condition),
            metric=ix(metric),
        ),
    )

    assert domain.get_functions()[0].get_name() == "fuel"
    assert domain.get_functions()[0].get_type().get_name() == "number"
    assert task.get_initial_function_values()[0].get_function().get_function().get_name() == "fuel"
    assert task.get_metric().is_minimize()


def test_parser_path_entry_points_and_strict_options():
    import tempfile
    from pathlib import Path

    tmp_path = Path(tempfile.mkdtemp())
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

    assert isinstance(domain.get_index(), pypddl.DomainIndex)
    assert isinstance(domain.get_index(), pypddl.DomainIndex)
    assert int(domain.get_index()) == 0
    assert domain.get_index() == domain.get_index()
    assert isinstance(domain.get_predicates()[0].get_index(), pypddl.PredicateIndex)
    assert int(domain.get_predicates()[0].get_index()) == 0


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


def test_parse_error_uses_typed_exception():
    try:
        parser = pypddl.Parser("(define (domain broken) (:predicates (p))")
    except pypddl.ParseError:
        pass
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
    assert isinstance(action.get_precondition().get_variant(), pypddl.ConditionOr)
    assert isinstance(action.get_precondition().get_variant(), pypddl.ConditionOr)
    assert [type(child.get_variant()) for child in action.get_precondition().get_variant().get_conditions()] == [pypddl.ConditionLiteral, pypddl.ConditionLiteral]
    assert isinstance(action.get_effect().get_variant(), pypddl.EffectAnd)
    assert isinstance(action.get_effect().get_variant(), pypddl.EffectAnd)
    assert isinstance(action.get_effect().get_variant().get_effects()[0].get_variant(), pypddl.EffectWhen)
    assert isinstance(action.get_effect().get_variant().get_effects()[0].get_variant(), pypddl.EffectWhen)


def test_numeric_expression_variant_views_are_inspectable():
    factory = pypddl.RepositoryFactory()
    repository = factory.create()
    number = make_number_expression(repository, 2.0)

    assert isinstance(number.get_variant(), pypddl.FunctionExpressionNumber)
    assert isinstance(number.get_variant(), pypddl.FunctionExpressionNumber)
    assert number.get_variant().get_value() == 2.0


def test_repository_exposes_recursive_constructors_and_accessors():
    factory = pypddl.RepositoryFactory()
    repository = factory.create()
    object_type = build(repository, pypddl.TypeBuilder("object"))
    number_type = build(repository, pypddl.TypeBuilder("number"))
    variable = build(repository, pypddl.VariableBuilder("x"))
    parameter = build(repository, pypddl.ParameterBuilder(ix(variable), ixs([object_type])))
    predicate = build(repository, pypddl.PredicateBuilder("p", ixs([parameter])))
    term = build(repository, pypddl.TermBuilder(variable))
    literal = make_literal(repository, predicate, [term])
    base_condition = make_condition(repository, predicate, [term])
    condition_not_node = build(repository, pypddl.ConditionNotBuilder(ix(base_condition)))
    condition_not = build(repository, pypddl.ConditionBuilder(condition_not_node))
    condition_or_node = build(repository, pypddl.ConditionOrBuilder(ixs([base_condition, condition_not])))
    condition_or = build(repository, pypddl.ConditionBuilder(condition_or_node))
    condition_imply_node = build(repository, pypddl.ConditionImplyBuilder(ix(base_condition), ix(condition_or)))
    condition_imply = build(repository, pypddl.ConditionBuilder(condition_imply_node))
    condition_exists_node = build(repository, pypddl.ConditionExistsBuilder(ixs([parameter]), ix(condition_imply)))
    condition_exists = build(repository, pypddl.ConditionBuilder(condition_exists_node))
    condition_forall_node = build(repository, pypddl.ConditionForallBuilder(ixs([parameter]), ix(condition_exists)))
    condition_forall = build(repository, pypddl.ConditionBuilder(condition_forall_node))

    number = make_number_expression(repository, 1.0)
    unary_node = build(repository, pypddl.UnaryFunctionExpressionBuilder(pypddl.UnaryArithmeticOperator.Minus, ix(number)))
    unary = build(repository, pypddl.FunctionExpressionBuilder(unary_node))
    binary_node = build(repository, pypddl.BinaryFunctionExpressionBuilder(pypddl.BinaryArithmeticOperator.Add, ix(number), ix(unary)))
    binary = build(repository, pypddl.FunctionExpressionBuilder(binary_node))
    multi_node = build(repository, pypddl.MultiFunctionExpressionBuilder(pypddl.MultiArithmeticOperator.Add, ixs([number, binary])))
    multi = build(repository, pypddl.FunctionExpressionBuilder(multi_node))

    fluent = build(repository, pypddl.FunctionSkeletonBuilder("f", ixs([parameter]), ix(number_type)))
    numeric_effect_node = build(repository, pypddl.EffectNumericBuilder(pypddl.NumericEffectOperator.Assign, ix(fluent), ixs([term]), ix(multi)))
    numeric_effect = build(repository, pypddl.EffectBuilder(numeric_effect_node))
    literal_effect = make_effect(repository, literal)
    effect_when_node = build(repository, pypddl.EffectWhenBuilder(ix(condition_forall), ix(literal_effect)))
    effect_when = build(repository, pypddl.EffectBuilder(effect_when_node))
    effect_forall_node = build(repository, pypddl.EffectForallBuilder(ixs([parameter]), ix(effect_when)))
    effect_forall = build(repository, pypddl.EffectBuilder(effect_forall_node))
    effect_one_of_node = build(repository, pypddl.EffectOneOfBuilder(ixs([literal_effect, numeric_effect])))
    effect_one_of = build(repository, pypddl.EffectBuilder(effect_one_of_node))
    alt1 = build(repository, pypddl.EffectProbabilisticAlternativeBuilder(0.4, ix(literal_effect)))
    alt2 = build(repository, pypddl.EffectProbabilisticAlternativeBuilder(0.6, ix(effect_one_of)))
    effect_probabilistic_node = build(repository, pypddl.EffectProbabilisticBuilder(ixs([alt1, alt2])))
    effect_probabilistic = build(repository, pypddl.EffectBuilder(effect_probabilistic_node))
    axiom = build(repository, pypddl.AxiomBuilder(ixs([parameter]), ix(literal), ix(condition_forall)))
    action = build(repository, pypddl.ActionBuilder("a", ixs([parameter]), ix(condition_forall), ix(effect_probabilistic)))
    domain = build(repository, pypddl.DomainBuilder("full-builder", types=ixs([object_type, number_type]), predicates=ixs([predicate]), functions=ixs([fluent]), actions=ixs([action]), axioms=ixs([axiom])))
    task = build(repository, pypddl.TaskBuilder("full-builder-task", ix(domain), goal=ix(condition_forall), axioms=ixs([axiom])))

    assert isinstance(condition_forall.get_variant(), pypddl.ConditionForall)
    assert isinstance(effect_forall.get_variant(), pypddl.EffectForall)
    assert isinstance(effect_probabilistic.get_variant(), pypddl.EffectProbabilistic)
    assert isinstance(multi.get_variant(), pypddl.MultiFunctionExpression)
    assert len(effect_probabilistic.get_variant().get_alternatives()) == 2
    assert isinstance(domain.get_axioms()[0].get_condition().get_variant(), pypddl.ConditionForall)
    assert isinstance(task.get_goal().get_variant(), pypddl.ConditionForall)
