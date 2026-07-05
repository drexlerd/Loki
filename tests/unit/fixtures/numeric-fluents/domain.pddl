(define (domain numeric-domain)
  (:requirements :numeric-fluents)
  (:predicates (p))
  (:functions (fuel))
  (:action burn
    :parameters ()
    :effect (decrease (fuel) 1))
)
