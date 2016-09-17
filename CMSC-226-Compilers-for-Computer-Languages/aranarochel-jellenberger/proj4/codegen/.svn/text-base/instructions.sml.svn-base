(* instructions.sml
 *
 * CMSC22610 Winter 2015 Sample code.
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 *
 * See the VM Reference handout for a description of the instructions.
 *)

signature INSTRUCTIONS =
  sig

  (* VM instructions *)
    type t

    val toString : t -> string

    val add : t				(* integer addition *)
    val sub : t				(* integer subtraction *)
    val mul : t				(* integer multiplication *)
    val div : t				(* integer division *)
    val mod : t				(* integer modulo *)
    val neg : t				(* integer negation *)
    val equ : t				(* equality conparison *)
    val less : t			(* integer less-than comparison *)
    val lesseq : t			(* integer less-than-or-equal comparison *)
    val not : t				(* boolean negation *)
    val boxed : t			(* boxity test *)
    val size : t			(* string size *)
    val subscript : t			(* string indexing *)
    val alloc : int -> t		(* tuple allocation *)
    val explode : t			(* explode a tuple *)
    val select : int -> t		(* select a tuple component *)
    val int : int -> t			(* push integer literal *)
    val literal : int -> t		(* push string literal *)
    val label : Labels.t -> t		(* push code label *)
    val swap : t			(* swap top two stack elements; shorthand for swapn(1) *)
    val swapn : int -> t		(* swap top of stack with value at SP+n *)
    val pop : t				(* pop top of stack shorthand for popn(1) *)
    val popn : int -> t			(* pop n elements from top of stack *)
    val dup : t				(* duplicate the top of the stack; shorthand for push(0) *)
    val pushn : int -> t		(* push value at SP+n *)
    val loadlocal : int -> t		(* push a local variable *)
    val storelocal : int -> t		(* pop a value and store it in a local variable *)
    val loadglobal : int -> t		(* push a global variable *)
    val pushep : t			(* push the current EP *)
    val popep : t			(* pop the current EP *)
    val jmp : Labels.t -> t		(* unconditional jump *)
    val jmpif : Labels.t -> t		(* conditional jump *)
    val call : t			(* function call *)
    val entry : int -> t		(* function entry *)
    val ret : t				(* function return *)
    val tailcall : t			(* function tail call *)
    val ccall : int -> t		(* run-time-system function call *)
    val nop : t				(* no operation *)
    val halt : t			(* halt the VM *)

  end

structure Instructions : INSTRUCTIONS =
  struct

    structure F = Format
    structure Rep = CodeRep

    datatype t = datatype Rep.instr

    fun toString (I{info={name, ...}, param}) = (case param
	   of Rep.NoParam => name
	    | Rep.SignedInt n => F.format "%s(%d)"
		[F.STR name, F.INT n]
	    | Rep.UnsignedInt n => F.format "%s(%d)"
		[F.STR name, F.WORD n]
	    | Rep.Label l => F.format "%s(%s)"
		[F.STR name, F.STR(Labels.toString l)]
	  (* end case *))

  (* make an instruction that takes no arguments *)
    fun mkInstr0 info = I{info = info, param = Rep.NoParam}

  (* make an instruction that takes a label argument *)
    fun mkInstrL info label = I{info = info, param = Rep.Label label}

  (* make an instruction that takes an unsigned integer argument *)
    fun mkInstrU info n = let
	  val _ = if (n < 0)
		then raise Fail(concat[
		    "unexpected negative argument to ", #name info, " instruction"
		  ])
		else ()
	  val n = Word.fromInt n
	  in
	    I{info = info, param = Rep.UnsignedInt n}
	  end

  (* make an instruction that takes a signed integer argument *)
    fun mkInstrS info i = I{info = info, param = Rep.SignedInt i}

    nonfix div mod

  (* instruction info; note that the opcodes must agree with the VM *)
    val haltInfo	= {opc = 0w00, name = "halt", minSz = 1, maxSz = 1}
    val addInfo		= {opc = 0w01, name = "add", minSz = 1, maxSz = 1}
    val subInfo		= {opc = 0w02, name = "sub", minSz = 1, maxSz = 1}
    val mulInfo		= {opc = 0w03, name = "mul", minSz = 1, maxSz = 1}
    val divInfo		= {opc = 0w04, name = "div", minSz = 1, maxSz = 1}
    val modInfo		= {opc = 0w05, name = "mod", minSz = 1, maxSz = 1}
    val negInfo		= {opc = 0w06, name = "neg", minSz = 1, maxSz = 1}
    val equInfo		= {opc = 0w07, name = "equ", minSz = 1, maxSz = 1}
    val lessInfo	= {opc = 0w08, name = "less", minSz = 1, maxSz = 1}
    val lesseqInfo	= {opc = 0w09, name = "lesseq", minSz = 1, maxSz = 1}
    val notInfo		= {opc = 0w10, name = "not", minSz = 1, maxSz = 1}
    val boxedInfo	= {opc = 0w11, name = "boxed", minSz = 1, maxSz = 1}
    val sizeInfo	= {opc = 0w12, name = "size", minSz = 1, maxSz = 1}
    val subscriptInfo	= {opc = 0w13, name = "subscript", minSz = 1, maxSz = 1}
    val allocInfo	= {opc = 0w14, name = "alloc", minSz = 2, maxSz = 3}
    val explodeInfo	= {opc = 0w15, name = "explode", minSz = 1, maxSz = 1}
    val selectInfo	= {opc = 0w16, name = "select", minSz = 2, maxSz = 3}
    val updateInfo	= {opc = 0w17, name = "update", minSz = 1, maxSz = 1}
    val intInfo		= {opc = 0w18, name = "int", minSz = 2, maxSz = 5}
    val literalInfo	= {opc = 0w19, name = "literal", minSz = 2, maxSz = 3}
    val labelInfo	= {opc = 0w20, name = "label", minSz = 2, maxSz = 3}
    val swapInfo	= {opc = 0w21, name = "swap", minSz = 1, maxSz = 3}
    val pushInfo	= {opc = 0w22, name = "push", minSz = 2, maxSz = 3}
    val popInfo		= {opc = 0w23, name = "pop", minSz = 1, maxSz = 3}
    val loadlocalInfo	= {opc = 0w24, name = "loadlocal", minSz = 2, maxSz = 3}
    val storelocalInfo	= {opc = 0w25, name = "storelocal", minSz = 2, maxSz = 3}
    val loadglobalInfo	= {opc = 0w26, name = "loadglobal", minSz = 2, maxSz = 3}
    val pushepInfo	= {opc = 0w27, name = "pushep", minSz = 1, maxSz = 1}
    val popepInfo	= {opc = 0w28, name = "popep", minSz = 1, maxSz = 1}
    val jmpInfo		= {opc = 0w29, name = "jmp", minSz = 2, maxSz = 3}
    val jmpifInfo	= {opc = 0w30, name = "jmpif", minSz = 2, maxSz = 3}
    val callInfo	= {opc = 0w31, name = "call", minSz = 1, maxSz = 1}
    val entryInfo	= {opc = 0w32, name = "entry", minSz = 2, maxSz = 3}
    val retInfo		= {opc = 0w33, name = "ret", minSz = 1, maxSz = 1}
    val tailcallInfo	= {opc = 0w34, name = "tailcall", minSz = 1, maxSz = 1}
    val ccallInfo	= {opc = 0w35, name = "ccall", minSz = 2, maxSz = 3}
    val nopInfo		= {opc = 0w36, name = "nop", minSz = 1, maxSz = 1}

    val halt	   = mkInstr0 haltInfo
    val add	   = mkInstr0 addInfo
    val sub	   = mkInstr0 subInfo
    val mul	   = mkInstr0 mulInfo
    val div	   = mkInstr0 divInfo
    val mod	   = mkInstr0 modInfo
    val neg	   = mkInstr0 negInfo
    val equ	   = mkInstr0 equInfo
    val less	   = mkInstr0 lessInfo
    val lesseq	   = mkInstr0 lesseqInfo
    val not	   = mkInstr0 notInfo
    val boxed	   = mkInstr0 boxedInfo
    val size	   = mkInstr0 sizeInfo
    val subscript  = mkInstr0 subscriptInfo
    val alloc	   = mkInstrU allocInfo
    val explode	   = mkInstr0 explodeInfo
    val select	   = mkInstrU selectInfo
    val int	   = mkInstrS intInfo
    val literal	   = mkInstrU literalInfo
    val label	   = mkInstrL labelInfo
    val swap	   = mkInstr0 swapInfo
    val swapn	   = mkInstrU swapInfo
    val pop	   = mkInstr0 popInfo
    val popn	   = mkInstrU popInfo
    val dup	   = mkInstr0 pushInfo
    val pushn	   = mkInstrU pushInfo
    val loadlocal  = mkInstrS loadlocalInfo
    val storelocal = mkInstrS storelocalInfo
    val loadglobal = mkInstrU loadglobalInfo
    val pushep	   = mkInstr0 pushepInfo
    val popep	   = mkInstr0 popepInfo
    val jmp	   = mkInstrL jmpInfo
    val jmpif	   = mkInstrL jmpifInfo
    val call	   = mkInstr0 callInfo
    val tailcall   = mkInstr0 tailcallInfo
    val entry	   = mkInstrU entryInfo
    val ret	   = mkInstr0 retInfo
    val ccall	   = mkInstrU ccallInfo
    val nop	   = mkInstr0 nopInfo

  end
