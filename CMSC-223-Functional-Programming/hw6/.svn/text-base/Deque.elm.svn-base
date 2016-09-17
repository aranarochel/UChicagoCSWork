module Deque
  (Deque, empty, isEmpty,
   addFront, removeFront, peekFront,
   addBack, removeBack, peekBack) where

import List ((::))
import List

type Deque a = D { front : List a, back : List a }

mkD f b = D {front = f, back = b}

empty : Deque a
empty = mkD [] []

isEmpty : Deque a -> Bool
isEmpty q = q == empty

--------------------------------------------------------------------------------
-- FILL IN THE DEFINITIONS BELOW

check : List a -> List a -> Deque a
check f b = 
    let half ls = (List.length ls)//2
    in case (f, b) of
        ([] , _) -> mkD (List.reverse <| List.drop (half b) b) (List.take (half b) b)
        (_ , []) -> mkD (List.take (half f) f) (List.reverse <| List.drop (half f) f)
        (_ , _)  -> mkD f b   
  

addFront : a -> Deque a -> Deque a
addFront x (D {front, back}) =
  check (x::front) back

addBack : a -> Deque a -> Deque a
addBack x (D {front, back}) =
  check front (x::back)

peekFront : Deque a -> Maybe a
peekFront (D {front, back}) = case front of
  []   -> Nothing
  f::_ -> Just f  

peekBack : Deque a -> Maybe a
peekBack (D {front, back}) = case back of
  []   -> Nothing
  b::_ -> Just b

removeFront : Deque a -> Maybe (Deque a)
removeFront (D {front, back}) = case front of
  []    -> Nothing
  _::f' -> Just (check f' back) 

removeBack : Deque a -> Maybe (Deque a)
removeBack (D {front, back}) = case back of
  []    -> Nothing
  _::b' -> Just (check front b')

