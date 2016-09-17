(* labels.sml
 *
 * CMSC22610 Winter 2015 Sample code.
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 *)

signature LABELS =
  sig
    type t
    val new : string -> t
    val toString : t -> string
  end

structure Labels : LABELS =
  struct

    datatype t = datatype CodeRep.label

    val cnt = ref 0

    fun new prefix = let
	  val n = !cnt
	  in
	    cnt := n+1;
	    L{name = concat[prefix, "$", Int.toString n], loc = ref ~1}
	  end

    fun toString (L{name, loc}) =
	  if (!loc < 0)
	    then name
	    else concat[name, " @ ", Int.toString(!loc)]

  end
