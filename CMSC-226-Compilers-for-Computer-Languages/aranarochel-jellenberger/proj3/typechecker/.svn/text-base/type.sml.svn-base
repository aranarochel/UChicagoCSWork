(* type.sml
 *
 * CMSC22610 Winter 2015
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *
 * The AST representation of types (aka semantic types).  This file also defines the
 * representation of type constructors and data constructors.
 *)

structure Type =
  struct

    type tyvar = TyVar.t

    datatype ty
      = PolyTy of tyvar list * ty	(* polymorphic type (aka type abstraction) *)
      | VarTy of tyvar			(* type variable *)
      | ConTy of tycon * ty list	(* type constructor *)
      | FunTy of ty * ty		(* function type *)
      | TupleTy of ty list		(* tuple type *)
      | ErrorTy				(* error type; we use this to avoid cascading errors *)

    and tycon
      = PrimTyc of {			(* primitive builtin types *)
	    name : string,		  (* type name *)
	    id : Stamp.stamp,		  (* unique identifier *)
	    arity : int			  (* number of type arguments *)
	  }
      | DataTyc of {			(* data-type constructor *)
	    name : string,		  (* type name *)
	    id : Stamp.stamp,		  (* unique identifier *)
	    tyParams : tyvar list,	  (* type parameters; these are also used in the types *)
					  (* of the data constructors *)
	    cons : dcon list ref	  (* data constructors *)
	  }

    and dcon = DCon of {		(* data constructor *)
	name : string,		  	  (* constructor name *)
	id : Stamp.stamp,		  (* unique identifier *)
	tyParams : tyvar list,		  (* type parameters (the same as the tycon) *)
	argTy : ty option,		  (* argument type; NONE for nullary constructors *)
	dty : tycon			  (* the type constructor that this dcon belongs to *)
      }

  end
