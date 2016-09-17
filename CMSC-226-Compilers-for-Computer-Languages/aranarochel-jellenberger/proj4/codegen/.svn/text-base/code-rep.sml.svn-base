(* code-rep.sml
 *
 * CMSC22610 Winter 2015 Sample code.
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 *
 * Internal representation of generated code.
 *)

structure CodeRep =
  struct

    type instr_info = {
	  opc : word,		(* VM opcode *)
	  name : string,	(* instruction name *)
	  minSz : int,
	  maxSz : int
      }

    datatype param
      = NoParam
      | SignedInt of int
      | UnsignedInt of word
      | Label of label

    and instr = I of {
	info : instr_info,
	param : param
      }

    and label = L of {
	name : string,
	loc : int ref
      }

    and code		    (* instructions in the code stream *)
      = Loc of label
      | SDI of {		(* size is span-dependent *)
	    i : instr,
	    sz : int		(* current lower-bound on size *)
	  }
      | Instr of {		(* fixed-size instruction *)
	    i : instr,
	    sz : int
	  }

    fun sizeOf (addr, I{info, param}) = (case param
	   of NoParam => 1
	    | SignedInt n =>
		if (~128 <= n) andalso (n < 128) then 2
		else if (~32768 <= n) andalso (n < 32768) then 3
		else 5
	    | UnsignedInt n =>
		if (n < 0w256) then 2
		else if (n < 0w65536) then 3
		else raise Fail "parameter too large"
	    | Label(L{loc, name}) => if (!loc < 0)
		then 2 (* minimum size of instruction+label *)
		else let
		  val offset = !loc - addr
		  in
		    if (~128 <= offset) andalso (offset < 128) then 2
		    else if (~32768 <= offset) andalso (offset < 32768) then 3
		    else raise Fail "offset too large"
		  end
	  (* end case *))

  end
