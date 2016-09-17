structure InstrumentedList : INSTRUMENTED_LIST  = struct

  type 'a ord   = {lt : 'a * 'a -> bool}
  type 'a instr = {len : int, max : 'a, min : 'a}

  datatype 'a seq
    = Nil
    | Cons of 'a instr * ('a * 'a seq)

  datatype 'a ilist
    = List of 'a ord * 'a seq

  (* helpful utilities *)

  fun choose (cmp, a, b) = if cmp(a,b) then a else b
  fun choose2 (cmp, a, b)  = if cmp(a,b) then b else a
  fun choose3 (a, b) = if a = b then a else b

  fun singleton (x : 'a) : 'a seq = Cons ({len=1,max=x,min=x}, (x, Nil))

  fun join (xs,ys) =
    (case (xs,ys)
       of (_,List(_,Nil)) => xs
        | (List(_,Nil),_) => ys
        | (List(ord,Cons(ins,(n,tail))),List(ord2,seq)) 
	  => (List(ord,Cons(ins,(n,seq)))))

  (* fill in the operations below, and uncomment the signature ascription when done *)

  fun hd ls = 
    (case ls
       of List (_,Nil) => NONE
        | List (_,Cons(_,(x,_))) => SOME x) 

  fun tl ls = 
    (case ls 
       of List (_,Nil) => NONE
        | List (x,Cons(_,(_,tail))) => SOME (List(x,tail)))

  fun cons (x ,ls) =
    (case ls
       of List ({lt=ord},Nil) => List ({lt=ord},singleton x)
        | List ({lt=ord},Cons({len=n,max=mx,min=mn},rest)) => 
            List({lt=ord},
	     Cons({len=n+1,max=choose2(ord,mx,x),min=choose(ord,x,mn)},
              (x,Cons({len=n,max=mx,min=mn},rest))))) 
  
  fun length ls = 
    (case ls
       of List (_,Nil) => 0
        | List (_,Cons({len=n,max=mx,min=mn},_)) => n)

  fun max ls =
    (case ls
       of List (_,Nil) => NONE
        | List (_,Cons({len=n,max=mx,min=mn},_)) => SOME mx)

  fun min ls =
    (case ls
       of List (_,Nil) => NONE
        | List (_,Cons({len=n,max=mx,min=mn},_)) => SOME mn)

  fun map ({lt=f} : 'b ord) (func : 'a -> 'b) (ls : 'a ilist) : 'b ilist = 
    (case ls
       of List (_,Nil) => List({lt=f},Nil)
        | List ({lt=ord},Cons({len=n,max=mx,min=mn},(x,rest))) => 
            (case (tl ls)
               of NONE => List({lt=f},
                           Cons({len=n,
                                 max=choose2(f,func mx,func x),
                                 min=choose(f,func x, func mn)}, (func x,Nil)))
	        | SOME ilist => join(List({lt=f},
                                          Cons({len=n,
                                                max=choose2(f,func mx,func x),
                                                min=choose(f,func x, func mn)}, 
                                              (func x,Nil))),
                                     map {lt=f} func ilist)))

  fun filter (test : 'a -> bool) (ls : 'a ilist) : 'a ilist =
    (case ls
       of List (_, Nil) => ls
        | List (ord,Cons({len=n,max=mx,min=mn},(x,rest))) =>
            (case (tl ls)
               of NONE => if test(x) then ls else filter test (List(ord,Nil))
                | SOME ilist => if test(x)
                                then cons(x,filter test ilist)
                                else filter test ilist)) 

  
  fun rev (ls : 'a ilist) : 'a ilist =
    (case ls
       of List (_,Nil) => ls
        | List (ord,Cons({len=n,max=mx,min=mn},(x,rest))) =>
            (case (tl ls)
               of NONE => ls
                | SOME ilist => join(rev ilist, cons(x,List(ord,Nil)))))

  fun same (cmp : 'a * 'a -> bool) (ilist1: 'a ilist, ilist2) : bool =
    (case (ilist1,ilist2)
       of (List(_,Nil),List(_,Nil)) => true
        | (List(_,Nil),List(ord,x)) => false
        | (List(ord,x),List(_,Nil)) => false
        | (List(_,Cons({len=n,max=mx,min=mn},(x,rest))),List(_,Cons({len=n2,max=mx2,min=mn2},(x2,rest2)))) =>
            (case (tl ilist1,tl ilist2)
               of (NONE,NONE) => if (n=n2) andalso (cmp(mx,mx2)) andalso (cmp(mn,mn2)) andalso (cmp(x,x2))
                                 then true else false
                | (SOME lst,SOME lst2) => if (n=n2) andalso (cmp(mx,mx2)) andalso (cmp(mn,mn2)) andalso (cmp(x,x2))
                                          then same cmp (lst,lst2)
                                          else false))


  fun toList (ls : 'a ilist) : 'a list =
    (case ls
       of List(_,Nil) => []
        | List (_,Cons(_,(x,_))) => 
            (case (tl ls)
               of NONE => [x]
                | SOME ilist => [x]@toList(ilist)))

  fun fromList ({lt=f} : 'a ord) (ls : 'a list) : 'a ilist =
    (case ls
       of [] => List({lt=f},Nil)
        | x::t => cons(x,fromList {lt=f} t))   

end
