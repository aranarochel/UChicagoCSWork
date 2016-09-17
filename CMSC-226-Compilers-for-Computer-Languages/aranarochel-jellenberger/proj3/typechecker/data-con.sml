(* data-con.sml
 *
 * CMSC22610 Winter 2015
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *)

structure DataCon : sig

    type t = Type.dcon

  (* create a new data constructor and add it to the list of data constructors
   * of its data type constructor.
   *)
    val new : Type.tycon * Atom.atom * Type.ty option -> t

  (* return the name of a data constructor *)
    val nameOf : t -> string

  (* return a unique string representation of a variable *)
    val toString : t -> string

  (* return true if this is a nullary data constructor *)
    val isNullary : t -> bool

  (* return the data-type constructor that the data constructor belongs to *)
    val tycOf : t -> Type.tycon

  (* return the signature (exploded type) of the data constructor.  For nullary
   * data constuctors, the second result will be NONE.  For data constructors that
   * belong to nullary type constructures, the first argument will be the empty
   * list.
   *)
    val sigOf : t -> Type.tyvar list * Type.ty option * Type.ty

  (* return the type of the data constructor.  This will be a polymorphic type, if the
   * owning data type is parameterized, and it will be a function type if the data
   * constructor is not nullary.
   *)
    val typeOf : t -> Type.ty

  (* are two data constructors the same? *)
    val same : t * t -> bool

  end = struct

    datatype t = datatype Type.dcon

    fun new (tyc as Type.DataTyc{tyParams, cons, ...}, name, optTy) = let
	  val dcon = DCon{
		  id = Stamp.new(),
		  name = Atom.toString name,
		  tyParams = tyParams,
		  argTy = optTy,
		  dty = tyc
		}
	  in
	    cons := dcon :: !cons;
	    dcon
	  end
      | new (tyc as Type.PrimTyc{name, ...}, _, _) =
	  raise Fail("compiler error: attempt to add dcon to primitive type " ^ name)

    fun nameOf (DCon{name, ...}) = name

    fun toString (DCon{name, id, ...}) = name ^ Stamp.toString id

    fun isNullary (DCon{argTy=NONE, ...}) = true
      | isNullary _ = false

    fun tycOf (DCon{dty, ...}) = dty

    fun sigOf (DCon{tyParams, argTy, dty, ...}) =
	  (tyParams, argTy, Type.ConTy(dty, List.map Type.VarTy tyParams))

    fun typeOf (DCon{tyParams, argTy, dty, ...}) = let
	  fun mkFunTy resTy = (case argTy
		 of NONE => resTy (* nullary constructor *)
		  | SOME argTy => Type.FunTy(argTy, resTy)
		(* end case *))
	  in
	    case tyParams
	     of [] => mkFunTy(Type.ConTy(dty, []))
	      | tvs => Type.PolyTy(tvs, mkFunTy(Type.ConTy(dty, List.map Type.VarTy tvs)))
	  end

  (* are two data constructors the same? *)
    fun same (DCon{id=a, ...}, DCon{id=b, ...}) = Stamp.same(a, b)

  end
