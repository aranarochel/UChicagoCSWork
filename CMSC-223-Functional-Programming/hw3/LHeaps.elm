module LHeaps where

import List ((::))
import List

{-- Copied from LeftistHeaps.elm from class. DO NOT MODIFY. -------------}

type alias Rank = Int
type Heap = E | T Rank Int Heap Heap

rank : Heap -> Rank
rank h = case h of {E -> 0; T r _ _ _ -> r}

makeT : Int -> Heap -> Heap -> Heap
makeT x h1 h2 =
  let (l,r) = if | rank h1 >= rank h2 -> (h1, h2)
                 | otherwise          -> (h2, h1) in
  T (1 + rank r) x l r

merge : Heap -> Heap -> Heap
merge h1 h2 = case (h1, h2) of
  (_, E) -> h1
  (E, _) -> h2 
  (T _ x1 l1 r1, T _ x2 l2 r2) ->
    if | x1 <= x2  -> makeT x1 l1 (merge r1 h2)
       | otherwise -> makeT x2 l2 (merge h1 r2)

{------------------------------------------------------------------------}


insert : Int -> Heap -> Heap
insert i h = 
    case h 
      of E -> T 1 i E E
         _  -> let (T r v l r) = h
               in 
                   makeT (min i v) l (insert (max i v) r)
    
{- Instead of getting O(log n) time from merging with the right subtree in the old implemention,
   this direct insertion algorithm must, in the worst case, make n size recursive calls to insert
   in the right position, resulting in O(n) time complexity
-}

fromList : List Int -> Heap
fromList ls = List.head <| makePass <| List.map (\h -> T 1 h E E) ls
  

mergePairs : List Heap -> List Heap
mergePairs hs = case hs of
  [] -> []
  [h] -> [h]
  h1::h2::rest -> merge h1 h2 :: mergePairs rest


makePass : List Heap -> List Heap
makePass hs = case (mergePairs hs) of
  [] -> []
  [h] -> [h]
  h::rest -> makePass (h::rest)
