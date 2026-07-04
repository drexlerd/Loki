"""Build a gripper domain and two tasks programmatically with pypddl."""

from pypddl import formalism as pypddl


def build(repository, builder):
    return repository.get_or_create(builder)


def make_literal(repository, predicate, terms, positive=True):
    atom = build(repository, pypddl.AtomData(predicate, list(terms)))
    return build(repository, pypddl.LiteralData(atom, positive))


def make_condition(repository, predicate, terms, positive=True):
    literal = make_literal(repository, predicate, terms, positive)
    condition_literal = build(repository, pypddl.ConditionLiteralData(literal))
    return build(repository, pypddl.ConditionData(condition_literal))


def make_effect(repository, literal):
    effect_literal = build(repository, pypddl.EffectLiteralData(literal))
    return build(repository, pypddl.EffectData(effect_literal))


def make_condition_and(repository, conditions):
    conjunction = build(repository, pypddl.ConditionAndData(list(conditions)))
    return build(repository, pypddl.ConditionData(conjunction))


def make_effect_and(repository, effects):
    conjunction = build(repository, pypddl.EffectAndData(list(effects)))
    return build(repository, pypddl.EffectData(conjunction))


def build_gripper():
    repository = pypddl.RepositoryFactory().create()
    strips = build(repository, pypddl.RequirementData(pypddl.RequirementKind.Strips))
    typing = build(repository, pypddl.RequirementData(pypddl.RequirementKind.Typing))

    object_t = build(repository, pypddl.TypeData("object"))
    room_t = build(repository, pypddl.TypeData("room", [object_t]))
    ball_t = build(repository, pypddl.TypeData("ball", [object_t]))
    gripper_t = build(repository, pypddl.TypeData("gripper", [object_t]))

    x = build(repository, pypddl.ParameterData(build(repository, pypddl.VariableData("x")), [room_t]))
    y = build(repository, pypddl.ParameterData(build(repository, pypddl.VariableData("y")), [room_t]))
    b = build(repository, pypddl.ParameterData(build(repository, pypddl.VariableData("b")), [ball_t]))
    g = build(repository, pypddl.ParameterData(build(repository, pypddl.VariableData("g")), [gripper_t]))

    at_robby = build(repository, pypddl.PredicateData("at-robby", [x]))
    at = build(repository, pypddl.PredicateData("at", [b, x]))
    free = build(repository, pypddl.PredicateData("free", [g]))
    carry = build(repository, pypddl.PredicateData("carry", [b, g]))

    x_term = build(repository, pypddl.TermData(x.get_variable()))
    y_term = build(repository, pypddl.TermData(y.get_variable()))
    b_term = build(repository, pypddl.TermData(b.get_variable()))
    g_term = build(repository, pypddl.TermData(g.get_variable()))

    move_pre = make_condition(repository, at_robby, [x_term])
    move_eff = make_effect_and(repository, [
        make_effect(repository, make_literal(repository, at_robby, [x_term], False)),
        make_effect(repository, make_literal(repository, at_robby, [y_term])),
    ])
    move = build(repository, pypddl.ActionData("move", [x, y], move_pre, move_eff))

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
    pick = build(repository, pypddl.ActionData("pick", [b, x, g], pick_pre, pick_eff))

    drop_pre = make_condition_and(repository, [
        make_condition(repository, carry, [b_term, g_term]),
        make_condition(repository, at_robby, [x_term]),
    ])
    drop_eff = make_effect_and(repository, [
        make_effect(repository, make_literal(repository, carry, [b_term, g_term], False)),
        make_effect(repository, make_literal(repository, free, [g_term])),
        make_effect(repository, make_literal(repository, at, [b_term, x_term])),
    ])
    drop = build(repository, pypddl.ActionData("drop", [b, x, g], drop_pre, drop_eff))

    domain = build(
        repository,
        pypddl.DomainData(
            "gripper",
            requirements=[strips, typing],
            types=[object_t, room_t, ball_t, gripper_t],
            predicates=[at_robby, at, free, carry],
            actions=[move, pick, drop],
        ),
    )

    def task(name, balls):
        rooma = build(repository, pypddl.ObjectData("rooma", [room_t]))
        roomb = build(repository, pypddl.ObjectData("roomb", [room_t]))
        left = build(repository, pypddl.ObjectData("left", [gripper_t]))
        right = build(repository, pypddl.ObjectData("right", [gripper_t]))
        ball_objects = [build(repository, pypddl.ObjectData(ball, [ball_t])) for ball in balls]

        rooma_term = build(repository, pypddl.TermData(rooma))
        roomb_term = build(repository, pypddl.TermData(roomb))
        left_term = build(repository, pypddl.TermData(left))
        right_term = build(repository, pypddl.TermData(right))

        initial_literals = [
            make_literal(repository, at_robby, [rooma_term]),
            make_literal(repository, free, [left_term]),
            make_literal(repository, free, [right_term]),
        ]
        goals = []
        for ball in ball_objects:
            ball_term = build(repository, pypddl.TermData(ball))
            initial_literals.append(make_literal(repository, at, [ball_term, rooma_term]))
            goals.append(make_condition(repository, at, [ball_term, roomb_term]))

        return build(
            repository,
            pypddl.TaskData(
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
    print(f"domain: {domain.get_name()}, actions={len(domain.get_actions())}, predicates={len(domain.get_predicates())}")
    print(f"task: {one_ball.get_name()}, objects={len(one_ball.get_objects())}, init={len(one_ball.get_initial_literals())}")
    print(f"task: {two_ball.get_name()}, objects={len(two_ball.get_objects())}, init={len(two_ball.get_initial_literals())}")
