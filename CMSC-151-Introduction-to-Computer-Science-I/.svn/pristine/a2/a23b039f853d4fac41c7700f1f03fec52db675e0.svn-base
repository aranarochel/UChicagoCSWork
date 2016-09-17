;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname hw3) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
;; Jaime Arana-Rochel
;; aranarochel
;; CS151 Autumn 2012, Homework 3

(require 2htdp/image)
(require racket/match)

;; ===Data Definitions===


;; a doll is either a 
;; - (make-central), or
;; - (make-shell d) where d is a doll
(define-struct central ())
(define-struct shell (inner))

;; a (listof bool) is either
;; - (empty), or
;; - (cons b bs) for b bool, and bs (listof bool)

;; a (listof num) is either
;; - (empty), or
;; - (cons n ns) for n num, and ns (listof num)

;; a (listof string) is either
;; - (empty), or
;; - (cons s ss) for a string, and ss (listof string)

;; ===Operations===

;; Problem 1


;; build-doll : nat -> doll
;; builds a doll with n layers
;; (not including the innermost one)
(define (build-doll n)
  (cond
    [(zero? n) (make-central)]
    [(positive? n)
     (make-shell (build-doll (sub1 n)))]))
(check-expect (build-doll 0) (make-central))
(check-expect (build-doll 2) (make-shell (make-shell (make-central))))


;; Problem 2


;; xor-list : (listof bool) -> bool
;; exclusive-or for a list of booleans
;; where a list is true when exactly one element is true
(define (xor-list lob)
  (cond
    [(empty? lob) false]
    [(cons? lob)
     (not (boolean=? (first lob) (xor-list (rest lob))))]))
(check-expect (xor-list (list true)) true)
(check-expect (xor-list (list true true)) false)
(check-expect (xor-list (list true false false)) true)
(check-expect (xor-list (list )) false)


;; Problem 3


;; on-axis : symbol (listof num) -> (listof posn)
;; consumes a symbol (either 'x or'y) and a list of numbers
;; and returns a corresponding list of points on the specified axis
(define (on-axis s lon)
  (cond
    [(empty? lon) empty]
    [(cons? lon)
     (cond
       [(symbol=? s 'x)
        (cons (make-posn (first lon) 0) (on-axis s (rest lon)))]
       [(symbol=? s 'y)
        (cons (make-posn 0 (first lon)) (on-axis s (rest lon)))])]))
(check-expect (on-axis 'x (list 1 2)) (list (make-posn 1 0) (make-posn 2 0)))
(check-expect (on-axis 'y (list -1 3)) (list (make-posn 0 -1) (make-posn 0 3)))


;; Problem 4

;; draw-row1 : num num color color-> img
;; draws a row given a number of squares in a row
;; the lenght of each square, and the light and dark colors
;; starting with the dark color
(define (draw-row1 n1 n2 c1 c2)
  (cond
    [(zero? n1) (square 0 "solid" "white")]
    [(positive? n1)
     (cond
       [(= n1 1) (square n2 "solid" c2)]
       [else 
        (beside (square n2 "solid" c2) (square n2 "solid" c1) 
                (draw-row1 (- n1 2) n2 c1 c2))])]))
"eyeball tests"
(draw-row1 0 50 "white" "black")
(draw-row1 3 50 "white" "black")
(draw-row1 5 50 "white" "black")


;; draw-row2 : num num color color-> img
;; draws a row given a number of squares in a row
;; the lenght of each square, and the light and dark colors
;; starting with the light color
(define (draw-row2 n1 n2 c1 c2)
  (cond
    [(zero? n1) (square 0 "solid" "white")]
    [(positive? n1)
     (cond
       [(= n1 1) (square n2 "solid" c1)]
       [else 
        (beside (square n2 "solid" c1) (square n2 "solid" c2) 
                (draw-row2 (- n1 2) n2 c1 c2))])]))
"eyeball tests"
(draw-row2 0 50 "white" "black")
(draw-row2 3 50 "white" "black")
(draw-row2 5 50 "white" "black")


;; chessboard : num num color color -> image
;; creates a chessboard given the number of squares in a row
;; the side lenght of each square
;; the light color
;; and the dark color
(define (chessboard n1 n2 c1 c2)
  (cond
    [(= n1 0) (square 0 "solid" "white")]
    [(= n1 1) (overlay (draw-row2 n1 n2 c1 c2) (draw-row1 n1 n2 c1 c2))]
    [else 
     (overlay/align/offset 
      "left" "top" 
      (overlay/align "left" "top" 
                     (draw-row2 n1 n2 c1 c2) 
                     (rotate -90 (draw-row2 n1 n2 c1 c2))) 
      n2 n2 (chessboard (sub1 n1) n2 c1 c2))]))
"eyeball test"
(chessboard 0 50 "white" "black")
(chessboard 1 50 "white" "black")
(chessboard 2 50 "white" "black")
(chessboard 4 77 "yellow" "purple")


;; Problem 6

;; concat-with : string (listof string) -> string
;; concatenates all strings
;; with a given separator string
(define (concat-with s los)
  (cond
    [(empty? (rest los)) (first los)]
    [(cons? los)
     (string-append (string-append (first los) s) (concat-with s (rest los)))]))
(check-expect (concat-with "," (list "Crosby" "Stills" "Nash")) "Crosby,Stills,Nash")
(check-expect (concat-with "and" (list "1" "2" "3")) "1and2and3")
(check-expect (concat-with "<>" (list "Billy")) "Billy")



;; Problem 7

;; proper-divisors : num num -> (listof num)
;; calculates a list of the proper divisors of a given integer
(define (proper-divisors n k)
  (cond
    [(zero? n) empty]
    [(integer? n)
     (cond
       [(integer? (/ n k)) (cons k (proper-divisors n (add1 k)))]
       [else empty])]))
(check-expect (proper-divisors 6 1) (list 1 2 3))
(check-expect (proper-divisors 0 1) empty)






      
      
     
     

    
    
    
    
    


 