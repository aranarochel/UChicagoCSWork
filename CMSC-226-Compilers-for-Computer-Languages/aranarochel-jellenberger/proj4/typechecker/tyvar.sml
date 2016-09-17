(* tyvar.sml
 *
 * CMSC22610 Winter 2015
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *)

structure TyVar :> sig

    type t

  (* create a new type variable with the given name *)
    val new : Atom.atom -> t

  (* create a fresh type variable that has the same name as its argument *)
    val copy : t -> t

  (* return the name of a type variable *)
    val nameOf : t -> string

  (* return a unique string representation of a type variable *)
    val toString : t -> string

  (* are two type variables the same? *)
    val same : t * t -> bool

  (* sets, finite maps, and hash tables keyed by type variables *)
    structure Set : ORD_SET where type Key.ord_key = t
    structure Map : ORD_MAP where type Key.ord_key = t
    structure Tbl : MONO_HASH_TABLE where type Key.hash_key = t

  end = struct

    datatype t = TV of {
	id : Stamp.stamp,	(* unique stamp for the variable *)
	name : string		(* name of source-language type variable that this represents *)
      }

    fun new name = TV{id = Stamp.new(), name = Atom.toString name}

    fun copy (TV{name, ...}) = TV{id = Stamp.new(), name = name}

    fun nameOf (TV{name, ...}) = name

    fun toString (TV{id, name}) = name ^ Stamp.toString id

    fun same (TV{id=a, ...}, TV{id=b, ...}) = Stamp.same(a, b)

    local
      structure Ord = struct
	  type ord_key = t
	  fun compare (TV{id=a, ...}, TV{id=b, ...}) = Stamp.compare(a, b)
	end
    in
    structure Set = RedBlackSetFn (Ord)
    structure Map = RedBlackMapFn (Ord)
    end

    structure Tbl = HashTableFn (struct
        type hash_key = t
        fun hashVal (TV{id, ...}) = Stamp.hash id
        val sameKey = same
      end)

  end
