"""Build a gripper domain and two tasks programmatically with pypddl."""

from collections.abc import Sequence

from pypddl import formalism as pypddl


def make_literal(
    repository: pypddl.Repository,
    predicate: pypddl.Predicate,
    terms: Sequence[pypddl.Term],
    positive: bool = True,
) -> pypddl.Literal:
    atom = repository.get_or_create(pypddl.AtomData(predicate, list(terms)))
    return repository.get_or_create(pypddl.LiteralData(atom, positive))


def make_condition(
    repository: pypddl.Repository,
    predicate: pypddl.Predicate,
    terms: Sequence[pypddl.Term],
    positive: bool = True,
) -> pypddl.Condition:
    literal = make_literal(repository, predicate, terms, positive)
    condition_literal = repository.get_or_create(pypddl.ConditionLiteralData(literal))
    return repository.get_or_create(pypddl.ConditionData(condition_literal))


def make_effect(repository: pypddl.Repository, literal: pypddl.Literal) -> pypddl.Effect:
    effect_literal = repository.get_or_create(pypddl.EffectLiteralData(literal))
    return repository.get_or_create(pypddl.EffectData(effect_literal))


def make_condition_and(
    repository: pypddl.Repository,
    conditions: Sequence[pypddl.Condition],
) -> pypddl.Condition:
    conjunction = repository.get_or_create(pypddl.ConditionAndData(list(conditions)))
    return repository.get_or_create(pypddl.ConditionData(conjunction))


def make_effect_and(
    repository: pypddl.Repository,
    effects: Sequence[pypddl.Effect],
) -> pypddl.Effect:
    conjunction = repository.get_or_create(pypddl.EffectAndData(list(effects)))
    return repository.get_or_create(pypddl.EffectData(conjunction))


def build_gripper() -> tuple[pypddl.Repository, pypddl.Domain, pypddl.Task, pypddl.Task]:
    repository = pypddl.RepositoryFactory().create()
    strips = repository.get_or_create(pypddl.RequirementData(pypddl.RequirementKind.Strips))
    typing = repository.get_or_create(pypddl.RequirementData(pypddl.RequirementKind.Typing))

    object_t = repository.get_or_create(pypddl.TypeData("object"))
    room_t = repository.get_or_create(pypddl.TypeData("room", [object_t]))
    ball_t = repository.get_or_create(pypddl.TypeData("ball", [object_t]))
    gripper_t = repository.get_or_create(pypddl.TypeData("gripper", [object_t]))

    x = repository.get_or_create(
        pypddl.ParameterData(repository.get_or_create(pypddl.VariableData("?x")), [room_t])
    )
    y = repository.get_or_create(
        pypddl.ParameterData(repository.get_or_create(pypddl.VariableData("?y")), [room_t])
    )
    b = repository.get_or_create(
        pypddl.ParameterData(repository.get_or_create(pypddl.VariableData("?b")), [ball_t])
    )
    g = repository.get_or_create(
        pypddl.ParameterData(repository.get_or_create(pypddl.VariableData("?g")), [gripper_t])
    )

    at_robby = repository.get_or_create(pypddl.PredicateData("at-robby", [x]))
    at = repository.get_or_create(pypddl.PredicateData("at", [b, x]))
    free = repository.get_or_create(pypddl.PredicateData("free", [g]))
    carry = repository.get_or_create(pypddl.PredicateData("carry", [b, g]))

    x_term = repository.get_or_create(pypddl.TermData(x.get_variable()))
    y_term = repository.get_or_create(pypddl.TermData(y.get_variable()))
    b_term = repository.get_or_create(pypddl.TermData(b.get_variable()))
    g_term = repository.get_or_create(pypddl.TermData(g.get_variable()))

    move_pre = make_condition(repository, at_robby, [x_term])
    move_eff = make_effect_and(repository, [
        make_effect(repository, make_literal(repository, at_robby, [x_term], False)),
        make_effect(repository, make_literal(repository, at_robby, [y_term])),
    ])
    move = repository.get_or_create(pypddl.ActionData("move", [x, y], move_pre, move_eff))

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
    pick = repository.get_or_create(pypddl.ActionData("pick", [b, x, g], pick_pre, pick_eff))

    drop_pre = make_condition_and(repository, [
        make_condition(repository, carry, [b_term, g_term]),
        make_condition(repository, at_robby, [x_term]),
    ])
    drop_eff = make_effect_and(repository, [
        make_effect(repository, make_literal(repository, carry, [b_term, g_term], False)),
        make_effect(repository, make_literal(repository, free, [g_term])),
        make_effect(repository, make_literal(repository, at, [b_term, x_term])),
    ])
    drop = repository.get_or_create(pypddl.ActionData("drop", [b, x, g], drop_pre, drop_eff))

    domain = repository.get_or_create(
        pypddl.DomainData(
            "gripper",
            requirements=[strips, typing],
            types=[object_t, room_t, ball_t, gripper_t],
            predicates=[at_robby, at, free, carry],
            actions=[move, pick, drop],
        ),
    )

    def task(name: str, balls: Sequence[str]) -> pypddl.Task:
        rooma = repository.get_or_create(pypddl.ObjectData("rooma", [room_t]))
        roomb = repository.get_or_create(pypddl.ObjectData("roomb", [room_t]))
        left = repository.get_or_create(pypddl.ObjectData("left", [gripper_t]))
        right = repository.get_or_create(pypddl.ObjectData("right", [gripper_t]))
        ball_objects = [repository.get_or_create(pypddl.ObjectData(ball, [ball_t])) for ball in balls]

        rooma_term = repository.get_or_create(pypddl.TermData(rooma))
        roomb_term = repository.get_or_create(pypddl.TermData(roomb))
        left_term = repository.get_or_create(pypddl.TermData(left))
        right_term = repository.get_or_create(pypddl.TermData(right))

        initial_literals = [
            make_literal(repository, at_robby, [rooma_term]),
            make_literal(repository, free, [left_term]),
            make_literal(repository, free, [right_term]),
        ]
        goals: list[pypddl.Condition] = []
        for ball in ball_objects:
            ball_term = repository.get_or_create(pypddl.TermData(ball))
            initial_literals.append(make_literal(repository, at, [ball_term, rooma_term]))
            goals.append(make_condition(repository, at, [ball_term, roomb_term]))

        return repository.get_or_create(
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
