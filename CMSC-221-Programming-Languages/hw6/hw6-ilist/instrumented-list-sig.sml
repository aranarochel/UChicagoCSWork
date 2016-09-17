signature INSTRUMENTED_LIST = sig

  type 'a ord   = {lt : 'a * 'a -> bool}
  type 'a instr = {len : int, max : 'a, min : 'a}

  datatype 'a seq
    = Nil
    | Cons of 'a instr * ('a * 'a seq)

  datatype 'a ilist
    = List of 'a ord * 'a seq

(* return NONE for empty list *)
  val hd       : 'a ilist -> 'a option

(* return NONE for empty list *)
  val tl       : 'a ilist -> 'a ilist option

(* make sure instrumentation is correctly maintained! *)
  val cons     : 'a * 'a ilist -> 'a ilist

(* the following three must all be constant-time operations *)
  val length   : 'a ilist -> int
  val max      : 'a ilist -> 'a option
  val min      : 'a ilist -> 'a option

(* the following higher-order operations must be completed in a single pass *)
(* i.e., converting to a regular list, using map/filter on that, then converting *)
(* back again, is not allowed *)
  val map      : 'b ord -> ('a -> 'b) -> 'a ilist -> 'b ilist 
  val filter   : ('a -> bool) -> 'a ilist -> 'a ilist

(* To be implemented...

  val foldr    : ('a * 'b -> 'b) -> 'b -> 'a ilist -> 'b
  val foldl    : ('a * 'b -> 'b) -> 'b -> 'a ilist -> 'b
*)
(* make sure instrumentation is correctly maintained! *)
  val rev      : 'a ilist -> 'a ilist

(* for "same", since the "ord" components of two ilists can't be compared, *)
(* just compare instrumentation data and list data *) 
  val same     : ('a * 'a -> bool) -> 'a ilist * 'a ilist -> bool

  val toList   : 'a ilist -> 'a list
  val fromList : 'a ord -> 'a list -> 'a ilist

end
