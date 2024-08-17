;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 3 Op-blocks world
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (domain blocks3ops)
  (:requirements :strips :non-deterministic :equality :negative-preconditions)
  (:predicates (on ?x ?y)
               (ontable ?x)
               (clear ?x)
  )

  (:action newtower
    :parameters (?x ?y)
    :precondition (and (clear ?x) (on ?x ?y))
    :effect (and (ontable ?x) (clear ?y) (not (on ?x ?y)))
  )

  (:action stack
    :parameters (?x ?y)
    :precondition (and (not (= ?x ?y)) (clear ?x) (clear ?y) (ontable ?x))
    :effect (oneof
              (and (on ?x ?y) (not (ontable ?x)) (not (clear ?y)))
              (and))
  )

  (:action move
    :parameters (?x ?y ?z)
    :precondition (and (not (= ?x ?z)) (clear ?x) (clear ?z) (on ?x ?y))
    :effect (oneof
              (and (on ?x ?z) (not (clear ?z)) (clear ?y) (not (on ?x ?y)))
              (and (ontable ?x) (clear ?y) (not (on ?x ?y))))
  )
)
