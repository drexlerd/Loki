"""Regenerate parser_counts_suite.json from all packaged benchmark problems.

Usage: uv run python tests/unit/parser/generate_parser_counts_suite.py
"""

import json
from pathlib import Path

from pypddl import formalism as pypddl
import pypddl_datasets

ROOT = Path(__file__).resolve().parents[3]
CONFIGURATIONS = [
    {"strict": strict, "add_action_costs": add_action_costs}
    for strict in (False, True)
    for add_action_costs in (False, True)
]


def discover_cases(prefix):
    cases = []
    for family in ("classical", "numeric"):
        family_root = prefix / family / "tests"
        for domain_file in sorted(family_root.rglob("domain.pddl")):
            task_files = sorted(
                path
                for path in domain_file.parent.glob("*.pddl")
                if path.name != "domain.pddl"
            )
            relative_dir = domain_file.parent.relative_to(family_root).as_posix()
            for task_file in task_files:
                name = f"{family}/{relative_dir}"
                if len(task_files) > 1:
                    name += f"/{task_file.stem}"
                cases.append(
                    {
                        "name": name,
                        "domain_file": domain_file.relative_to(prefix).as_posix(),
                        "task_file": task_file.relative_to(prefix).as_posix(),
                    }
                )
    assert cases and len({case["name"] for case in cases}) == len(cases)
    return cases


def make_options(config):
    options = pypddl.ParserOptions()
    for key, value in config.items():
        setattr(options, key, value)
    return options


def domain_counts(domain):
    return {
        "requirements": len(domain.get_requirements()),
        "types": len(domain.get_types()),
        "constants": len(domain.get_constants()),
        "predicates": len(domain.get_predicates()),
        "functions": len(domain.get_functions()),
        "actions": len(domain.get_actions()),
        "axioms": len(domain.get_axioms()),
    }


def task_counts(task):
    return {
        "requirements": len(task.get_requirements()),
        "objects": len(task.get_objects()),
        "initial_literals": len(task.get_initial_literals()),
        "initial_function_values": len(task.get_initial_function_values()),
        "predicates": len(task.get_predicates()),
        "axioms": len(task.get_axioms()),
        "has_goal": task.get_goal() is not None,
        "has_metric": task.get_metric() is not None,
    }


def main():
    prefix = Path(pypddl_datasets.data_root())
    cases = discover_cases(prefix)
    for case in cases:
        configurations = []
        for options_config in CONFIGURATIONS:
            parser = pypddl.Parser(
                prefix / case["domain_file"], make_options(options_config)
            )
            domain = parser.domain()
            task = parser.parse_task(prefix / case["task_file"])
            configurations.append(
                {
                    "parser_options": options_config,
                    "domain": domain_counts(domain),
                    "task": task_counts(task),
                }
            )
        case["configurations"] = configurations

    out = ROOT / "tests/unit/parser/parser_counts_suite.json"
    out.write_text(json.dumps({"cases": cases}, indent=4) + "\n")
    print(f"Wrote {len(cases)} cases x {len(CONFIGURATIONS)} configurations to {out}")


if __name__ == "__main__":
    main()
