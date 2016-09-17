(* parse-tree.sml
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *
 * Parse tree representation of FLang programs.
 *)

structure ParseTree =
  struct

    type 'a mark = 'a Error.mark (* {span : span, tree : 'a} *)

    type id = Atom.atom

    datatype program
      = MarkProgram of program mark		(* source-file mark *)
      | Program of definition list * exp	(* FLang program *)

    and definition
      = MarkDef of definition mark		(* source-file mark *)
      | TypeDef of id * id list * typ		(* type definition *)
      | DataDef of id * id list * con list	(* data type definition *)
      | ValDef of bind				(* value definition *)

    and typ
      = MarkTyp of typ mark			(* source-file mark *)
      | PolyTyp of id list * typ		(* type abstraction *)
      | VarTyp of id				(* type variable *)
      | ConTyp of id * typ list			(* type constructor application *)
      | FunTyp of typ * typ			(* function type *)
      | TupleTyp of typ list			(* tuple type; list has 2+ elements *)

    and con
      = MarkCon of con mark			(* source-file mark *)
      | Con of id * typ option			(* data constructor definition *)

    and bind
      = MarkBind of bind mark			(* source-file mark *)
      | FunBind of id * param list * typ * exp	(* function definition *)
      | ValBind of pat * typ option * exp	(* value identifier definition; pat will be *)
						(* either VarPat or WildPat *)
      | ExpBind of exp				(* expression *)

    and param
      = MarkParam of param mark			(* source-file mark *)
      | TyParam of id list			(* type parameters *)
      | ValParam of (id * typ)			(* value parameter *)

    and exp
      = MarkExp of exp mark			(* source-file mark *)
      | IfExp of exp * exp * exp		(* conditional *)
      | BinExp of exp * id * exp		(* infix binary operators *)
      | AppExp of exp * exp			(* value application *)
      | TyAppExp of exp * typ list		(* type application/instantiation *)
      | VarExp of id				(* value identifier *)
      | ConExp of id				(* data constructor identifier *)
      | IntExp of IntInf.int			(* integer literal *)
      | StrExp of string			(* string literal *)
      | TupleExp of exp list			(* tuple expression; list has 2+ elements *)
      | CaseExp of exp * case_match list	(* case expression *)
      | LetExp of scope				(* local scope *)

    and scope
      = MarkScope of scope mark			(* source-file mark *)
      | Scope of bind list * exp		(* local scope *)

    and case_match
      = MarkCaseMatch of case_match mark	(* source-file mark *)
      | CaseMatch of pat * scope		(* pattern matching rule in a case expression *)

    and pat
      = MarkPat of pat mark			(* source-file mark *)
      | VarPat of id				(* variable binding pattern *)
      | ConPat of id * pat option		(* data constructor pattern *)
      | TuplePat of pat list			(* tuple pattern; list has 2+ elements *)
      | WildPat					(* wild-card pattern *)

  end
