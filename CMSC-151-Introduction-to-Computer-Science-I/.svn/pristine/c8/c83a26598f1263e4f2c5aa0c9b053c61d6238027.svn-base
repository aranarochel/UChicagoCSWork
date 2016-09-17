;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname lab8) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
;; Jaime Arana-Rochel
;; aranarochel
;; CS151 Autumn 2012, Lab 8

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

;; A color-map is either 
;; - empty, or
;; - (make-cmap c i L R)
;;   for c color, i num, L R color-maps
(define-struct cmap (c i L R))

;; a (pair α β) is a (make-pair a b)
;; for a α, b β
(define-struct pair (a b))

;; a gif is a (make-gif indices index w)
;; for indices a (listof (pair num num)), index a (listof indexed-color), and w num
(define-struct gif (indices index w))


;; === Image Compression Part I ===

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


;; === Image Compression Part II ===


;; insert2 : indexed-color color-map -> color-map
;; inserts an indexed color into a color-map
(define (insert2 ic cm)
  (cond
    [(empty? cm) (make-cmap (indexed-color-c ic) (indexed-color-i ic)
                            empty empty)]
    [(cmap? cm)
     (cond
       [(color=? (indexed-color-c ic) (cmap-c cm)) cm]
       [(color<? (indexed-color-c ic) (cmap-c cm))
        (make-cmap 
         (cmap-c cm)
         (cmap-i cm)
         (insert2 ic (cmap-L cm))
         (cmap-R cm))]
       [else
        (make-cmap 
         (cmap-c cm)
         (cmap-i cm)
         (cmap-L cm)
         (insert2 ic (cmap-R cm)))])]))

(check-expect (insert2 (make-indexed-color 0 (make-color 0 0 0)) 
                       (make-cmap (make-color 1 1 1) 1 empty empty))
              (make-cmap (make-color 1 1 1) 1
                         (make-cmap (make-color 0 0 0) 0 empty empty)
                         empty))
(check-expect (insert2 (make-indexed-color 0 (make-color 0 0 0))
                       (make-cmap (make-color 0 0 0) 1 empty empty))
              (make-cmap (make-color 0 0 0) 1 empty empty))
(check-expect (insert2 (make-indexed-color 0 (make-color 2 0 0)) 
                       (make-cmap (make-color 1 1 1) 1 empty empty))
              (make-cmap (make-color 1 1 1) 1
                         empty
                         (make-cmap (make-color 2 0 0) 0 empty empty)))



;; table->cmap : (listof indexed-color) -> color-map
;; converts a list of indexed-colors into a color-map
(define (table->cmap ics)
  (cond
    [(empty? (rest ics)) 
     (insert2 (first ics) empty)]
    [else
     (foldr insert2 empty ics)]))

(check-expect (table->cmap 
               (list (make-indexed-color 0 (make-color 3 3 3))
                     (make-indexed-color 1 (make-color 1 1 1))
                     (make-indexed-color 2 (make-color 1 1 2))
                     (make-indexed-color 3 (make-color 1 3 3))))
              (make-cmap
               (make-color 1 3 3 255)
               3
               (make-cmap 
                (make-color 1 1 2 255) 2 
                (make-cmap (make-color 1 1 1 255) 1 empty empty) empty)
               (make-cmap (make-color 3 3 3 255) 0 empty empty)))
(check-expect (table->cmap (list (make-indexed-color 0 (make-color 3 3 3))))
              (make-cmap (make-color 3 3 3) 0 empty empty))


;; present? : color color-map -> bool
;; checks to see if a color is in a color map
(define (present? c cm)
  (cond
    [(empty? cm) false]
    [else
     (cond
       [(color=? c (cmap-c cm)) true]
       [(color<? c (cmap-c cm)) (present? c (cmap-L cm))]
       [else (present? c (cmap-R cm))])]))
(check-expect (present? (make-color 1 3 3)
                        (make-cmap 
                         (make-color 1 3 3 255) 1
                         (make-cmap 
                          (make-color 1 1 2 255) 2 
                          empty empty)
                         (make-cmap (make-color 3 3 3 255) 0 
                                    empty empty)))
              true)
(check-expect (present? (make-color 1 1 2)
                        (make-cmap 
                         (make-color 1 3 3 255) 1
                         (make-cmap 
                          (make-color 1 1 2 255) 2 
                          empty empty)
                         (make-cmap (make-color 3 3 3 255) 0 
                                    empty empty)))
              true)
(check-expect (present? (make-color 3 3 3)
                        (make-cmap 
                         (make-color 1 3 3 255) 1
                         (make-cmap 
                          (make-color 1 1 2 255) 2 
                          empty empty)
                         (make-cmap (make-color 3 3 3 255) 0 
                                    empty empty)))
              true)
(check-expect (present? (make-color 0 0 0) empty) false)



;; index-of-color : color-map -> (color-> num)
;; is a curried operator for color-maps
(define (index-of-color cm)
  (local {;; find-index : color-> num
          ;; retrieves color (c) index
          (define (find-index c)
            (cond
              [(present? c cm)
               (cond
                 [(color=? c (cmap-c cm)) (cmap-i cm)]
                 [(color<? c (cmap-c cm)) 
                  ((index-of-color (cmap-L cm)) c)]
                 [(not (color<? c (cmap-c cm))) 
                  ((index-of-color (cmap-R cm)) c)])]
              [else (error 'index-of-color "color not indexed")]))}
find-index))

(check-expect ((index-of-color (make-cmap 
                                (make-color 1 3 3 255) 1
                                (make-cmap 
                                 (make-color 1 1 2 255) 2 
                                 empty empty)
                                (make-cmap (make-color 3 3 3 255) 0 
                                           empty empty)))
               (make-color 1 3 3))
              1)
(check-expect ((index-of-color (make-cmap 
                                (make-color 1 3 3 255) 1
                                (make-cmap 
                                 (make-color 1 1 2 255) 2 
                                 empty empty)
                                (make-cmap (make-color 3 3 3 255) 0 
                                           empty empty)))
               (make-color 1 1 2))
              2)
(check-expect ((index-of-color (make-cmap 
                                (make-color 1 3 3 255) 1
                                (make-cmap 
                                 (make-color 1 1 2 255) 2 
                                 empty empty)
                                (make-cmap (make-color 3 3 3 255) 0 
                                           empty empty)))
               (make-color 3 3 3))
              0)
(check-error ((index-of-color (make-cmap 
                               (make-color 1 3 3 255) 3
                               (make-cmap 
                                (make-color 1 1 2 255) 2 
                                empty empty)
                               (make-cmap (make-color 3 3 3 255) 0 
                                          empty empty)))
              (make-color 1 3 4)))


;; index-list: color-map (listof color) -> (listof num)
;; converts a list of colors into a list of index values
(define (index-list cm cs)
  (cond
    [(and (empty? cm) (empty? cs)) empty]
    [(empty? cm) empty]
    [(empty? cs) empty]
    [else
     (map (lambda (c) ((index-of-color cm) c)) cs)]))

(check-expect (index-list 
               (make-cmap (make-color 1 3 3 255) 3
                          (make-cmap 
                           (make-color 1 1 2 255) 2 
                           empty empty)
                          (make-cmap (make-color 3 3 3 255) 0 
                                     empty empty))
               (list (make-color 1 3 3) 
                     (make-color 3 3 3)))
              (list 3 0))
(check-expect (index-list empty (list (make-color 0 0 0))) empty)
(check-expect (index-list empty empty) empty)
(check-expect (index-list (make-cmap (make-color 0 0 0) 0 empty empty)
                          empty)
              empty)


;; rle : (α α -> bool) (listof α) -> (listof (pair num α)
;; run length encoding



;; compress : image -> gif
;; compresses an image
;; (note: code is written as if rle is defined)
(define (compress i)
  (local {(define list-index-color
            (image->table i))
          (define rgb-map
            (table->cmap list-index-color))
          (define list-colors
            (image->color-list i))
          (define list-of-indices
            (index-list rgb-map list-colors))}
    (make-gif
     (rle = list-of-indices)
     list-index-color
     (image-width i))))
            
    





