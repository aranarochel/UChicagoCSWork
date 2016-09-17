module Pi where

import Basics(..)
import Color
import Time
import Text (asText)
import List ((::))
import List
import Random (generate,float,int)
import Random
import Signal
import Graphics.Element (Element,empty,flow,outward,image)
import Graphics.Collage (..)
import String
import Window
  -- TODO: modify/add imports as needed

type alias Point = { x:Float, y:Float }

type alias State = ((Int, List Point), (Int, List Point))

initState = ((0,[]), (0,[]))


-- UPDATE
-- function that checks if a point is in the quarter circle
isInCircle : Point -> Bool
isInCircle pt = (pt.x^2 + pt.y^2) < 1 || (pt.x^2 + pt.y^2) == 1

-- function to add a point to the state
addPoint (pt, st) =
  case (isInCircle pt, st) 
    of (True, ((i1,ps1),x)) -> ((i1+1, pt::ps1), x)
       (False, (x, (i1,ps1))) -> (x, (i1+1, pt::ps1))

upstate : Point -> State -> State
upstate pt st = addPoint (pt, st)
  

-- VIEW

pointsToCirclesAlpha : Float -> Float -> Color.Color -> List Point -> List Form
pointsToCirclesAlpha acol mul col points =
  let
      pTof p = let {x,y} = p
               in
                   alpha acol <| move (x*mul,y*mul) (filled col (ngon 5 5))
  in
      List.map pTof points

piApprox : State -> Float
piApprox st = 
  let
      ((hits,_),(misses,_)) = st
  in
      (toFloat hits / toFloat (hits+misses)) * 4.0




view : (Int,Int) -> State -> Element
view (w,h) st =
  let
      ((hits,hitList),(misses,missList)) = st
      myPi = piApprox st
      hitCircles  = pointsToCirclesAlpha 1.0 400 (Color.rgb 200 150 50) hitList
      missCircles = pointsToCirclesAlpha 1.0 400 (Color.rgb 20 90 100) missList
      hitCirclesShadow = pointsToCirclesAlpha 0.15 340 (Color.rgb 20 200 20) hitList
      missCirclesShadow = pointsToCirclesAlpha 0.15 340 (Color.rgb 50 100 150) missList
      numPi = [move (200,-100) (toForm <| asText myPi)]
      myImage = List.foldr List.append [] (hitCircles::missCircles::hitCirclesShadow::missCirclesShadow::numPi::[])
  in 
      collage w h myImage
        
        
    



-- CONTROLLER

genPoint : Random.Seed -> (Point, Random.Seed)
genPoint s =
  let (p1,sd1) = generate (float 0 1) s
      (p2,sd2) = generate (float 0 1) sd1
  in
      ({x=p1, y=p2}, sd2)
    
  -- ({x=0,y=0}, s)

signalPointSeed : Signal (Point, Random.Seed)
signalPointSeed =
  let iniState = ({x=0,y=0}, Random.initialSeed 20)
      ticker = Time.every (Time.millisecond)

      updateS _ st = let (p,s) = st
                     in  
                         genPoint s

  in
      Signal.foldp updateS iniState ticker
   

signalPoint : Signal Point
signalPoint =
  let 
      getP pointSeed = let (p,s) = pointSeed
                       in 
                           p
  in
      Signal.map getP signalPointSeed


main : Signal Element
main =
  Signal.map2 view Window.dimensions
    (Signal.foldp upstate initState signalPoint)
