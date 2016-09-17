module DrawHeap where

import Array as A
import List ((::))
import List
import Window
import Result (..)
import Signal ((<~),(~))
import Signal
import String
import Color
import Text as T
import Graphics.Element as E
import Graphics.Collage as C
import Graphics.Input (button, customButton)
import Graphics.Input.Field (field, defaultStyle, noContent, Content)


-- Heap implementation and functions --

type alias InternalHeap = A.Array Int
type Heap = WrapHeap (A.Array Int)

fromJust : Int -> Maybe Int -> Int
fromJust i mx = case mx of 
  Just x -> x
  Nothing -> i

justGet : Int -> A.Array Int -> Int
justGet i a = fromJust i (A.get i a)

pop : A.Array Int -> A.Array Int
pop a = A.slice 0 (A.length a - 1) a 

swap : Int -> Int -> A.Array Int -> A.Array Int
swap i j a =
  a |> A.set i (justGet j a) |> A.set j (justGet i a)

insert : Int -> Heap -> Heap
insert x (WrapHeap a) =
  let n  = A.length a
      a' = A.push x a in
    WrapHeap (bubbleUp n a')

bubbleUp : Int -> InternalHeap -> InternalHeap
bubbleUp i a = if
  | i == 0 -> a
  | otherwise ->
      let j   = (i-1) // 2
          ch  = justGet i a
          par = justGet j a in
      if par <= ch
      then a
      else a |> swap i j |> bubbleUp j

deleteMin : Heap -> Maybe Heap
deleteMin (WrapHeap a) =
  let n = A.length a in if
    | n == 0    -> Nothing
    | otherwise -> let x = justGet (n-1) a in
                     a |> pop
                       |> A.set 0 x
                       |> bubbleDown 0
                       |> WrapHeap
                       |> Just

bubbleDown : Int -> InternalHeap -> InternalHeap
bubbleDown i a =
  let n = A.length a in
  if | i >= n -> a
     | otherwise ->
         let (j, k) = (2*i + 1, 2*i + 2) in
         let i'  = if justGet j a < justGet i  a then j else i  in
         let i'' = if justGet k a < justGet i' a then k else i' in
         if | i == i''  -> a
            | otherwise -> a |> swap i i'' |> bubbleDown i''

empty : Heap
empty = WrapHeap A.empty




-- VISUALIZATION IMPLEMENTATION --


-- INITIALIZE VALUES AND DECLARE HELPER FUNCS --
type alias State = Heap 
initstate = insert 5 <| insert 3 <| insert 0 empty

type ButtonEvent = Add Int| Delete

buttonCh : Signal.Channel ButtonEvent
buttonCh = Signal.channel Delete

inputBox : Signal.Channel Content
inputBox = Signal.channel noContent

btnW = 200
btnH = 45



myButton evt s =
  let mkBtn c =
    C.collage btnW btnH [ 
        C.filled c (C.rect btnW btnH)
      , C.outlined lineStyle (C.rect btnW btnH)
      , strStyle s |> C.toForm
    ]
  in
  customButton evt
    (mkBtn Color.lightPurple)
    (mkBtn Color.lightGreen)
    (mkBtn Color.green)

hspace = E.spacer 30 30

strStyle : String -> E.Element
strStyle = T.fromString >> T.height 25 >> T.centered

lineStyle =
  let ls = C.defaultLine in
    { ls | color <- Color.darkBlue,
           width <- 5 }

{- Gets the value of a "Result Int".
   If the user types in a string, then
   0 will be added to the tree -}
stoi s = case (String.toInt s) of
    Ok i -> i
    _    -> 0

{- unwraps the Maybe heap 
   implementation ensures that smallest tree is of size 3-}
maybeHtoH h ini = case h of
    Nothing -> initstate
    Just hp -> let (WrapHeap a) = hp
               in 
                   if | (A.length a) == 2 -> ini
                      | otherwise -> hp

{- groups items in increasing order of 2
   ex. lgroup 1 [0,1,2,3,4,5,6] = [[0],[1,2],[3,4,5,6]] -}
lgroup i ls = case ls of
  [] -> []
  _  -> List.take i ls :: (lgroup (i*2) (List.drop i ls))

-- UPSTATE --

upstate event state = case event of
    Add x      -> insert x state
    Delete     -> maybeHtoH (deleteMin state) state



-- DRAWING --

mkListElem : Int -> E.Element 
mkListElem i = [ C.square 50 |> C.outlined (C.dashed Color.darkGreen) |> C.rotate (degrees 30)
               , T.asText i |> C.toForm
               ]
                   |> C.collage 50 50

drawList a = 
    let elemList = List.map mkListElem (A.toList a) 
    in
        List.foldr E.beside E.empty elemList


-- takes an index & array and produces a tree node with children branches
mkTreeElem : Int -> A.Array Int -> E.Element
mkTreeElem i a = case (A.get ((2*i)+1) a, A.get ((2*i)+2) a) of  -- case of (Lchild,Rchild)  
    (Nothing,Nothing) -> [ C.circle 20 |> C.filled Color.lightYellow 
                         , A.toList a |> List.head |> T.asText |> C.toForm
                         ] 
                         |> C.collage 80 80
    (Nothing, Just _) -> [ C.rect 3 40 |> C.filled Color.blue |> C.rotate (degrees 30) |> C.move (20, -30)
                         , C.circle 20 |> C.filled Color.lightYellow
                         , A.toList a |> List.head |> T.asText |> C.toForm
                         ]
                         |> C.collage 80 80
    (Just _, Nothing) -> [ C.circle 20 |> C.filled Color.lightYellow
                         , A.toList a |> List.head |> T.asText |> C.toForm
                         ]
                         |> C.collage 80 80
    (Just _, Just _)  -> [ C.circle 20 |> C.filled Color.lightYellow
                         , A.toList a |> List.head |> T.asText |> C.toForm 
                         ]
                         |> C.collage 80 80

{- takes an array and list of indices to the array
   to facilitate getting the children branches -}
drawTree a indices = 
    let
        dt ar id = case (A.toList ar) of
            []      -> []
            v::rest -> (mkTreeElem (List.head indices) ar) 
                        :: (dt (A.fromList rest) (List.tail id))
    in
        let elementPairs = lgroup 1 (dt a indices)
            elemList = List.map (\i -> List.foldr E.beside E.empty i) elementPairs
            elems = List.map (\e -> [e |> C.toForm |> C.moveX -30] |> C.collage 500 50) elemList
        in
            E.flow E.down elems
                          
                         

view (w,h) con (WrapHeap a) = 
       E.color Color.lightGrey
    <| E.above (E.container (w+60) 200 E.middle <| (drawTree a (A.toList (A.initialize (A.length a) identity))))
    <| E.above (E.container w 120 E.middle <| (drawList a))
    <| E.container w h E.midTop
    <| E.flow E.down 
    <| List.intersperse hspace
        [ field defaultStyle (Signal.send inputBox) "insert an Int!" con
        , myButton (Signal.send buttonCh (Add (stoi con.string))) "Add"
        , myButton (Signal.send buttonCh Delete) "Delete"
        ]


main = 
    view <~ Window.dimensions 
          ~ Signal.subscribe inputBox
          ~ Signal.foldp upstate initstate (Signal.subscribe buttonCh)