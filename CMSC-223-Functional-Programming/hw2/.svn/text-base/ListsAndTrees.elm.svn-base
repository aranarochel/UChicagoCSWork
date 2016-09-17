module ListsAndTrees where

import List ((::))
import List


-- helper function 
isEven i = i % 2 == 0


suffixes xs =
  case xs
    of [] -> [[]]
       _::xs' -> xs :: suffixes xs'

{- Suffixes runs in O(n) time because in the scenario of a non empty list,
   the function has to traverse through the entire list of size n -}

{- Suffixes can be represented in O(n) because the function has to allocate
   n number of memory cells for the list as it recursively traverses xs. This
   is shown in the fact that the output list is of size n, the size of the
   original list -}  

type Tree = Empty | Node Int Tree Tree

mem : Int -> Tree -> Bool
mem i tree =
  let memHelp t tracker = 
    case t 
      of Empty -> case tracker
                    of [] -> False
                       [x] -> not (x < i)
         Node d l r -> if | i < d -> memHelp l tracker
                          | otherwise -> memHelp r [d]
  in
     memHelp tree [] 

fullTree : Int -> Int -> Tree
fullTree el height = 
  case height
    of 0 -> Empty
       _ -> Node el (fullTree el (height-1))
                    (fullTree el (height-1))

balancedTree : Int -> Int -> Tree
balancedTree el n =
  case n
    of 0 -> Empty
       _ -> let (_,btree) = create2 el (n-1)
            in
                btree 

create2 : Int -> Int -> (Tree, Tree)
create2 el m =
  case m 
    of 0 -> (Empty, Node el Empty Empty)
       _ ->  let (l,r) = create2 el ((m-1)//2)
             in
                if | isEven (m-1) -> (Node el l l, Node el l r)
                   | otherwise -> (Node el l r, Node el r r)
  

balancedTrees : Int -> Int -> List Tree
balancedTrees el size =
 if | size < 1 -> [Empty]
    | (size % 2) == 1 -> let strees = balancedTrees el (size//2)
                         in
                             List.concatMap (\l -> List.map (\r -> Node el l r) strees) strees 
    | (size % 2) == 0 -> let strees1 = balancedTrees el ((size-1)//2)
                             strees2 = balancedTrees el (((size-1)//2)+1)
                         in
                             List.concatMap (\l -> List.concatMap (\r -> [Node el l r
                                                                         ,Node el r l
                                                                         ]) strees2) strees1

completeTrees : Int -> Int -> List Tree
completeTrees _ _ =
  -- TODO
  []

almostCompleteTrees : Int -> Int -> List Tree
almostCompleteTrees _ _ =
  -- TODO
  []

