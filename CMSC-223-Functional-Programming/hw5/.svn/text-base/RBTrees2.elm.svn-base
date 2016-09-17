module RBTrees2 where

import RedBlackTrees (..)
import List ((::))
import List
import Debug

insert : Int -> Tree -> Tree
insert x t = 
    let (T _ l y r,ls) = ins x t
    in 
        T B l y r

type Dir = Left | Right

insH : Int -> Tree -> List Dir
insH x t = 
  case t of
    E -> []
    T c l y r -> if
      | x == y    -> [] 
      | x <  y    -> [Left] 
      | otherwise -> [Right]

ins : Int -> Tree -> (Tree, List Dir)
ins x t =
  case t of
    E         -> (T R E x E,[])
    T c l y r -> if  
      | x == y    -> (t,[])
      | x <  y    -> let lList = insH x l 
                         (tree,dirs) = (ins x l) 
                         tr = (chooseBalance (List.append [Left] lList)) c tree y r
                     in
                         (tr,[])
      | otherwise -> let rList = insH x r 
                         (tree,dirs) = (ins x r)
                         tr = (chooseBalance (List.append [Right] rList)) c l y tree
                     in
                         (tr,[])

  

type alias Balance = Color -> Tree -> Int -> Tree -> Tree

chooseBalance : List Dir -> Balance
chooseBalance ds = case ds of
    [Left,Left] -> balanceLL
    [Left,Right] -> balanceLR
    [Right,Left] -> balanceRL
    [Right,Right] -> balanceRR
    _ -> balanceLL
  

balanceLL : Balance
balanceLL c l val r =
  case (c, l, val, r) of
    (B, T R (T R a x b) y c, z, d) -> T R (T B a x b) y (T B c z d)
    _                              -> T c l val r

balanceLR : Balance
balanceLR c l val r =
  case (c, l, val, r) of
    (B, T R a x (T R b y c), z, d) -> T R (T B a x b) y (T B c z d)
    _                              -> T c l val r

balanceRL : Balance
balanceRL c l val r =
  case (c, l, val, r) of
    (B, a, x, T R (T R b y c) z d) -> T R (T B a x b) y (T B c z d)
    _                              -> T c l val r

balanceRR : Balance
balanceRR c l val r =
  case (c, l, val, r) of
    (B, a, x, T R b y (T R c z d)) -> T R (T B a x b) y (T B c z d)
    _                              -> T c l val r
