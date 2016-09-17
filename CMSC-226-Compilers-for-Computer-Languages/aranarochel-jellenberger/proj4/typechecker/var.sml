(* var.sml
 *
 * CMSC22610 Winter 2015
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *)

structure Var :> sig

    type t

  (* create a new semantic variable with the given name and type *)
    val new : Atom.atom * Type.ty -> t

  (* create a new primitive operator variable *)
    val prim : Atom.atom * Type.ty -> t

  (* variable for wildcard *)
    val wild : Type.ty -> t

  (* return a variable's name *)
    val nameOf : t -> string

  (* return a variable's type *)
    val typeOf : t -> Type.ty

  (* return a unique string representation of a variable *)
    val toString : t -> string

  (* is a variable a primitive operator? *)
    val isPrim : t -> bool

  (* are two variables the same? *)
    val same : t * t -> bool

  (* sets, finite maps, and hash tables keyed by variables *)
    structure Set : ORD_SET where type Key.ord_key = t
    structure Map : ORD_MAP where type Key.ord_key = t
    structure Tbl : MONO_HASH_TABLE where type Key.hash_key = t

  end = struct

  (* value variables in the AST *)
    datatype t = V of {
	name : string,
	id : Stamp.stamp,	(* unique stamp that distinguishes this variable *)
	isPrim : bool,		(* true for builtin primitives (e.g., "+", "print", ...) *)
	ty : Type.ty
      }

    fun new (name, ty) = V{name = Atom.toString name, id = Stamp.new(), isPrim = false, ty = ty}

    fun prim (name, ty) = V{name = Atom.toString name, id = Stamp.new(), isPrim = true, ty = ty}

    fun wild ty = V{name = "_", id = Stamp.new(), isPrim = false, ty = ty}

    fun nameOf (V{name, ...}) = name

    fun typeOf (V{ty, ...}) = ty

    fun toString (V{name, id, ...}) = name ^ Stamp.toString id

    fun isPrim (V{isPrim=p, ...}) = p

    fun same (V{id=a, ...}, V{id=b, ...}) = Stamp.same(a, b)

    structure Key =
      struct
	type ord_key = t
	fun compare (V{id=a, ...}, V{id=b, ...}) = Stamp.compare(a, b)
      end
    structure Map = RedBlackMapFn (Key)
    structure Set = RedBlackSetFn (Key)

    structure Tbl = HashTableFn (struct
	type hash_key = t
	fun hashVal (V{id, ...}) = Stamp.hash id
	val sameKey = same
      end)

  end
