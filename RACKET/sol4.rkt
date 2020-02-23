#lang racket
(provide (all-defined-out))

(define (check_bst_helper bst min max)
  (if (null? bst)
      #t
      (let ([n (car bst)]
            [left (car (cdr bst))]
            [right (car (cdr (cdr bst)))])
        (if (or (if (null? min) #f (> min n)) (if (null? max) #f (< max n)))
            #f
            (and (check_bst_helper left min n) (check_bst_helper right n max))))))
                      
(define (check_bst bst)
  (if (list? bst)
      (check_bst_helper bst null null)
      #f))


(define (apply f bst)
  (if (null? bst)
      null
      (let ([n (car bst)]
            [left (car (cdr bst))]
            [right (car (cdr (cdr bst)))])
        (list (f n) (apply f left) (apply f right)))))

 
(define (elements bst)
  (if (null? bst)
      null
      (let ([n (car bst)]
            [left (car (cdr bst))]
            [right (car (cdr (cdr bst)))])
        (append (elements left) (list n) (elements right)))))

(define (equals bst1 bst2)
  (cond [(or (not (list? bst1)) (not (list? bst2))) #f]
        [(and (null? bst1) (null? bst2)) #t]
        [(or (null? bst1) (null? bst2)) #f]
        [(or (not (check_bst bst1)) (not (check_bst bst2))) #f]
        [#t (equal? (elements bst1) (elements bst2))]))