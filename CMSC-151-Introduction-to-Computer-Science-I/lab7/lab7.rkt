;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname lab7) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
;; Jaime Arana-Rochel
;; aranarochel
;; CS151 Autumn 2012, Lab 7

(require 2htdp/image)
(require racket/match)


;; === Data Structures ===

;; a color-tree is either
;; - empty, or
;; - a (make-color-tree r L R)
;;   for r color, L R color-trees
(define-struct color-tree (r L R))

;; an indexed-color is a (make-indexed-color i c)
;; for i num, c color
(define-struct indexed-color (i c))


;; === Color Table ===

;; color=? : color color -> bool
;; determines whether two color are the same
(define (color=? c1 c2)
  (and (= (color-red c1) (color-red c2))
       (= (color-green c1) (color-green c2))
       (= (color-blue c1) (color-blue c2))))
(check-expect (color=? (make-color 1 1 1) (make-color 1 1 1)) true)
(check-expect (color=? (make-color 1 1 1) (make-color 1 1 0)) false)


;; color<? : color color -> bool
;; determines if a color is less
;; than another color
(define (color<? c1 c2)
  (cond
    [(< (color-red c1) (color-red c2)) true]
    [(> (color-red c1) (color-red c2)) false]
    [else
     (cond
       [(< (color-green c1) (color-green c2)) true]
       [(> (color-green c1) (color-green c2)) false]
       [else
        (cond
          [(< (color-blue c1) (color-blue c2)) true]
          [else false])])]))
(check-expect (color<? (make-color 1 1 1) 
                       (make-color 2 1 1)) true)
(check-expect (color<? (make-color 2 1 1)
                       (make-color 1 1 1)) false)
(check-expect (color<? (make-color 1 1 1)
                       (make-color 1 2 1)) true)
(check-expect (color<? (make-color 1 2 1)
                       (make-color 1 1 1)) false)
(check-expect (color<? (make-color 1 1 1)
                       (make-color 1 1 2)) true)
(check-expect (color<? (make-color 1 1 2)
                       (make-color 1 1 1)) false)


;; insert : color color-tree -> color-tree
;; inserts a color into a color-tree,
;; unless the color is already present
(define (insert c t)
  (cond
    [(empty? t) (make-color-tree c empty empty)]
    [(color-tree? t)
     (cond
       [(color=? c (color-tree-r t)) t]
       [(color<? c (color-tree-r t))
        (make-color-tree 
         (color-tree-r t)
         (insert c (color-tree-L t))
         (color-tree-R t))]
       [else
        (make-color-tree 
         (color-tree-r t)
         (color-tree-L t)
         (insert c (color-tree-R t)))])]))

(check-expect (insert (make-color 1 1 1) empty)
              (make-color-tree (make-color 1 1 1) empty empty))
(check-expect (insert (make-color 0 0 0) 
                      (make-color-tree
                       (make-color 0 0 0)
                       empty empty))
              (make-color-tree (make-color 0 0 0) empty empty))
(check-expect (insert (make-color 0 0 0) 
                      (make-color-tree
                       (make-color 1 1 1)
                       empty empty))
              (make-color-tree (make-color 1 1 1) 
                               (make-color-tree 
                                (make-color 0 0 0) empty empty)
                               empty))
(check-expect (insert (make-color 2 2 2) 
                      (make-color-tree
                       (make-color 1 1 1) empty empty))
              (make-color-tree 
               (make-color 1 1 1) 
               empty
               (make-color-tree (make-color 2 2 2) 
                                empty empty)))



;; image->tree : image -> color-tree
;; builds a binary search tree containing
;; the distinct colors in an image
(define (image->tree i)
  (local {(define (colorlist->tree cs)
            (cond
              [(empty? (rest cs)) 
               (insert (first cs) empty)]
              [else
               (foldr insert empty (reverse cs))]))}
    (if (equal? empty-image i) empty
        (colorlist->tree (image->color-list i)))))

(check-expect (image->tree (beside (square 1 "solid" (make-color 2 2 2))
                                   (square 1 "solid" (make-color 3 3 3))
                                   (square 1 "solid" (make-color 1 1 1))))
              (make-color-tree
               (make-color 2 2 2 255)
               (make-color-tree (make-color 1 1 1 255) empty empty)
               (make-color-tree (make-color 3 3 3 255) empty empty)))
(check-expect (image->tree (square 1 "solid" (make-color 1 1 1)))
              (make-color-tree 
               (make-color 1 1 1)
               empty empty))
(check-expect (image->tree empty-image) empty)


;; tree->table : color-tree -> (listof indexed-color)
;; builds a list of indexed-colors
;; given a color tree
(define (tree->table t)
  (local {(define (tree->list t)
            (cond
              [(empty? t) empty]
              [(color-tree? t)
               (append (list (color-tree-r t))
                       (tree->list (color-tree-L t))
                       (tree->list (color-tree-R t)))]))
          (define (list->table n l)
            (cond
              [(empty? l) empty]
              [else
               (cons (make-indexed-color n (first l))
                     (list->table (add1 n) (rest l)))]))}
    (list->table 0 (tree->list t))))

(check-expect (tree->table 
               (make-color-tree 
                (make-color 3 3 3)
                (make-color-tree (make-color 1 1 1) empty empty)
                (make-color-tree (make-color 4 4 4) empty empty)))
              (list
               (make-indexed-color 0 (make-color 3 3 3 255))
               (make-indexed-color 1 (make-color 1 1 1 255))
               (make-indexed-color 2 (make-color 4 4 4 255))))


;; image->table : image -> (listof indexed-color)
;; builds a color table from an image
(define (image->table i)
  (tree->table (image->tree i)))

(check-expect (image->table (square 1 "solid" "red"))
              (list (make-indexed-color 0 (make-color 255 0 0))))
(check-expect (image->table (beside (square 2 "solid" "red") 
                                    (square 2 "solid" "blue")))
              (list (make-indexed-color 0 (make-color 255 0 0 255)) 
                    (make-indexed-color 1 (make-color 0 0 255 255))))
    
    

;; n-distinct-colors : image -> num
;; counts the number of distinct colors in an image
(define (n-distinct-colors i)
 (length (image->table i))) 
(check-expect (n-distinct-colors (square 1 "solid" "red")) 1)
(check-expect (n-distinct-colors (beside
                                  (square 1 "solid" "red")
                                  (square 1 "solid" "blue"))) 2)
              
                                              







               
        