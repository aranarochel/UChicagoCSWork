module ExplicitMin
  (Heap, empty, isEmpty, findMin, insert, deleteMin, merge) where

-- NOTE: without functors or type classes, we would manually swap in
-- different implementations of H by twiddling the following imports

import BinomialHeaps as H
-- import LeftistHeaps as H

type Heap =
       E
     | NE Int H.Heap   -- the Int is the minimum element

empty : Heap
empty = E

isEmpty : Heap -> Bool
isEmpty h = h == empty
  

insert : Int -> Heap -> Heap
insert i h = case h of
  E      -> NE i (H.insert i H.empty)
  NE v hs -> NE (min v i) (H.insert i hs)  

merge : Heap -> Heap -> Heap
merge h1 h2 = case (h1, h2) of
  (E, h) -> h
  (h, E) -> h
  (NE i1 hs1, NE i2 hs2) -> NE (min i1 i2) (H.merge hs1 hs2)

findMin : Heap -> Maybe Int
findMin h = case h of
  E       -> Nothing
  NE i hs -> Just i 
  

deleteMin : Heap -> Maybe Heap
deleteMin h = case h of
  E       -> Nothing
  NE i hs -> let (Just hs') = H.deleteMin hs
                 (Just m)   = H.findMin hs'
             in
                 Just (NE m hs')

