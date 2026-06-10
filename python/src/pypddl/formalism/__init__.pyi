from typing import Any as _Any

__all__: tuple[str, ...]

Parser: _Any
ParserOptions: _Any
TranslatorOptions: _Any
Repository: _Any
RepositoryFactory: _Any
DomainTranslationResult: _Any
ProblemTranslationResult: _Any
SemanticError: type[Exception]
ParseError: type[Exception]
MissingDomainError: type[Exception]
MismatchedDomainError: type[Exception]
UnsupportedRequirementError: type[Exception]
MissingRequirementError: type[Exception]
UndefinedTypeError: type[Exception]
UndefinedPredicateError: type[Exception]
UndefinedObjectError: type[Exception]
UndefinedVariableError: type[Exception]
UndefinedFunctionError: type[Exception]
DuplicateDefinitionError: type[Exception]
DuplicateTypeError: type[Exception]
DuplicatePredicateError: type[Exception]
DuplicateObjectError: type[Exception]
DuplicateVariableError: type[Exception]
DuplicateFunctionError: type[Exception]
ArityMismatchError: type[Exception]
TypeMismatchError: type[Exception]
InvalidMetricError: type[Exception]
InvalidNumericConstraintError: type[Exception]
InvalidNumericEffectError: type[Exception]
InvalidProbabilisticEffectError: type[Exception]
InvalidEqualityError: type[Exception]

def translate_domain(*args: _Any, **kwargs: _Any) -> _Any: ...
def translate_task(*args: _Any, **kwargs: _Any) -> _Any: ...
def format_domain(domain: _Any) -> str: ...
def format_task(task: _Any) -> str: ...
def __getattr__(name: str) -> _Any: ...
