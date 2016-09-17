(* basis-names.sml
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 *
 * CMSC 22610 Sample code (Winter 2015)
 *
 * Names of identifiers and operators bound in the FLang basis.
 *)

structure BasisNames =
  struct

  (* predefined type names *)
    val bool =		Atom.atom "Bool"
    val int =		Atom.atom "Integer"
    val list =		Atom.atom "List"
    val string =	Atom.atom "String"
    val unit =		Atom.atom "Unit"

  (* operators *)
    val eql =		Atom.atom "=="
    val lte =		Atom.atom "<="
    val lt =		Atom.atom "<"
    val concat =	Atom.atom "@"
    val add =		Atom.atom "+"
    val subtract =	Atom.atom "-"
    val multiply =	Atom.atom "*"
    val divide =	Atom.atom "/"
    val modulo =	Atom.atom "%"

  (* pre-defined data constructors *)
    val boolTrue =	Atom.atom "True"
    val boolFalse =	Atom.atom "False"
    val listCons =	Atom.atom "::"	(* infix *)
    val listNil =	Atom.atom "Nil"
    val unitUnit =	Atom.atom "Unit"

  (* predefined variables *)
    val argc =		Atom.atom "argc"
    val arg =		Atom.atom "arg"
    val fail =		Atom.atom "fail"
    val ignore =	Atom.atom "ignore"
    val neg =		Atom.atom "neg"
    val not =		Atom.atom "not"
    val print =		Atom.atom "print"
    val size =		Atom.atom "size"
    val sub =		Atom.atom "sub"

  end
