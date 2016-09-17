;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname lab2) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
;; Jaime Arana-Rochel
;; aranarochel
;; CS 15100 2012 Lab 2

(require 2htdp/image)
(require racket/match)

;; a 3-dimensional vector is a (make-vec3 x y z) for x, y, z numbers
(define-struct vec3 (x y z))

;; rgb is a (make-rgb r g b)
;; for r (red) a num, g (green) a num, and b (blue) a num
;; each component is on the interval [0,1]
(define-struct rgb (r g b))


;; vec3+ : vec3 vec3 -> vec3
;; adds two vectors, given two vectors
(define (vec3+ v1 v2)
  (make-vec3
   (+ (vec3-x v1) (vec3-x v2))
   (+ (vec3-y v1) (vec3-y v2))
   (+ (vec3-z v1) (vec3-z v2))))
(check-expect (vec3+ (make-vec3 1 1 1) (make-vec3 1 1 1)) (make-vec3 2 2 2))
(check-expect (vec3+ (make-vec3 2 3 4) (make-vec3 5 6 7)) (make-vec3 7 9 11))


;; vec3- : vec3 vec3 : vec3
;; subtracts two vectors, given two vectors
(define (vec3- v1 v2)
  (make-vec3
   (- (vec3-x v1) (vec3-x v2))
   (- (vec3-y v1) (vec3-y v2))
   (- (vec3-z v1) (vec3-z v2))))
(check-expect (vec3- (make-vec3 1 1 1) (make-vec3 1 1 1)) (make-vec3 0 0 0))
(check-expect (vec3- (make-vec3 2 3 4) (make-vec3 5 6 7)) (make-vec3 -3 -3 -3))


;; vec3-neg : vec3 -> vec3
;; negates a vector, in effect producing a vector with no direction or magnitude
;; given a vector
(define (vec3-neg v)
  (make-vec3
   (- (vec3-x v) (vec3-x v))
   (- (vec3-y v) (vec3-y v ))
   (- (vec3-z v) (vec3-z v))))
(check-expect (vec3-neg (make-vec3 -1 1 1)) (make-vec3 0 0 0))
(check-expect (vec3-neg (make-vec3 6 10 -90)) (make-vec3 0 0 0))


;; vec3-scale : num vec3 -> vec3
;; scales (multiplies) a vector by a given scalar
(define (vec3-scale n v)
  (make-vec3
   (* n (vec3-x v))
   (* n (vec3-y v))
   (* n (vec3-z v))))
(check-expect (vec3-scale 1 (make-vec3 2 2 2)) (make-vec3 2 2 2))
(check-expect (vec3-scale 2 (make-vec3 1 2 3)) (make-vec3 2 4 6))


;; vec3-dot : vec3 vec3 -> num
;; produces a scalar by performing a dot product on two vectors
(define (vec3-dot v1 v2)
  (+
   (* (vec3-x v1) (vec3-x v2))
   (* (vec3-y v1) (vec3-y v2))
   (* (vec3-z v1) (vec3-z v2))))
(check-expect (vec3-dot (make-vec3 1 1 1) (make-vec3 1 1 1)) 3)
(check-expect (vec3-dot (make-vec3 0 0 0) (make-vec3 1 1 1)) 0)


;; vec3-mag : vec3 -> num
;; Computes the magnitude (lenght) of a given vector
(define (vec3-mag v)
  (sqrt
   (+ (sqr (vec3-x v))
      (sqr (vec3-y v))
      (sqr (vec3-z v)))))
(check-expect (vec3-mag (make-vec3 0 0 0)) 0)
(check-expect (vec3-mag (make-vec3 0 3 4)) 5)


;; vec3-unit? : vec3 -> bool
;; tests if a given vector is a unit vector (has magnitude 1)
(define (vec3-unit? v)
  (= (sqrt
   (+ (sqr (vec3-x v))
      (sqr (vec3-y v))
      (sqr (vec3-z v))))
     1))
(check-expect (vec3-unit? (make-vec3 1 1 1)) false)
(check-expect (vec3-unit? (make-vec3 1 0 0)) true)


;; vec3-zero? : vec3 -> bool
;; tests if a given vector has magnitude zero
(define (vec3-zero? v)
  (= (sqrt
   (+ (sqr (vec3-x v))
      (sqr (vec3-y v))
      (sqr (vec3-z v))))
     0))
(check-expect (vec3-zero? (make-vec3 1 1 1)) false)
(check-expect (vec3-zero? (make-vec3 0 0 0)) true)


;; vec3-norm : vec3 -> vec3
;; Normalizes the vector i.e computes a unit vector pointing in the same
;; direction as the given vector
(define (vec3-norm v)
  (make-vec3
   (/ (vec3-x v) (vec3-mag v))
   (/ (vec3-y v) (vec3-mag v))
   (/ (vec3-z v) (vec3-mag v))))
(check-expect (vec3-norm (make-vec3 4 4 2)) (make-vec3 4/6 4/6 2/6))
(check-expect (vec3-norm (make-vec3 0 3 4)) (make-vec3 0 3/5 4/5))


;; color->rgb : color -> rgb
;; converts color values into rgb values
(define (color->rgb c)
  (make-rgb
   (/ (color-red c) 255)
   (/ (color-green c) 255)
   (/ (color-blue c) 255)))
(check-expect (color->rgb (make-color 255 255 255)) (make-rgb 1 1 1))
(check-expect (color->rgb (make-color 0 0 0)) (make-rgb 0 0 0))


;; rgb->color : rgb -> color
;; converts rgb values into color values
(define (rgb->color c)
  (make-color
   (* 255 (rgb-r c))
   (* 255 (rgb-g c))
   (* 255 (rgb-b c))))
(check-expect (rgb->color (make-rgb 0 0 0)) (make-color 0 0 0))
(check-expect (rgb->color (make-rgb 1 1 1)) (make-color 255 255 255))


;; rgb-modulate : rgb rgb -> rgb
;; pointwise multiplies two given rgb's
(define (rgb-modulate c1 c2)
  (make-rgb 
   (* (rgb-r c1) (rgb-r c2))
   (* (rgb-g c1) (rgb-g c2))
   (* (rgb-b c1) (rgb-b c2))))
(check-expect (rgb-modulate (make-rgb 0 0 0) (make-rgb 1 1 1)) (make-rgb 0 0 0))
(check-expect (rgb-modulate (make-rgb .5 .5 .5) (make-rgb 1 1 1)) (make-rgb .5 .5 .5))


;; num-clamp : num -> num
;; Limits a given number on the interval [0,1]
(define (num-clamp n)
  (cond
    [(< n 0) 0]
    [(> n 1) 1]
    [else n]))
(check-expect (num-clamp -1) 0)
(check-expect (num-clamp 2) 1)
(check-expect (num-clamp .5) .5)


;; rgb-scale : num rgb -> rgb
;; scales each component by a given scalar
(define (rgb-scale n c)
 (make-rgb
  (num-clamp (* n (rgb-r c)))
  (num-clamp (* n (rgb-g c)))
  (num-clamp (* n (rgb-b c)))))
(check-expect (rgb-scale 2 (make-rgb 1 1 1)) (make-rgb 1 1 1))
(check-expect (rgb-scale -2 (make-rgb 1 1 1)) (make-rgb 0 0 0))
(check-expect (rgb-scale .5 (make-rgb 1 1 1)) (make-rgb .5 .5 .5))

;; rgb+ : rgb rgb -> rgb
;; adds two given rgb values
(define (rgb+ c1 c2)
  (make-rgb
   (num-clamp (+ (rgb-r c1) (rgb-r c2)))
   (num-clamp (+ (rgb-g c1) (rgb-g c2)))
   (num-clamp (+ (rgb-b c1) (rgb-b c2)))))
(check-expect (rgb+ (make-rgb 1 1 1) (make-rgb 1 1 1)) (make-rgb 1 1 1))
(check-expect (rgb+ (make-rgb .1 .2 .3) (make-rgb .1 .2 .3)) (make-rgb .2 .4 .6))

