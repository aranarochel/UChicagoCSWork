module LazyListPlus where

import LazyList (..)
import Lazy (Lazy, lazy, force)

map : (a -> b) -> LazyList a -> LazyList b
map f xs = case (force xs) of
  Nil        -> lazy (\_ -> Nil)
  Cons x xs' -> lazy (\_ -> Cons (f x) (map f xs')) 


concat : LazyList (LazyList a) -> LazyList a
concat xss = case (force xss) of
  Nil          -> lazy (\_ -> Nil)
  Cons xs xss' -> append xs (concat xss') 

concatMap : (a -> LazyList b) -> LazyList a -> LazyList b
concatMap f xs =
  concat <| map f xs

cartProdWith : (a -> b -> c) -> LazyList a -> LazyList b -> LazyList c
cartProdWith f xs ys = 
  let
      prod xS yS = case (force xS, force yS) of
        (Nil, _)                 -> lazy (\_ -> Nil)
        (_, Nil)                 -> lazy (\_ -> Nil)
        (Cons x xs', Cons y ys') -> lazy (\_ -> Cons (x,y) (prod xs' ys'))

      cartProd = append (prod xs ys) (prod (reverse xs) ys)
  in 
      map (\(a,b) -> f a b) cartProd


