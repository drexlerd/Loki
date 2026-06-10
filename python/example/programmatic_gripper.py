"""Build a gripper domain and two tasks programmatically with pypddl."""

from pypddl import formalism as pypddl


def build(repository, builder):
    return repository.get_or_create(builder)


def make_literal(repository, predicate, terms, positive=True):
    atom = build(repository, pypddl.AtomBuilder(predicate, list(terms)))
    return build(repository, pypddl.LiteralBuilder(positive, atom))


def make_condition(repository, predicate, terms, positive=True):
    literal = make_literal(repository, predicate, terms, positive)
    condition_literal = build(repository, pypddl.ConditionLiteralBuilder(literal))
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


def build_gripper():
    factory = pypddl.RepositoryFactory()
    repository = factory.create()
    strips = build(repository, pypddl.RequirementBuilder(pypddl.RequirementKind.Strips))
    typing = build(repository, pypddl.RequirementBuilder(pypddl.RequirementKind.Typing))

    object_t = build(repository, pypddl.TypeBuilder("object"))
    room_t = build(repository, pypddl.TypeBuilder("room", [object_t]))
    ball_t = build(repository, pypddl.TypeBuilder("ball", [object_t]))
    gripper_t = build(repository, pypddl.TypeBuilder("gripper", [object_t]))

    x = build(repository, pypddl.ParameterBuilder(build(repository, pypddl.VariableBuilder("x")), [room_t]))
    y = build(repository, pypddl.ParameterBuilder(build(repository, pypddl.VariableBuilder("y")), [room_t]))
    b = build(repository, pypddl.ParameterBuilder(build(repository, pypddl.VariableBuilder("b")), [ball_t]))
    g = build(repository, pypddl.ParameterBuilder(build(repository, pypddl.VariableBuilder("g")), [gripper_t]))

    at_robby = build(repository, pypddl.PredicateBuilder("at-robby", [x]))
    at = build(repository, pypddl.PredicateBuilder("at", [b, x]))
    free = build(repository, pypddl.PredicateBuilder("free", [g]))
    carry = build(repository, pypddl.PredicateBuilder("carry", [b, g]))

    x_term = build(repository, pypddl.TermBuilder(x.get_variable()))
    y_term = build(repository, pypddl.TermBuilder(y.get_variable()))
    b_term = build(repository, pypddl.TermBuilder(b.get_variable()))
    g_term = build(repository, pypddl.TermBuilder(g.get_variable()))

    move_pre = make_condition(repository, at_robby, [x_term])
    move_eff = make_effect_and(repository, [
        make_effect(repository, make_literal(repository, at_robby, [x_term], False)),
        make_effect(repository, make_literal(repository, at_robby, [y_term])),
    ])
    move = build(repository, pypddl.ActionBuilder("move", [x, y], move_pre, move_eff))

    pick_pre = make_condition_and(repository, [
        make_condition(repository, at, [b_term, x_term]),
        make_condition(repository, at_robby, [x_term]),
        make_condition(repository, free, [g_term]),
    ])
    pick_eff = make_effect_and(repository, [
        make_effect(repository, make_literal(repository, at, [b_term, x_term], False)),
        make_effect(repository, make_literal(repository, free, [g_term], False)),
        make_effect(repository, make_literal(repository, carry, [b_term, g_term])),
    ])
    pick = build(repository, pypddl.ActionBuilder("pick", [b, x, g], pick_pre, pick_eff))

    drop_pre = make_condition_and(repository, [
        make_condition(repository, carry, [b_term, g_term]),
        make_condition(repository, at_robby, [x_term]),
    ])
    drop_eff = make_effect_and(repository, [
        make_effect(repository, make_literal(repository, carry, [b_term, g_term], False)),
        make_effect(repository, make_literal(repository, free, [g_term])),
        make_effect(repository, make_literal(repository, at, [b_term, x_term])),
    ])
    drop = build(repository, pypddl.ActionBuilder("drop", [b, x, g], drop_pre, drop_eff))

    domain = build(
        repository,
        pypddl.DomainBuilder(
            "gripper",
            requirements=[strips, typing],
            types=[object_t, room_t, ball_t, gripper_t],
            predicates=[at_robby, at, free, carry],
            actions=[move, pick, drop],
        ),
    )

    def task(name, balls):
        rooma = build(repository, pypddl.ObjectBuilder("rooma", [room_t]))
        roomb = build(repository, pypddl.ObjectBuilder("roomb", [room_t]))
        left = build(repository, pypddl.ObjectBuilder("left", [gripper_t]))
        right = build(repository, pypddl.ObjectBuilder("right", [gripper_t]))
        ball_objects = [build(repository, pypddl.ObjectBuilder(ball, [ball_t])) for ball in balls]

        rooma_term = build(repository, pypddl.TermBuilder(rooma))
        roomb_term = build(repository, pypddl.TermBuilder(roomb))
        left_term = build(repository, pypddl.TermBuilder(left))
        right_term = build(repository, pypddl.TermBuilder(right))

        initial_literals = [
            make_literal(repository, at_robby, [rooma_term]),
            make_literal(repository, free, [left_term]),
            make_literal(repository, free, [right_term]),
        ]
        goals = []
        for ball in ball_objects:
            ball_term = build(repository, pypddl.TermBuilder(ball))
            initial_literals.append(make_literal(repository, at, [ball_term, rooma_term]))
            goals.append(make_condition(repository, at, [ball_term, roomb_term]))

        return build(
            repository,
            pypddl.TaskBuilder(
                name,
                domain,
                objects=[rooma, roomb, left, right, *ball_objects],
                initial_literals=initial_literals,
                goal=make_condition_and(repository, goals),
            ),
        )

    one_ball = task("gripper-1-ball", ["ball1"])
    two_ball = task("gripper-2-ball", ["ball1", "ball2"])
    return repository, domain, one_ball, two_ball


if __name__ == "__main__":
    _, domain, one_ball, two_ball = build_gripper()
    print(f"domain: {domain.get_name()}, actions={domain.get_num_actions()}, predicates={domain.get_num_predicates()}")
    print(f"task: {one_ball.get_name()}, objects={one_ball.get_num_objects()}, init={one_ball.get_num_initial_literals()}")
    print(f"task: {two_ball.get_name()}, objects={two_ball.get_num_objects()}, init={two_ball.get_num_initial_literals()}")
