module BHeaps
  (Heap, empty, isEmpty, findMin, insert, deleteMin, merge) where

import List ((::))
import List

type Tree = Node Int (List Tree)
type alias Rank = Int
type alias InternalHeap = List (Rank, Tree)
type Heap = WrapHeap InternalHeap

{-- Internal Helpers ----------------------------------------------------}
root (Node x _) = x


link : Tree -> Tree -> Tree
link t1 t2 = 
  let (Node x1 c1) = t1
      (Node x2 c2) = t2
  in 
    if | x1 <= x2  -> Node x1 (t2::c1)
       | otherwise -> Node x2 (t1::c2) 


insertTree : Int -> Tree -> InternalHeap -> InternalHeap
insertTree r t ts = case ts of
  []           -> [(r,t)]
  (r',t')::ts' -> if
    | r == r' -> insertTree (r+1) (link t t') ts'
    | r < r' -> (r,t)::ts   

merge_ : InternalHeap -> InternalHeap -> InternalHeap
merge_ ts1 ts2 = case (ts1, ts2) of
  ([], _) -> ts2
  (_, []) -> ts1
  ((r1,t1)::ts1',(r2,t2)::ts2') -> 
    if | r1 < r2 -> (r1,t1):: merge_ ts1' ts2
       | r1 > r2 -> (r2,t2):: merge_ ts2' ts1
       | otherwise -> insertTree (r1+1) (link t1 t2) (merge_ ts1' ts2')

removeMinTree : InternalHeap -> (Tree, InternalHeap)
removeMinTree ts = case ts of
  [(r,t)]    -> (t, [])
  (r,t)::ts' -> 
        let (t',ts'') = removeMinTree ts' in 
        if | root t < root t' -> (t, ts')
           | otherwise        -> (t', (r,t)::ts'') 

{-- External Interface --------------------------------------------------}

empty : Heap
empty = WrapHeap []

isEmpty : Heap -> Bool
isEmpty h = h == empty

insert : Int -> Heap -> Heap
insert x (WrapHeap ts) = WrapHeap (insertTree 0 (Node x []) ts)  
  

merge : Heap -> Heap -> Heap
merge (WrapHeap ts1) (WrapHeap ts2) = WrapHeap (merge_ ts1 ts2)
  

findMin : Heap -> Maybe Int
findMin (WrapHeap h) = case h of
  [] -> Nothing
  _ -> h |> List.map (\(r,t) -> root t) |> List.foldl1 min |> Just
  
deleteMin : Heap -> Maybe Heap
deleteMin (WrapHeap ts) = 
 {- case ts of
    [] -> Nothing
    _  -> let ((Node m c), ts2) = removeMinTree ts in 
          Just <| WrapHeap <| merge_ (List.reverse c) ts2
  -} 
  Nothing