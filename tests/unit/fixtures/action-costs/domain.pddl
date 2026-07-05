(define (domain action-cost-domain)
  (:requirements :action-costs)
  (:predicates (p))
  (:functions (total-cost))
  (:action pay
    :parameters ()
    :effect (increase (total-cost) 1))
)
