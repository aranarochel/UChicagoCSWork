;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname hw1) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
;; Jaime Arana-Rochel
;; aranarochel
;; CS151 2012 Homework 1

(require 2htdp/image)

;; Problem 1

;; f->k : num -> num
;; Converts degrees fahrenheit into degrees kelvin given degrees fahrenheit
(define (f->k f)
  (+ (* 5/9 (- f 32)) 273))

(check-expect (f->k 32) 273)
(check-expect (f->k 100) 2797/9)


;; k->f : num -> num
;; Converts degrees kelvin into degrees fahrenheit given deegres kelvin
(define (k->f k)
  (+ 32 (* 1.8 (- k 273))))

(check-expect (k->f 273) 32)


;; Problem 2

;; pythagorean? : num num num -> bool
;; Tells whether a set of three numbers is a pythagorean triple, 
;; given that the last number inputted is the largest
(define (pythagorean? a b c)
  (= (+ (* a a) (* b b)) (* c c)))

(check-expect (pythagorean? 3 4 5) true)
(check-expect (pythagorean? 5 12 13) true)



;; Problem 3

;; weekly-pay : num num -> num
;; Assuming a forty hour work week, computes the total amount of money
;; that a workers earns per week given a wage and overtime hours worked
(define (weekly-pay w h)
  (+ (* 40 w) (* 1.5 w h)))

(check-expect (weekly-pay 1 1) 41.5)
(check-expect (weekly-pay 12 4) 552)


;; Problem 4

;; cab-fare-with-tip : num -> num
;; computes the cab fare with tip included, 
;; rounding to the nearest dollar, given the cab fare
(define (cab-fare-with-tip f)
  (ceiling (+ f (* .15 f))))

(check-expect (cab-fare-with-tip 8) 10)
(check-expect (cab-fare-with-tip 16) 19)


;; Problem 5

;; d : num -> num
;; computes the number of digits in a number that is 
;; raised to itself, given said number
(define (d n)
  (inexact->exact (round (+ 1 (* n (/ (log n) (log 10)))))))
                           
(check-expect (d 1) 1)
(check-expect (d 8) 8)


;; Problem 6

;; outer-volume : num -> num
;; computes the volume of the outer sphere 
;; of the hemishell given its radius
(define (outer-volume r)
  (* 4/3 pi (expt r 3)))
(check-within (outer-volume 1) (* 4/3 pi) .000001)

;; inner-volume : num num -> num
;; computes the volume of the inner sphere of the hemishell
;; given the radius of the outer sphere and the thickness
;; of the hemishell
(define (inner-volume r t)
  (* 4/3 pi (expt (- r t) 3)))
(check-within (inner-volume 5 1) (* 256/3 pi) .000001)

;; outer-surface-area : num -> num
;; computes the outer surface area of the hemishell 
;; given the radius of the outer sphere
(define (outer-surface-area r)
  (* 4 pi (expt r 2)))
(check-within (outer-surface-area 2) (* 16 pi) .000001)

;; inner-surface-area : num num -> num
;; computes the inner surface area of the hemishell
;; given the radius of the outer sphere and thickness of the hemishell
(define (inner-surface-area r t)
  (* 4 pi (expt (- r t) 2)))
(check-within (inner-surface-area 5 2) (* 36 pi) .000001)

;; area-ring
;; computes the area of the ring between the outer and inner
;; surface areas given the radius of the outer sphere 
;; and the thickness of the hemishell
(define (area-ring r t)
  (- (* pi (* r r)) (* pi (expt (- r t) 2))))
(check-within (area-ring 6 4) (* 32 pi) .000001)


;; hemishell-volume : num num -> num
;; computes the volume of a hemishell given the radius of the outer sphere
;; and the thickness of the hemishell
(define (hemishell-volume r t)
  (/ (- (outer-volume r) (inner-volume r t)) 2))
(check-within (hemishell-volume 5 2) (* 196/3 pi) .000001)

;; hemishell-surface-area : num num -> num
;; computes the whole surface area of a hemishell given
;; the radius of the outer sphere and the thickness of the hemishell
(define (hemishell-surface-area r t)
  (+ (area-ring r t) (/ (+ (outer-surface-area r) (inner-surface-area r t)) 2)))
(check-within (hemishell-surface-area 3 1) (* 31 pi) .000001)
















