(* stamp.sml
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 *
 * CMSC 22610, Winter 2015
 *
 * Stamps are locally unique identifiers used in the compiler to
 * distinguish different types, variables, etc.  For a given compilation,
 * the stamp assigned to an object is guaranteed to be unique.
 *)

structure Stamp :> sig

  (* abstract type of stamps *)
    type stamp

  (* generate a new and unique stamp *)
    val new : unit -> stamp

  (* return true if two stamps are the same stamp *)
    val same : (stamp * stamp) -> bool

  (* defines a total ordering on stamps *)
    val compare : (stamp * stamp) -> order

  (* return a hash code for a stamp *)
    val hash : stamp -> word

  (* return a string representation of a stamp.  This function is used in printing
   * identifiers.
   *)
    val toString : stamp -> string

  (* a reserved stamp used for things that need stamps, but there is not
   * a natural place to generate the stamp.
   *)
    val builtin : stamp

  end = struct

    structure W = Word

    datatype stamp = STAMP of {
	id : Word.word
      }

    val cnt = ref 0w0

    fun new () = let val w = !cnt in cnt := w+0w1; STAMP{id = w} end

    fun same (STAMP{id, ...}, STAMP{id=id', ...}) = (id = id')
    fun compare (STAMP{id, ...}, STAMP{id=id', ...}) = W.compare(id, id')
    fun hash (STAMP{id, ...}) = id

    fun toString (STAMP{id, ...}) =
	  concat["<", StringCvt.padLeft #"0" 4 (W.toString id), ">"]

    val builtin = new()

  end

