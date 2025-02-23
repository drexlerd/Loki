(define (problem delivery-x-1)
   (:domain delivery)
   (:objects rooma roomb - room
             item2 item1 - item
             bot1 - bot
             left1 - arm)
   (:init (= (weight item2) 1)
          (= (weight item1) 1)
          (at-bot bot1 rooma)
          (free left1)
          (mount left1 bot1)
          (at item2 rooma)
          (at item1 rooma)
          
          (door rooma roomb)
          (door roomb rooma)
          
          (= (current_load bot1) 0)
          (= (load_limit bot1) 1)
          (= (cost) 0))
          
   (:goal (and (at item2 roomb)
               (at item1 roomb)))
               
   (:metric minimize (cost))
)
