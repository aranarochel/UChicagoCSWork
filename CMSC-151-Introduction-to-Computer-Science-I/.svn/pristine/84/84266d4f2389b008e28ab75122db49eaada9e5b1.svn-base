;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname lab1) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
;; Jaime Arana-Rochel
;; aranarochel
;; CS151 2012 Lab 1

(require 2htdp/image)

;; --- John Hancock Center ---

;; 344 m tall
;; with antennas, 457 m
;; thus antennas are 113 m

;; JHC dimensions from http://en.wikipedia.org/wiki/John_Hancock_Center

;; In the following named constants, the scale is 1 meter per pixel.

;; jhc-mid-h : number
;; middle part height
(define jhc-mid-h 339) 

;; jhc-mid-w : number
;; middle part width
(define jhc-mid-w 44)  

;; jhc-tri-w : number
;; triangle width
(define jhc-tri-w 12)

;; jhc-top-h : number
;; topper height
(define jhc-top-h 5)

;; jhc-top-w : number
;; topper width
(define jhc-top-w 37)

;; jhc-ant-h
;; antenna height
(define jhc-ant-h 113)

;; jhc-middle : img
(define jhc-middle
  (rectangle jhc-mid-w jhc-mid-h "solid" "black"))

;; jhc-rside : img
(define jhc-rside
  (right-triangle jhc-tri-w jhc-mid-h "solid" "black"))

;; jhc-lside : img
(define jhc-lside
  (flip-horizontal jhc-rside))

;; jhc-topper : img
(define jhc-topper
  (rectangle jhc-top-w jhc-top-h "solid" "black"))

;; jhc-antenna : img
(define jhc-antenna
  (rectangle 1 jhc-ant-h "outline" "gray"))



;; Part 1 of Lab 1


;; jhc-toppantenn : img
(define jhc-toppantenn 
(overlay/align "middle" "bottom" 
               (above/align "right" jhc-antenna jhc-topper) 
               (above/align "left" jhc-antenna jhc-topper)))


;; jhc-body : img
(define jhc-body
  (beside jhc-lside jhc-middle jhc-rside))



;; jhc
(define jhc
  (above jhc-toppantenn jhc-body))


;; Part 2 of Lab 1


;; -- Smurfit-Stone Building--

;; 177 m tall

;; Smurfit-Stone Building dimensions from http://en.wikipedia.org/wiki/Crain_Communications_Building

;; ssb-lrectangle : img
(define ssb-lrectangle
  (rectangle 68 125 "solid" "black"))

;; ssb-srectangle : img
(define ssb-srectangle
  (rectangle 10 125 "solid" "gray"))

;; ssb-diamond : img
(define ssb-diamond
  (rhombus 60 70 "solid" "Dark Gray"))

;; ssb-body : img
(define ssb-body
  (overlay/align "middle" "middle" ssb-srectangle ssb-lrectangle))

;; building2
(define building2
  (overlay/offset ssb-diamond 0 62 ssb-body))






              




