(* simple.sml
 *
 * CMSC22610 Winter 2015 Sample code.
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *
 * A simplified first-order IR that encodes representation information.
 *)

structure Simple =
  struct

datatype program = Program of definition list

and definition = Function of string * exp list * exp list * exp
(*									ParameterExp	VarExp 		 *)

and exp = LetExp of exp * exp * exp
		| IfExp of exp * exp * exp
		| FAppExp of exp * exp list * exp
		| OpAppExp of primitive * exp list
		| TupleExp of exp list
		| ComponentExp of exp * int
		| FunExp of string 
		| GlobalVarExp of string * int
		| ParameterExp of string
		| LocalVarExp of string * int
		| SelfExp of exp 				(*function * exp list can be represented as a Tuple Exp*)
		| NumExp of int
		| StringExp of string

and primitive = Add | Sub | Mul | Div | Mod |
				Neg | Equ | Less | LessEq | Not |
				Boxed | ConC | Argc | Arg | Ignore |
				Print | Size | SUB | Fail

  end
