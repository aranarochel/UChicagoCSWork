module RBTreesDel (Set, empty, member, insert, delete, size) where

import RBMaps as Map
import List
import Debug

type Set comparable = Set (Map.Map comparable Bool)

empty : Set comparable
empty = Set Map.empty

member : comparable -> Set comparable -> Bool
member cm (Set m) = case (Map.get cm m) of
    Nothing -> False
    Just x -> x 

insert : comparable -> Set comparable -> Set comparable
insert cm (Set m) = Set (Map.insert cm True m) 
  

delete : comparable -> Set comparable -> Set comparable
delete cm (Set m) = case (member cm (Set m)) of
    False -> Set m
    True  -> Set (Map.insert cm False m)
  

size : Set comparable -> (Int, Int)
size (Set m) = 
    let addtup (cm,v) (x1,x2) = if | v == True -> (x1+1,x2)
                                   | otherwise -> (x1,x2+1)
    in 
        List.foldr addtup (0,0) (Map.toList m)

