module RBTrees1 where

import RedBlackTrees (..)
import Debug

insert : Int -> Tree -> Tree
insert x t =
  let (T _ l y r) = ins x t in
    T B l y r

ins : Int -> Tree -> Tree
ins x t =
  case t of
    E -> T R E x E
    T c l y r -> if
      | x == y    -> t
      | x <  y    -> balanceL c (ins x l) y r
      | otherwise -> balanceR c l y (ins x r)

balanceL : Color -> Tree -> Int -> Tree -> Tree
balanceL c l val r =
  case (c, l, val, r) of
    (B, T R (T R a x b) y c, z, d) -> T R (T B a x b) y (T B c z d)
    (B, T R a x (T R b y c), z, d) -> T R (T B a x b) y (T B c z d)
    _                              -> T c l val r

balanceR : Color -> Tree -> Int -> Tree -> Tree
balanceR c l val r =
  case (c, l, val, r) of
    (B, a, x, T R (T R b y c) z d) -> T R (T B a x b) y (T B c z d)
    (B, a, x, T R b y (T R c z d)) -> T R (T B a x b) y (T B c z d)
    _                              -> T c l val r
