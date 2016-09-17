module FP where

import List ((::))  -- TODO: modify imports if you'd like
import List
import Result (..)


-- helper function for digitsOfInt
doiHelper : Int -> List Int
doiHelper n = 
  case n
    of 0 -> []
       _ -> (n % 10) :: doiHelper (n//10)

digitsOfInt : Int -> List Int
digitsOfInt n = if | n < 0 -> []
                   | n == 0 -> [0]
                   | otherwise -> List.reverse (doiHelper n)
  

additivePersistence : Int -> Int
additivePersistence n =
  if | n < 10 -> 0
     | otherwise -> 1 + (additivePersistence <| List.sum <| digitsOfInt n)

digitalRoot : Int -> Int
digitalRoot n =
  if | n < 10 -> n
     | otherwise -> digitalRoot <| List.sum <| digitsOfInt n


-- helper function for subsequences
subHelper : a -> List a -> List (List a) -> List (List a)
subHelper h ls base = ls :: (h::ls) :: base

subsequences : List a -> List (List a)
subsequences xs =
  let
    sub ls = 
      case ls
        of [] -> []
           h::t -> [h] :: List.foldr (subHelper h) [] (sub t)
  in 
    [] :: sub xs


take : Int -> List a -> Result String (List a)
take k xs =
  if | k > List.length xs -> Err "not enough elements"
     | k < 0              -> Err "negative index"
     | otherwise          -> Ok (List.take k xs)
    

