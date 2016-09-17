module RBTrees3 where

import RedBlackTrees (..)
import RBTrees2 (Balance, balanceLL, balanceLR, balanceRL, balanceRR)
import List ((::))
import List
import Debug

insert : Int -> Tree -> Tree
insert x t = 
    let (T _ l y r,_) = ins x t
    in 
        T B l y r


{- helper function that gives the possible balance functions based on path -}
{- insH will be used on the children nodes of a parent node. For example if the path goes left
   on the child then the possible balancing funcs are LL or RL. The parent node then
   chooses either the first or second of the pair based on which child path to go to 
   (this step is done in "ins") -}
insH : Int -> Tree -> (Balance, Balance)
insH x t = 
  case t of
    E -> (balanceLL,balanceRR)
    T c l y r -> if
      | x == y    -> (balanceLL,balanceRR) 
      | x <  y    -> (balanceLL,balanceRL) 
      | otherwise -> (balanceLR,balanceRR)




ins : Int -> Tree -> (Tree, (Balance, Balance))
ins x t =
  case t of
    E         -> (T R E x E,(balanceLL,balanceRR))
    T c l y r -> if  
      | x == y    -> (t,(balanceLL,balanceRR))
      | x <  y    -> let (bal1,_) = insH x l        
                         (tree,bals) = (ins x l)       
                         tr =  bal1 c tree y r
                     in
                         (tr,bals)
      | otherwise -> let (_,bal2) = insH x r 
                         (tree,bals) = (ins x r)
                         tr = bal2 c l y tree
                     in
                         (tr,bals)

