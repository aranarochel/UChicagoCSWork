(* tycon.sml
 *
 * CMSC22610 Winter 2015
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *)

structure TyCon : sig

    type t = Type.tycon

  (* return a new data tycon *)
    val new : Atom.atom * TyVar.t list -> t

  (* finish the definition of a data-type constructor by sorting its constructors
   * into canonical order.
   *)
    val finish : t -> unit

  (* return the name of a tycon *)
    val nameOf : t -> string

  (* return a unique string representation of a tycon *)
    val toString : t -> string

  (* return the arity of the type constructor *)
    val arityOf : t -> int

  (* are two type constructors the same? *)
    val same : t * t -> bool

  (* total ordering on type constructors *)
    val compare : t * t -> order

  (* primitive type constructors *)
    val integerTyc : t
    val stringTyc : t

  end = struct

    datatype t = datatype Type.tycon

    fun new (name, tvs) = DataTyc{
	    id = Stamp.new(),
	    name = Atom.toString name,
	    tyParams = tvs,
	    cons = ref []
	  }

    fun finish (DataTyc{cons, ...}) = let
	  fun gt (Type.DCon{name=a, ...}, Type.DCon{name=b, ...}) = (case String.compare(a, b)
		 of GREATER => true
		  | _ => false
		(* end case *))
	  in
	    cons := ListMergeSort.sort gt (!cons)
	  end
      | finish _ = ()

    fun nameOf (PrimTyc{name, ...}) = name
      | nameOf (DataTyc{name, ...}) = name

    fun toString (PrimTyc{name, id, ...}) = name ^ Stamp.toString id
      | toString (DataTyc{name, id, ...}) = name ^ Stamp.toString id

    fun arityOf (PrimTyc{arity, ...}) = arity
      | arityOf (DataTyc{tyParams, ...}) = List.length tyParams

  (* are two type constructors the same? *)
    fun same (PrimTyc{id=a, ...}, PrimTyc{id=b, ...}) = Stamp.same(a, b)
      | same (DataTyc{id=a, ...}, DataTyc{id=b, ...}) = Stamp.same(a, b)
      | same _ = false

    fun compare (PrimTyc{id=a, ...}, PrimTyc{id=b, ...}) = Stamp.compare(a, b)
      | compare (DataTyc{id=a, ...}, DataTyc{id=b, ...}) = Stamp.compare(a, b)
      | compare (PrimTyc _, DataTyc _) = LESS
      | compare (DataTyc _, PrimTyc _) = GREATER

  (* FLang has two primitive abstract types *)
    val integerTyc = PrimTyc{
	    id = Stamp.new(),
	    name = Atom.toString BasisNames.int,
	    arity = 0
	  }
    val stringTyc = PrimTyc{
	    id = Stamp.new(),
	    name = Atom.toString BasisNames.string,
	    arity = 0
	  }

  end
