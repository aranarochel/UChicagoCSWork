module RBMaps (Map, empty, get, insert, toList) where

import Debug
import List

type Color = R | B
type Map comparable v
   = E
   | T Color (Map comparable v) (comparable, v) (Map comparable v)

empty : Map comparable v
empty = E
  

get : comparable -> Map comparable v -> Maybe v
get cm t = case t of
  E -> Nothing
  T _ l (cm',v) r -> if
    | cm == cm'   -> Just v 
    | cm <  cm'    -> get cm l
    | otherwise -> get cm r

insert : comparable -> v -> Map comparable v -> Map comparable v
insert cm v t =
  let (T _ l y r) = ins cm v t in
    T B l y r

ins : comparable -> v -> Map comparable v -> Map comparable v
ins cm v t =
  case t of
    E -> T R E (cm,v) E
    T c l (cm',v') r -> if
      | cm == cm'    -> T c l (cm,v) r
      | cm <  cm'    -> balance c (ins cm v l) (cm',v') r
      | otherwise -> balance c l (cm',v') (ins cm v r)

balance : Color
       -> Map comparable v -> (comparable, v) -> Map comparable v
       -> Map comparable v
balance c l pair r =
  case (c, l, pair, r) of
    (B, T R (T R a x b) y c, z, d) -> T R (T B a x b) y (T B c z d)
    (B, T R a x (T R b y c), z, d) -> T R (T B a x b) y (T B c z d)
    (B, a, x, T R (T R b y c) z d) -> T R (T B a x b) y (T B c z d)
    (B, a, x, T R b y (T R c z d)) -> T R (T B a x b) y (T B c z d)
    _                              -> T c l pair r

toList : Map comparable v -> List (comparable, v)
toList m = case m of
    E              -> []
    T _ l (cm,v) r -> List.append [(cm,v)] (List.append (toList l) (toList r)) 
