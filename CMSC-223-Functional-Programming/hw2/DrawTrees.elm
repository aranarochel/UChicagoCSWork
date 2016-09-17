module DrawTrees where

import List ((::))
import List
import ListsAndTrees (..)

import Color
import Signal
import Window
import Mouse
import Text as T
import Graphics.Element as E
import Graphics.Collage as C


-- helper function to shift a list
-- a.k.a shiftList [1,2,3] = [2,3,1]
shiftList xs = List.append (List.tail xs) [List.head xs]


sampleListOn : Signal b -> List a -> Signal a
sampleListOn sig xs =
  Signal.map (\x -> List.head x)
             (Signal.foldp (\s values -> shiftList values) xs sig)


-- VIEW
-- might have to fiddle with width and height sizes in the "mkNode/mkEdgeR/mkEdgeL" functions
-- in case it doesn't show up on screen. Shows up fine on linux machines at Crerar


mkNode w h =  C.move (w-2500,h-1000) <| C.filled Color.blue <| (C.circle 5)

mkEdgeR w h = C.move (w-2500+30,h-1000-40) <| C.rotate (degrees 30) <| C.filled Color.red 
                                       <| (C.rect 3 80)

mkEdgeL w h = C.move (w-2500-30,h-1000-40) <| C.rotate (degrees -30) <| C.filled Color.red 
                                       <| (C.rect 3 80)

-- takes a tree and makes a list of forms composing that tree
treeF (w,h) t =  
  let func (p,q) tr accum =
        case tr
          of Empty -> accum
             Node x l r -> List.append (func (p+50,q-80) l accum) 
                           (List.append (func (p-50,q-80) r accum) 
                           (List.append [mkNode p q,mkEdgeL p q,mkEdgeR p q] accum))
              
  in 
    func (w,h) t []



view : (Int,Int) -> Tree -> E.Element
view (w,h) tree =
  E.container w h E.middle
    (C.collage w h (treeF (toFloat w,toFloat h) tree))
  
  

signalTree : Signal Tree
signalTree =
  sampleListOn Mouse.clicks (balancedTrees 0 4)

main : Signal E.Element
main =
  Signal.map2 view Window.dimensions signalTree