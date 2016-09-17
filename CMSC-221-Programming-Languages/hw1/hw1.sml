(* CS221 Spring 2014 The University of Chicago *)
(* HW1: SML warm-up exercises *)

(* Jaime Arana-Rochel
   CNET ID: aranarochel *)

(* due Wed Apr 9 11:59pm in hw1/hw1.sml in your repository *)

(* === Preliminaries === *)

(* some useful type synonyms *)

type 'a pair = 'a * 'a
type 'a pred = 'a -> bool
type 'a cmp  = 'a * 'a -> bool

(* CHECK module: a lightweight testing infrastructure *)

signature CHECK = sig
  val expect   : ''a * ''a * string -> unit
  val expectBy : 'a cmp -> 'a * 'a * string -> unit
  val within   : real -> real * real * string -> unit
end

structure Check : CHECK = struct

  fun msg s m = "Check." ^ s ^ " failure: " ^ m

  fun expect (x : ''a, y : ''a, m : string) : unit = 
    if x=y then () else raise Fail (msg "expect" m)

  fun expectBy (eq : 'a cmp) (x : 'a, y : 'a, m : string) : unit =
    if eq(x,y) then () else raise Fail (msg "expectBy" m)

  fun within (eps : real) (x : real, y : real, m : string) : unit = 
    if abs(x-y)<=eps then () else raise Fail (msg "within" m)

end (* structure Check *)

(* === Homework Problems === *)

(* === Problem 1 === *)

datatype nat
  = Zero
  | Succ of nat

fun natToInt (n : nat) : int =
  (case n
     of Zero => 0
      | Succ x => 1 + natToInt x)    


val _ = Check.expect (natToInt Zero, 0, "natToInt:1")
val _ = Check.expect (natToInt (Succ Zero), 1, "natToInt:2")


fun intToNat (i : int) : nat =
  (case i
     of 0 => Zero
      | n => Succ(intToNat(n - 1)))


val _ = Check.expect (intToNat 0, Zero, "intToNat:1")
val _ = Check.expect (intToNat 1, Succ Zero, "intToNat:2")


fun natAdd (m : nat, n : nat) : nat = 
  (case (m, n)
     of (Zero, Zero) => Zero
      | (Succ n1, Zero) => Succ(natAdd(n1, Zero))
      | (Zero, Succ n2) => Succ(natAdd(Zero, n2))
      | (Succ n1, Succ n2) => Succ(Succ(natAdd(n1,n2))))

val _ = Check.expect (natAdd (Zero, Zero), Zero, "natAdd:1")
val _ = Check.expect (natAdd (Succ Zero, Zero), Succ Zero, "natAdd:2")
val _ = Check.expect (natAdd (Succ(Succ Zero),Succ Zero),
                             Succ(Succ(Succ Zero)), "natAdd:3")


fun natEq (m : nat, n : nat) : bool = (m = n)

val _ = Check.expect (natEq(Zero, Zero), true, "natEq:1")
val _ = Check.expect (natEq(Succ Zero, intToNat(12)), false, "natEq:2")
val _ = Check.expect (natEq(intToNat(57), natAdd(intToNat(20),intToNat(37))),
                     true, "natEq:3")


fun natGT (m : nat, n : nat) : bool = 
  if natToInt(m) > natToInt(n) then true else false

val _ = Check.expect (natGT (Zero, Succ Zero), false, "natGT:1")
val _ = Check.expect (natGT (intToNat 100, intToNat 99), true, "natGT:2")

						 
fun natToString (n : nat) : string = 
  (case n
     of Zero => "Zero"
      | Succ i => "Succ("^natToString(i)^")")

val _ = Check.expect (natToString Zero, "Zero", "natToString:1")
val _ = Check.expect (natToString (intToNat 3), "Succ(Succ(Succ(Zero)))", "natToString:2")


(* === Problem 2 === *)

datatype filesize 
  = B of int
  | KB of real
  | MB of real
  | GB of real
  | TB of real

(* Helper function to get the size of the file from the datatype *)
fun getSize (B i) = real(i)
  | getSize (KB r) = r
  | getSize (MB r) = r
  | getSize (GB r) = r
  | getSize (TB r) = r

val _ = Check.within 0.001 (getSize(B 290), 290.0, "getSize:1")
val _ = Check.within 0.001 (getSize(TB 25.67), 25.67, "getSize:2")

fun toMB (s : filesize) : filesize = 
  (case s
     of B  i => MB (real(i)/1048576.0)
      | KB r => MB (r/1024.0)
      | GB r => MB (r*1024.0)
      | TB r => MB (r*1048576.0)
      | MB _ => s) 

val _ = Check.within 0.001 (getSize(toMB(B 1409267)), 1.3439, "toMB:1")
val _ = Check.within 0.001 (getSize(toMB(KB 49822.55)), 48.655,"toMB:2")
val _ = Check.within 0.001 (getSize(toMB(TB 29.4)),30828134.399,"toMB:3") 


(* === Problem 3 === *)

fun copies (x : 'a, n : int) : 'a list = 
  if n = 0 then [] else x::copies(x,n-1) 

val _ = Check.expect (copies(2,0), nil, "copies:1")
val _ = Check.expect (copies((1,"foo"),2), [(1,"foo"),(1,"foo")],"copies:2")
val _ = Check.expect (copies(#"t",3),[#"t",#"t",#"t"],"copies:3")


fun weave (xs : 'a list, ys : 'a list) : 'a list =
  (case (xs, ys)
     of (nil,nil) => []
      | (x::tl,nil) => []
      | (nil,y::tl) => []
      | (x::xt,y::yt) => x::y::weave(xt,yt))	     

val _ = Check.expect (weave([1,6],[]), [],"weave:1")
val _ = Check.expect (weave([1,2],[10]),[1,10],"weave:2")
val _ = Check.expect (weave(["the","amazingly"],["test","passed"]),["the","test","amazingly","passed"],"weave:3")

(* weave should weave together xs and ys.
 * When one or the other list runs out, it should stop weaving.
 * ex: weave ([1,2],[10,20]) => [1,10,2,20]
 * ex: weave ([1],[10,20]) => [1,10]
 * ex: weave ([1,2],[10])  => [1,10]
 *)

fun number (base : int, xs : 'a list) : (int * 'a) list =
  (case xs
     of nil => []
      | x::xt => (base,x)::number(base+1,xt))       

val _ = Check.expect (number(0,["a","b"]), [(0,"a"),(1,"b")], "number:1")
val _ = Check.expect (number(300,[]), [], "number:2")
val _ = Check.expect (number(1024,[[1,1],[2,2]]), [(1024,[1,1]),(1025,[2,2])], "number:3")

(* number must pair each item with an index, starting at base.
 * ex: number(0,["a","b"]) => [(0,"a"),(1,"b")]
 * ex: number(1,["a","b"]) => [(1,"a"),(2,"b")]
 *)

(* === Problem 4 === *)

fun isPrime n = let
  fun iP m =
    if n = m then true else if (n mod m = 0) then false 
     else if m*m >= n then true
     else iP (m+1)
  in 
    iP 2
  end 

val _ = Check.expect (isPrime 2, true, "isPrime:1")
val _ = Check.expect (isPrime 503, true, "isPrime:2")
val _ = Check.expect (isPrime 702, false, "isPrime:3")

fun isOdd n = if (n mod 2) = 1 then true else false

val _ = Check.expect (isOdd 5,true,"isOdd:1")
val _ = Check.expect (isOdd ~47,true,"isOdd:2")
val _ = Check.expect (isOdd 88,false,"isOdd:3")


infix \/
fun (p : 'a pred) \/ (q : 'a pred) : 'a pred = let
  fun disjun x = (p x) orelse (q x)
  in 
   disjun
  end

val _ = Check.expect ((isPrime \/ isOdd) 15, true, "disjun:1")
val _ = Check.expect ((isPrime \/ isOdd) 88, false, "disjun:2")


(* This is a "disjunctive composition" operator for tests.
 * Assuming you have tests isPrime and isOdd, then
 * the test (isPrime \/ isOdd) identifies primes and/or odds.
 *)

infix /\
fun (p : 'a pred) /\ (q : 'a pred) : 'a pred = let
  fun conjun x = (p x) andalso (q x)
  in 
   conjun
  end 
  
val _ = Check.expect ((isPrime /\ isOdd) 15, false, "conjun:1")
val _ = Check.expect ((isPrime /\ isOdd) 17, true, "conjun:2")
val _ = Check.expect ((isPrime /\ isOdd) 4, false, "conjun:3")
(* This is a "conjunctive composition" operator for tests.
 * Assuming you have tests isPrime and isOdd, then
 * the test (isPrime /\ isOdd) identifies odd primes.
 *)

(* === Problem 5 === *)

type ('a,''b) bin = ''b * 'a list

(* ==== Utils ==== *)
fun isPrimeCat n = let
  fun iP m =
    if n = m then "Prime" else if (n mod m = 0) then "not Prime" 
     else if m*m >= n then "Prime"
     else iP (m+1)
  in 
    iP 2
  end 

fun isOddCat n = if (n mod 2) = 1 then "Odd" else "Even"

(* Helper function for fun bins
 * places each item in its own separate bin *)
fun toBins (cat, xs) : ('a,''b) bin list =
  (case xs
     of nil => []
      | x::t => (cat(x),[x])::toBins(cat,t)) 

val _ = Check.expect (toBins(isOdd,[1,2,3]), [(true,[1]),(false,[2]),(true,[3])],"toBins:1")
val _ = Check.expect (toBins(isPrime,[4,15,17]), [(false,[4]),(false,[15]),(true,[17])],"toBins:2")


fun bins (cat : 'a -> ''b) (xs : 'a list) : ('a,''b) bin list = let
  fun insert (bin, bs) =
    (case (bin,bs)
       of (x,[]) => [x]
        | ((a,b),(h,t)::rest) => 
            if a=h 
            then (a,b@t)::rest 
            else (h,t)::insert(bin,rest))
  
  fun inBin [] = []
    | inBin [x] = [x]
    | inBin (h::t) = insert(h,inBin(t))
  in 
   inBin (toBins(cat,xs))
  end

val _ = Check.expect (bins isOddCat [1,2,3],[("Odd",[1,3]),("Even",[2])],"bins:1")
val _ = Check.expect (bins isPrimeCat [1,2,3,15,17],
                     [("Prime",[1,2,3,17]),("not Prime",[15])],"bins:2")
val _ = Check.expect (bins isOdd [3,10,23],[(true,[3,23]),(false,[10])],"bins:3")
	             
(* Given a categorizing function, put every item in a bin
 * with the rest of the items in the same category.
 * Look closely at the types.
 * The order of the bin and the order of the items in the
 *   list within each bin doesn't matter.
 *)

(* === Problem 6 === *)


fun commutes (oper : 'a pair -> ''b, pairs : 'a pair list) : 'a pair option =
  (case pairs
     of [] => NONE
      | (a,b)::t => if (oper(a,b)) <> (oper(b,a)) then SOME (a,b) else commutes(oper,t)) 

val _ = Check.expect (commutes ((op +),[(1,1),(2,3)]), NONE,"commutes:1")
val _ = Check.expect (commutes ((op -),[(1,1),(2,3)]), SOME (2,3),"commutes:2")
val _ = Check.expect (commutes ((op ^),[("Ice-Cream","Ice-Cream"),("hi","there"),("a","a")]),
                     SOME ("hi","there"),"commutes:3")


(* If the operator commutes on all pairs, return NONE.
 * If the operator does not commute on a pair, return it, 
 * wrapped in SOME, as a counterexample to commutativity. 
 * Return the first such pair that occurs in the list.
 *)

(* === Problem 7 === *)

(* Here is a mutually recursive datatype for trees
 * that alternate between having 2 and 3 children at
 * each level, and furthermore alternate between 
 * having 'a and 'b data at each level. 
 *)

datatype ('a, 'b) t2
  = E2
  | Nd2 of 'a * ('a, 'b) t3 * ('a, 'b) t3
and ('a, 'b) t3
  = E3
  | Nd3 of 'b * ('a, 'b) t2 * ('a, 'b) t2 * ('a, 'b) t2


(* Helpfer Function for Collect2 and Collect3 *)
fun lmerge (l1 : 'a list * 'b list, l2 : 'a list * 'b list) : 'a list * 'b list =
  (case (l1,l2)
     of (x,([],[])) => x
      | ((a,l),(b,t)) => (a @ b, l @ t))

(* Collect all the items from a t2/t3 tree into a
 * pair of lists. All items must appear in those lists,
 * and items that appear n times in the tree must appear
 * n times in the lists as well. The order of the items in
 * the lists in the result doesn't matter.
 *)

fun collect2 (t : ('a, 'b) t2) : 'a list * 'b list =  
  (case t
     of E2 => ([],[])
      | Nd2 (x,t1,t2) => lmerge( ([x],[]), lmerge(collect3 t1, collect3 t2)))
and collect3 (t : ('a, 'b) t3) : 'a list * 'b list = 
  (case t
     of E3 => ([],[])
      | Nd3 (x,t1,t2,t3) => lmerge( ([],[x]), lmerge(collect2 t1, lmerge(collect2 t2,collect2 t3)))) 


val _ = Check.expect (collect3 (Nd3 ("tree",E2,E2,E2)), ([], ["tree"]), "collect3:1")
val _ = Check.expect (collect2 (Nd2 (1,E3, Nd3 (true,E2,E2,E2))), ([1],[true]),"collect2:1")
val _ = Check.expect (collect3 (Nd3 (true,Nd2([1,3,5],Nd3(false,E2,E2,E2),E3),E2,Nd2([7],E3,E3))),
                     ([[1,3,5],[7]],[true,false]),"collect3:2")
                     
