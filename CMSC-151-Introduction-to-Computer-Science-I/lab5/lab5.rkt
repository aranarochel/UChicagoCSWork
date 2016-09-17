;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname lab5) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
;; Jaime Arana-Rochel
;; aranarochel
;; CS151 Autumn 2012, Lab 5

(require 2htdp/image)
(require racket/match)

;; design : num -> img
;; draw square design, not symmetrical, with given side length
(define (design s)
  (local
    {(define r (rectangle (* s 3/5) (min 2 (/ s 10)) "solid" "darkblue"))
     (define block (above r (rectangle (* s 3/5) (/ s 5) "solid" "blue") r))}
    (overlay
     (square s "outline" "gray")
     (overlay/xy block
                 (/ s -3.5)
                 (/ s -6) 
                 (square s "solid" "dodgerblue")))))

"eyeball tests: design"
(build-list 6 (lambda (n) (design (expt 2 (+ n 2)))))


;; hor-sec : num -> img
;; build a horizontal section of the 
;; recursive-tiles function
(define (hor-sec n)
  (local {(define (r n)
            (beside (design n) 
                    (above (design (/ n 2)) (design (/ n 2)))))}
    (cond
      [(<= n 2) (design 2)]
      [(> n 2) (overlay 
                (beside/align "top" (r n) 
                              (above 
                               (hor-sec (/ n 4)) (hor-sec (/ n 4))))
                (beside/align "bottom" (r n) 
                              (above 
                               (hor-sec (/ n 4)) (hor-sec (/ n 4)))))])))
"eyeball test: hor-sec"
(hor-sec 128)
(hor-sec 0)


;; ver-sec : num -> img
;; build a vertical section of the 
;; recursive-tiles function
(define (ver-sec n)
  (local {(define (r n)
            (above (beside 
                    (design (/ n 2)) (design (/ n 2))) (design n)))}
    (cond
      [(<= n 2) (design 2)]
      [(> n 1) (overlay 
                (above/align "left" (beside 
                                     (ver-sec (/ n 4)) 
                                     (ver-sec (/ n 4))) 
                             (r n))
                (above/align "right" (beside 
                                      (ver-sec (/ n 4)) 
                                      (ver-sec (/ n 4))) 
                             (r n)))])))
"eyeball test: ver-sec"
(ver-sec 128)
(ver-sec 0)



;; quad : num -> img
;; builds the first quadrant of the 
;; recursive-tiles function
(define (quad n)
  (cond
    [(zero? n) empty-image]
    [(<= n 2) (design 2)]
    [(> n 2)
     (overlay/align/offset 
      "left" "bottom"
      (overlay/align "left" "bottom" (hor-sec n) (ver-sec n))
      n (- 0 n)
      (quad (/ n 2)))]))
"eyeball test: quad"
(quad 256)
(quad 0)


;; recursive-tiles : num -> img
;; produces a recursive tiling of the design function
(define (recursive-tiles n)
  (above 
   (beside (flip-horizontal (quad (/ n 4))) (quad (/ n 4)))
   (flip-vertical (beside (flip-horizontal (quad (/ n 4))) (quad (/ n 4))))))
"eyeball test: recursive-tiles"
(recursive-tiles 256)

  



                        





  