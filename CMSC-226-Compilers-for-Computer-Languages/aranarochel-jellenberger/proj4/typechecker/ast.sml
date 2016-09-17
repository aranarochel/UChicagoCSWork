(* ast.sml
 *
 * CMSC22610 Winter 2015
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *
 * Typed Abstract Syntax Tree representation of FLang programs and types.  Note that
 * datatype definitions have been lifted into the environment and 
 *)

structure AST =
  struct

    type dcon = DataCon.t		(* AST data constuctors *)
    type var = Var.t			(* AST value variables *)

    datatype ty = datatype Type.ty	(* AST types *)

    datatype bind
      = DataBind of TyCon.t		(* datatype definition; only in the top-level expression *)
      | FunBind of var * param list * exp (* function definition *)
      | ValBind of var * exp		(* value-variable binding *)

    and param
      = TyParam of TyVar.t list		(* function type parameters *)
      | ValParam of var			(* function value parameter *)

    and exp = Exp of exp_rep * ty	(* expression representation paired with its type *)

    and exp_rep
      = IfExp of exp * exp * exp	(* conditional *)
      | AppExp of exp * exp		(* value-function application *)
      | TyAppExp of exp * ty list	(* type-function application *)
      | TupleExp of exp list		(* tuple expression *)
      | LetExp of bind * exp		(* local value binding *)
      | CaseExp of exp * rule list	(* match-case *)
      | VarExp of var			(* variable *)
      | ConExp of dcon			(* data constructor *)
      | IntExp of IntInf.int		(* integer literal *)
      | StrExp of string		(* string literal *)

    and rule = Rule of pat * exp	(* rule in pattern match *)

    and pat
      = ConAppPat of dcon * ty list * var	(* data constructor application pattern *)
      | ConPat of dcon * ty list		(* nullary data constructor pattern *)
      | ListPat of ty * var * var		(* list :: pattern *)
      | TuplePat of var list			(* tuple pattern *)
      | VarPat of var				(* simple (i.e., variable) pattern *)

    datatype program = Program of exp

  end
