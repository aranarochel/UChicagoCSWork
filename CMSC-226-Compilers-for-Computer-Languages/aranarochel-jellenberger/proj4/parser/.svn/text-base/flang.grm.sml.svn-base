structure 
FLangTokens = struct

    datatype token = EOF
      | STRING of string
      | NUMBER of IntInf.int
      | LID of Atom.atom
      | UID of Atom.atom
      | WILD
      | DARROW
      | ARROW
      | SEMI
      | COMMA
      | MOD
      | DIV
      | TIMES
      | MINUS
      | PLUS
      | AT
      | DCOLON
      | COLON
      | LT
      | LTEQ
      | EQEQ
      | EQ
      | RCB
      | LCB
      | RB
      | LB
      | RP
      | LP
      | KW_with
      | KW_type
      | KW_then
      | KW_of
      | KW_let
      | KW_if
      | KW_fun
      | KW_end
      | KW_else
      | KW_data
      | KW_con
      | KW_case

    val allToks = [EOF, WILD, DARROW, ARROW, SEMI, COMMA, MOD, DIV, TIMES, MINUS, PLUS, AT, DCOLON, COLON, LT, LTEQ, EQEQ, EQ, RCB, LCB, RB, LB, RP, LP, KW_with, KW_type, KW_then, KW_of, KW_let, KW_if, KW_fun, KW_end, KW_else, KW_data, KW_con, KW_case]

    fun toString tok =
(case (tok)
 of (EOF) => "EOF"
  | (STRING(_)) => "STRING"
  | (NUMBER(_)) => "NUMBER"
  | (LID(_)) => "LID"
  | (UID(_)) => "UID"
  | (WILD) => "_"
  | (DARROW) => "=>"
  | (ARROW) => "->"
  | (SEMI) => ";"
  | (COMMA) => ","
  | (MOD) => "%"
  | (DIV) => "/"
  | (TIMES) => "*"
  | (MINUS) => "-"
  | (PLUS) => "+"
  | (AT) => "@"
  | (DCOLON) => "::"
  | (COLON) => ":"
  | (LT) => "<"
  | (LTEQ) => "<="
  | (EQEQ) => "=="
  | (EQ) => "="
  | (RCB) => "}"
  | (LCB) => "{"
  | (RB) => "]"
  | (LB) => "["
  | (RP) => ")"
  | (LP) => "("
  | (KW_with) => "with"
  | (KW_type) => "type"
  | (KW_then) => "then"
  | (KW_of) => "of"
  | (KW_let) => "let"
  | (KW_if) => "if"
  | (KW_fun) => "fun"
  | (KW_end) => "end"
  | (KW_else) => "else"
  | (KW_data) => "data"
  | (KW_con) => "con"
  | (KW_case) => "case"
(* end case *))
    fun isKW tok =
(case (tok)
 of (EOF) => false
  | (STRING(_)) => false
  | (NUMBER(_)) => false
  | (LID(_)) => false
  | (UID(_)) => false
  | (WILD) => false
  | (DARROW) => false
  | (ARROW) => false
  | (SEMI) => false
  | (COMMA) => false
  | (MOD) => false
  | (DIV) => false
  | (TIMES) => false
  | (MINUS) => false
  | (PLUS) => false
  | (AT) => false
  | (DCOLON) => false
  | (COLON) => false
  | (LT) => false
  | (LTEQ) => false
  | (EQEQ) => false
  | (EQ) => false
  | (RCB) => false
  | (LCB) => false
  | (RB) => false
  | (LB) => false
  | (RP) => false
  | (LP) => false
  | (KW_with) => false
  | (KW_type) => false
  | (KW_then) => false
  | (KW_of) => false
  | (KW_let) => false
  | (KW_if) => false
  | (KW_fun) => false
  | (KW_end) => false
  | (KW_else) => false
  | (KW_data) => false
  | (KW_con) => false
  | (KW_case) => false
(* end case *))

  fun isEOF EOF = true
    | isEOF _ = false

end

functor FLangParseFn(Lex : ANTLR_LEXER) = struct

  local
    structure Tok = 
FLangTokens
    structure UserCode =
      struct

  structure PT = ParseTree
  structure BN = BasisNames
  fun opt2list NONE = []
    | opt2list (SOME l) = l

  fun mark con (span, tr) = con{span = span, tree = tr}
  val markTyp = mark PT.MarkTyp
  val markExp = mark PT.MarkExp
  val markPat = mark PT.MarkPat


  fun mkLBinExp (lPos : Error.pos, lhs : PT.exp, rhs : (PT.id * PT.exp * Error.pos) list) = let
	fun mk (lhs, []) = lhs
	  | mk (lhs, (rator, e, rPos)::r) =
	      mk (markExp ((lPos, rPos), PT.BinExp(lhs, rator, e)), r)
	in
	  mk (lhs, rhs)
	end


  fun mkRBinExp (lPos : Error.pos, lhs : PT.exp, rhs : (PT.id * Error.pos * PT.exp) list, rPos : Error.pos) = let
	fun mk (_, lhs, []) = lhs
	  | mk (lPos, lhs, (rator, lPos', e)::r) = let
	      val rhs = mk (lPos', e, r)
	      in
		markExp ((lPos, rPos), PT.BinExp(lhs, rator, rhs))
	      end
	in
	  mk (lPos, lhs, rhs)
	end


fun Program_PROD_1_ACT (Definitions, Definitions_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (mark PT.MarkProgram (FULL_SPAN, PT.Program Definitions))
fun Definitions_PROD_1_ACT (SEMI, TypeDef, Definitions, SEMI_SPAN : (Lex.pos * Lex.pos), TypeDef_SPAN : (Lex.pos * Lex.pos), Definitions_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (let val (defs, exp) = Definitions
		    in
		      (TypeDef :: defs, exp)
		    end)
fun Definitions_PROD_2_ACT (SEMI, ValBind, Definitions, SEMI_SPAN : (Lex.pos * Lex.pos), ValBind_SPAN : (Lex.pos * Lex.pos), Definitions_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (let val (defs, exp) = Definitions
		    in
		      (PT.ValDef ValBind :: defs, exp)
		    end)
fun Definitions_PROD_3_ACT (SR, Exp, SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (case SR
		     of NONE => ([], Exp)
		      | SOME(defs, exp) => (PT.ValDef(PT.ExpBind Exp) :: defs, exp)
		    )
fun TypeDef_PROD_1_ACT (UID, TypeParams, KW_data, KW_with, ConDef, UID_SPAN : (Lex.pos * Lex.pos), TypeParams_SPAN : (Lex.pos * Lex.pos), KW_data_SPAN : (Lex.pos * Lex.pos), KW_with_SPAN : (Lex.pos * Lex.pos), ConDef_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (mark PT.MarkDef (FULL_SPAN, PT.DataDef(UID, opt2list TypeParams, ConDef)))
fun TypeDef_PROD_2_ACT (EQ, UID, Type, TypeParams, KW_type, EQ_SPAN : (Lex.pos * Lex.pos), UID_SPAN : (Lex.pos * Lex.pos), Type_SPAN : (Lex.pos * Lex.pos), TypeParams_SPAN : (Lex.pos * Lex.pos), KW_type_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (mark PT.MarkDef (FULL_SPAN, PT.TypeDef(UID, opt2list TypeParams, Type)))
fun ConDef_PROD_1_ACT (SR, UID, KW_con, SR_SPAN : (Lex.pos * Lex.pos), UID_SPAN : (Lex.pos * Lex.pos), KW_con_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (mark PT.MarkCon (FULL_SPAN, PT.Con(UID, SR)))
fun TypeParams_PROD_1_ACT (LB, RB, SR, LID, LB_SPAN : (Lex.pos * Lex.pos), RB_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), LID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (LID :: SR)
fun Type_PROD_1_ACT (Type, TypeParams, Type_SPAN : (Lex.pos * Lex.pos), TypeParams_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markTyp (FULL_SPAN, PT.PolyTyp(TypeParams, Type)))
fun Type_PROD_2_ACT (SR, TupleType, SR_SPAN : (Lex.pos * Lex.pos), TupleType_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (case SR
		     of NONE => TupleType
		      | SOME ty => markTyp (FULL_SPAN, PT.FunTyp(TupleType, ty))
		    )
fun TupleType_PROD_1_ACT (SR, AtomicType, SR_SPAN : (Lex.pos * Lex.pos), AtomicType_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (case SR
		 of [] => AtomicType
		  | _ => markTyp (FULL_SPAN, PT.TupleTyp(AtomicType :: SR))
		)
fun AtomicType_PROD_1_ACT (LP, RP, Type, LP_SPAN : (Lex.pos * Lex.pos), RP_SPAN : (Lex.pos * Lex.pos), Type_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (Type)
fun AtomicType_PROD_2_ACT (LID, LID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markTyp (FULL_SPAN, PT.VarTyp LID))
fun AtomicType_PROD_3_ACT (UID, TypeArgs, UID_SPAN : (Lex.pos * Lex.pos), TypeArgs_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markTyp (FULL_SPAN, PT.ConTyp(UID, opt2list TypeArgs)))
fun TypeArgs_PROD_1_ACT (LB, RB, SR, Type, LB_SPAN : (Lex.pos * Lex.pos), RB_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), Type_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (Type :: SR)
fun ValBind_PROD_1_ACT (EQ, SR, Exp, SimplePat, KW_let, EQ_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), SimplePat_SPAN : (Lex.pos * Lex.pos), KW_let_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (mark PT.MarkBind (FULL_SPAN, PT.ValBind(SimplePat, SR, Exp)))
fun ValBind_PROD_2_ACT (EQ, Exp, LID, Type, ARROW, Param, KW_fun, EQ_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), LID_SPAN : (Lex.pos * Lex.pos), Type_SPAN : (Lex.pos * Lex.pos), ARROW_SPAN : (Lex.pos * Lex.pos), Param_SPAN : (Lex.pos * Lex.pos), KW_fun_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (mark PT.MarkBind (FULL_SPAN, PT.FunBind(LID, Param, Type, Exp)))
fun Param_PROD_1_ACT (TypeParams, TypeParams_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (mark PT.MarkParam (FULL_SPAN, PT.TyParam TypeParams))
fun Param_PROD_2_ACT (LP, RP, LID, Type, COLON, LP_SPAN : (Lex.pos * Lex.pos), RP_SPAN : (Lex.pos * Lex.pos), LID_SPAN : (Lex.pos * Lex.pos), Type_SPAN : (Lex.pos * Lex.pos), COLON_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (mark PT.MarkParam (FULL_SPAN, PT.ValParam(LID, Type)))
fun Exp_PROD_1_ACT (Exp1, Exp2, Exp3, KW_else, KW_then, KW_if, Exp1_SPAN : (Lex.pos * Lex.pos), Exp2_SPAN : (Lex.pos * Lex.pos), Exp3_SPAN : (Lex.pos * Lex.pos), KW_else_SPAN : (Lex.pos * Lex.pos), KW_then_SPAN : (Lex.pos * Lex.pos), KW_if_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markExp (FULL_SPAN, PT.IfExp(Exp1, Exp2, Exp3)))
fun Exp_PROD_2_ACT (RExp, RExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (RExp)
fun RExp_PROD_1_SUBRULE_1_PROD_1_ACT (ROp, LExp, ROp_SPAN : (Lex.pos * Lex.pos), LExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (ROp, LExp, #2 LExp_SPAN)
fun RExp_PROD_1_ACT (SR, LExp, SR_SPAN : (Lex.pos * Lex.pos), LExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (mkLBinExp(#1 FULL_SPAN, LExp, SR))
fun ROp_PROD_1_ACT (EQEQ, EQEQ_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (BN.eql)
fun ROp_PROD_2_ACT (LT, LT_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (BN.lt)
fun ROp_PROD_3_ACT (LTEQ, LTEQ_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (BN.lte)
fun LExp_PROD_1_SUBRULE_1_PROD_1_ACT (SExp, DCOLON, SExp_SPAN : (Lex.pos * Lex.pos), DCOLON_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (BN.listCons, #1 SExp_SPAN, SExp)
fun LExp_PROD_1_ACT (SR, SExp, SR_SPAN : (Lex.pos * Lex.pos), SExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (mkRBinExp(#1 SExp_SPAN, SExp, SR, #2 FULL_SPAN))
fun SExp_PROD_1_SUBRULE_1_PROD_1_ACT (AT, AExp, AT_SPAN : (Lex.pos * Lex.pos), AExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (BN.concat, AExp, #2 AExp_SPAN)
fun SExp_PROD_1_ACT (SR, AExp, SR_SPAN : (Lex.pos * Lex.pos), AExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (mkLBinExp(#1 FULL_SPAN, AExp, SR))
fun AExp_PROD_1_SUBRULE_1_PROD_1_ACT (AOp, MExp, AOp_SPAN : (Lex.pos * Lex.pos), MExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (AOp, MExp, #2 MExp_SPAN)
fun AExp_PROD_1_ACT (SR, MExp, SR_SPAN : (Lex.pos * Lex.pos), MExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (mkLBinExp(#1 FULL_SPAN, MExp, SR))
fun AOp_PROD_1_ACT (PLUS, PLUS_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (BN.add)
fun AOp_PROD_2_ACT (MINUS, MINUS_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (BN.subtract)
fun MExp_PROD_1_SUBRULE_1_PROD_1_ACT (MOp, ApplyExp, MOp_SPAN : (Lex.pos * Lex.pos), ApplyExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (MOp, ApplyExp, #2 ApplyExp_SPAN)
fun MExp_PROD_1_ACT (SR, ApplyExp, SR_SPAN : (Lex.pos * Lex.pos), ApplyExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (mkLBinExp(#1 FULL_SPAN, ApplyExp, SR))
fun MOp_PROD_1_ACT (TIMES, TIMES_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (BN.multiply)
fun MOp_PROD_2_ACT (DIV, DIV_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (BN.divide)
fun MOp_PROD_3_ACT (MOD, MOD_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (BN.modulo)
fun ApplyExp_PROD_1_SUBRULE_1_PROD_1_ACT (AtomicExp, Argument, AtomicExp_SPAN : (Lex.pos * Lex.pos), Argument_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (#2 Argument_SPAN, Argument)
fun ApplyExp_PROD_1_ACT (SR, AtomicExp, SR_SPAN : (Lex.pos * Lex.pos), AtomicExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (let
		    fun mkApp (e, []) = e
		      | mkApp (e, (rPos, mk)::r) =
			  mkApp (mk((#1 AtomicExp_SPAN, rPos), e), r)
		    in
		      mkApp (AtomicExp, SR)
		    end)
fun Argument_PROD_1_ACT (AtomicExp, AtomicExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (fn (span, f) => markExp (span, PT.AppExp(f, AtomicExp)))
fun Argument_PROD_2_ACT (TypeArgs, TypeArgs_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (fn (span, f) => markExp (span, PT.TyAppExp(f, TypeArgs)))
fun AtomicExp_PROD_1_ACT (LID, LID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markExp (FULL_SPAN, PT.VarExp LID))
fun AtomicExp_PROD_2_ACT (UID, UID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markExp (FULL_SPAN, PT.ConExp UID))
fun AtomicExp_PROD_3_ACT (NUMBER, NUMBER_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markExp (FULL_SPAN, PT.IntExp NUMBER))
fun AtomicExp_PROD_4_ACT (STRING, STRING_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markExp (FULL_SPAN, PT.StrExp STRING))
fun AtomicExp_PROD_5_ACT (LP, RP, SR, Exp, LP_SPAN : (Lex.pos * Lex.pos), RP_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (case SR
		     of [] => Exp
		      | exps => markExp (FULL_SPAN, PT.TupleExp(Exp::exps))
		    )
fun AtomicExp_PROD_6_ACT (LCB, RCB, Scope, LCB_SPAN : (Lex.pos * Lex.pos), RCB_SPAN : (Lex.pos * Lex.pos), Scope_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markExp (FULL_SPAN, PT.LetExp(PT.Scope Scope)))
fun AtomicExp_PROD_7_ACT (Exp, KW_case, MatchCase, KW_of, KW_end, Exp_SPAN : (Lex.pos * Lex.pos), KW_case_SPAN : (Lex.pos * Lex.pos), MatchCase_SPAN : (Lex.pos * Lex.pos), KW_of_SPAN : (Lex.pos * Lex.pos), KW_end_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markExp (FULL_SPAN, PT.CaseExp(Exp, MatchCase)))
fun Scope_PROD_1_ACT (SEMI, ValBind, Scope, SEMI_SPAN : (Lex.pos * Lex.pos), ValBind_SPAN : (Lex.pos * Lex.pos), Scope_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (let val (defs, exp) = Scope
		    in
		      (ValBind :: defs, exp)
		    end)
fun Scope_PROD_2_ACT (SR, Exp, SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (case SR
		     of NONE => ([], Exp)
		      | SOME(defs, exp) => (PT.ExpBind Exp :: defs, exp)
		    )
fun MatchCase_PROD_1_ACT (LCB, Pat, RCB, Scope, DARROW, LCB_SPAN : (Lex.pos * Lex.pos), Pat_SPAN : (Lex.pos * Lex.pos), RCB_SPAN : (Lex.pos * Lex.pos), Scope_SPAN : (Lex.pos * Lex.pos), DARROW_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (mark
		      PT.MarkCaseMatch
		      (FULL_SPAN, PT.CaseMatch(Pat, mark PT.MarkScope (Scope_SPAN, PT.Scope Scope))))
fun Pat_PROD_1_ACT (SimplePat, SimplePat_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (SimplePat)
fun Pat_PROD_2_ACT (UID, SimplePat, UID_SPAN : (Lex.pos * Lex.pos), SimplePat_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markPat (FULL_SPAN, PT.ConPat(UID, SimplePat)))
fun Pat_PROD_3_ACT (SimplePat1, SimplePat2, DCOLON, SimplePat1_SPAN : (Lex.pos * Lex.pos), SimplePat2_SPAN : (Lex.pos * Lex.pos), DCOLON_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markPat (FULL_SPAN, PT.ConPat(BasisNames.listCons, SOME(PT.TuplePat[SimplePat1, SimplePat2]))))
fun TuplePat_PROD_1_ACT (LP, RP, SR, SimplePat, LP_SPAN : (Lex.pos * Lex.pos), RP_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), SimplePat_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (case SR
		     of [] => SimplePat
		      | pats => markPat (FULL_SPAN, PT.TuplePat(SimplePat :: pats))
		    )
fun SimplePat_PROD_1_ACT (LID, LID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markPat (FULL_SPAN, PT.VarPat LID))
fun SimplePat_PROD_2_ACT (WILD, WILD_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markPat (FULL_SPAN, PT.WildPat))
      end (* UserCode *)

    structure Err = AntlrErrHandler(
      structure Tok = Tok
      structure Lex = Lex)
    structure EBNF = AntlrEBNF(
      struct
	type strm = Err.wstream
	val getSpan = Err.getSpan
      end)

    fun mk lexFn = let
fun getS() = {}
fun putS{} = ()
fun unwrap (ret, strm, repairs) = (ret, strm, repairs)        val (eh, lex) = Err.mkErrHandler {get = getS, put = putS}
	fun fail() = Err.failure eh
	fun tryProds (strm, prods) = let
	  fun try [] = fail()
	    | try (prod :: prods) = 
	        (Err.whileDisabled eh (fn() => prod strm)) 
		handle Err.ParseError => try (prods)
          in try prods end
fun matchEOF strm = (case (lex(strm))
 of (Tok.EOF, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchSTRING strm = (case (lex(strm))
 of (Tok.STRING(x), span, strm') => (x, span, strm')
  | _ => fail()
(* end case *))
fun matchNUMBER strm = (case (lex(strm))
 of (Tok.NUMBER(x), span, strm') => (x, span, strm')
  | _ => fail()
(* end case *))
fun matchLID strm = (case (lex(strm))
 of (Tok.LID(x), span, strm') => (x, span, strm')
  | _ => fail()
(* end case *))
fun matchUID strm = (case (lex(strm))
 of (Tok.UID(x), span, strm') => (x, span, strm')
  | _ => fail()
(* end case *))
fun matchWILD strm = (case (lex(strm))
 of (Tok.WILD, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchDARROW strm = (case (lex(strm))
 of (Tok.DARROW, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchARROW strm = (case (lex(strm))
 of (Tok.ARROW, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchSEMI strm = (case (lex(strm))
 of (Tok.SEMI, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchCOMMA strm = (case (lex(strm))
 of (Tok.COMMA, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchMOD strm = (case (lex(strm))
 of (Tok.MOD, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchDIV strm = (case (lex(strm))
 of (Tok.DIV, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchTIMES strm = (case (lex(strm))
 of (Tok.TIMES, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchMINUS strm = (case (lex(strm))
 of (Tok.MINUS, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchPLUS strm = (case (lex(strm))
 of (Tok.PLUS, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchAT strm = (case (lex(strm))
 of (Tok.AT, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchDCOLON strm = (case (lex(strm))
 of (Tok.DCOLON, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchCOLON strm = (case (lex(strm))
 of (Tok.COLON, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchLT strm = (case (lex(strm))
 of (Tok.LT, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchLTEQ strm = (case (lex(strm))
 of (Tok.LTEQ, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchEQEQ strm = (case (lex(strm))
 of (Tok.EQEQ, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchEQ strm = (case (lex(strm))
 of (Tok.EQ, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchRCB strm = (case (lex(strm))
 of (Tok.RCB, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchLCB strm = (case (lex(strm))
 of (Tok.LCB, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchRB strm = (case (lex(strm))
 of (Tok.RB, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchLB strm = (case (lex(strm))
 of (Tok.LB, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchRP strm = (case (lex(strm))
 of (Tok.RP, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchLP strm = (case (lex(strm))
 of (Tok.LP, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchKW_with strm = (case (lex(strm))
 of (Tok.KW_with, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchKW_type strm = (case (lex(strm))
 of (Tok.KW_type, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchKW_then strm = (case (lex(strm))
 of (Tok.KW_then, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchKW_of strm = (case (lex(strm))
 of (Tok.KW_of, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchKW_let strm = (case (lex(strm))
 of (Tok.KW_let, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchKW_if strm = (case (lex(strm))
 of (Tok.KW_if, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchKW_fun strm = (case (lex(strm))
 of (Tok.KW_fun, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchKW_end strm = (case (lex(strm))
 of (Tok.KW_end, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchKW_else strm = (case (lex(strm))
 of (Tok.KW_else, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchKW_data strm = (case (lex(strm))
 of (Tok.KW_data, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchKW_con strm = (case (lex(strm))
 of (Tok.KW_con, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))
fun matchKW_case strm = (case (lex(strm))
 of (Tok.KW_case, span, strm') => ((), span, strm')
  | _ => fail()
(* end case *))

val (Program_NT) = 
let
fun TypeParams_NT (strm) = let
      val (LB_RES, LB_SPAN, strm') = matchLB(strm)
      val (LID_RES, LID_SPAN, strm') = matchLID(strm')
      fun TypeParams_PROD_1_SUBRULE_1_NT (strm) = let
            val (COMMA_RES, COMMA_SPAN, strm') = matchCOMMA(strm)
            val (LID_RES, LID_SPAN, strm') = matchLID(strm')
            val FULL_SPAN = (#1(COMMA_SPAN), #2(LID_SPAN))
            in
              ((LID_RES), FULL_SPAN, strm')
            end
      fun TypeParams_PROD_1_SUBRULE_1_PRED (strm) = (case (lex(strm))
             of (Tok.COMMA, _, strm') => true
              | _ => false
            (* end case *))
      val (SR_RES, SR_SPAN, strm') = EBNF.closure(TypeParams_PROD_1_SUBRULE_1_PRED, TypeParams_PROD_1_SUBRULE_1_NT, strm')
      val (RB_RES, RB_SPAN, strm') = matchRB(strm')
      val FULL_SPAN = (#1(LB_SPAN), #2(RB_SPAN))
      in
        (UserCode.TypeParams_PROD_1_ACT (LB_RES, RB_RES, SR_RES, LID_RES, LB_SPAN : (Lex.pos * Lex.pos), RB_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), LID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
          FULL_SPAN, strm')
      end
fun TypeArgs_NT (strm) = let
      val (LB_RES, LB_SPAN, strm') = matchLB(strm)
      val (Type_RES, Type_SPAN, strm') = Type_NT(strm')
      fun TypeArgs_PROD_1_SUBRULE_1_NT (strm) = let
            val (COMMA_RES, COMMA_SPAN, strm') = matchCOMMA(strm)
            val (Type_RES, Type_SPAN, strm') = Type_NT(strm')
            val FULL_SPAN = (#1(COMMA_SPAN), #2(Type_SPAN))
            in
              ((Type_RES), FULL_SPAN, strm')
            end
      fun TypeArgs_PROD_1_SUBRULE_1_PRED (strm) = (case (lex(strm))
             of (Tok.COMMA, _, strm') => true
              | _ => false
            (* end case *))
      val (SR_RES, SR_SPAN, strm') = EBNF.closure(TypeArgs_PROD_1_SUBRULE_1_PRED, TypeArgs_PROD_1_SUBRULE_1_NT, strm')
      val (RB_RES, RB_SPAN, strm') = matchRB(strm')
      val FULL_SPAN = (#1(LB_SPAN), #2(RB_SPAN))
      in
        (UserCode.TypeArgs_PROD_1_ACT (LB_RES, RB_RES, SR_RES, Type_RES, LB_SPAN : (Lex.pos * Lex.pos), RB_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), Type_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
          FULL_SPAN, strm')
      end
and Type_NT (strm) = let
      fun Type_PROD_1 (strm) = let
            val (TypeParams_RES, TypeParams_SPAN, strm') = TypeParams_NT(strm)
            val (Type_RES, Type_SPAN, strm') = Type_NT(strm')
            val FULL_SPAN = (#1(TypeParams_SPAN), #2(Type_SPAN))
            in
              (UserCode.Type_PROD_1_ACT (Type_RES, TypeParams_RES, Type_SPAN : (Lex.pos * Lex.pos), TypeParams_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun Type_PROD_2 (strm) = let
            val (TupleType_RES, TupleType_SPAN, strm') = TupleType_NT(strm)
            fun Type_PROD_2_SUBRULE_1_NT (strm) = let
                  val (ARROW_RES, ARROW_SPAN, strm') = matchARROW(strm)
                  val (Type_RES, Type_SPAN, strm') = Type_NT(strm')
                  val FULL_SPAN = (#1(ARROW_SPAN), #2(Type_SPAN))
                  in
                    ((Type_RES), FULL_SPAN, strm')
                  end
            fun Type_PROD_2_SUBRULE_1_PRED (strm) = (case (lex(strm))
                   of (Tok.ARROW, _, strm') => true
                    | _ => false
                  (* end case *))
            val (SR_RES, SR_SPAN, strm') = EBNF.optional(Type_PROD_2_SUBRULE_1_PRED, Type_PROD_2_SUBRULE_1_NT, strm')
            val FULL_SPAN = (#1(TupleType_SPAN), #2(SR_SPAN))
            in
              (UserCode.Type_PROD_2_ACT (SR_RES, TupleType_RES, SR_SPAN : (Lex.pos * Lex.pos), TupleType_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.LP, _, strm') => Type_PROD_2(strm)
          | (Tok.UID(_), _, strm') => Type_PROD_2(strm)
          | (Tok.LID(_), _, strm') => Type_PROD_2(strm)
          | (Tok.LB, _, strm') => Type_PROD_1(strm)
          | _ => fail()
        (* end case *))
      end
and TupleType_NT (strm) = let
      val (AtomicType_RES, AtomicType_SPAN, strm') = AtomicType_NT(strm)
      fun TupleType_PROD_1_SUBRULE_1_NT (strm) = let
            val (TIMES_RES, TIMES_SPAN, strm') = matchTIMES(strm)
            val (AtomicType_RES, AtomicType_SPAN, strm') = AtomicType_NT(strm')
            val FULL_SPAN = (#1(TIMES_SPAN), #2(AtomicType_SPAN))
            in
              ((AtomicType_RES), FULL_SPAN, strm')
            end
      fun TupleType_PROD_1_SUBRULE_1_PRED (strm) = (case (lex(strm))
             of (Tok.TIMES, _, strm') => true
              | _ => false
            (* end case *))
      val (SR_RES, SR_SPAN, strm') = EBNF.closure(TupleType_PROD_1_SUBRULE_1_PRED, TupleType_PROD_1_SUBRULE_1_NT, strm')
      val FULL_SPAN = (#1(AtomicType_SPAN), #2(SR_SPAN))
      in
        (UserCode.TupleType_PROD_1_ACT (SR_RES, AtomicType_RES, SR_SPAN : (Lex.pos * Lex.pos), AtomicType_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
          FULL_SPAN, strm')
      end
and AtomicType_NT (strm) = let
      fun AtomicType_PROD_1 (strm) = let
            val (LP_RES, LP_SPAN, strm') = matchLP(strm)
            val (Type_RES, Type_SPAN, strm') = Type_NT(strm')
            val (RP_RES, RP_SPAN, strm') = matchRP(strm')
            val FULL_SPAN = (#1(LP_SPAN), #2(RP_SPAN))
            in
              (UserCode.AtomicType_PROD_1_ACT (LP_RES, RP_RES, Type_RES, LP_SPAN : (Lex.pos * Lex.pos), RP_SPAN : (Lex.pos * Lex.pos), Type_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun AtomicType_PROD_2 (strm) = let
            val (LID_RES, LID_SPAN, strm') = matchLID(strm)
            val FULL_SPAN = (#1(LID_SPAN), #2(LID_SPAN))
            in
              (UserCode.AtomicType_PROD_2_ACT (LID_RES, LID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun AtomicType_PROD_3 (strm) = let
            val (UID_RES, UID_SPAN, strm') = matchUID(strm)
            fun AtomicType_PROD_3_SUBRULE_1_NT (strm) = let
                  val (TypeArgs_RES, TypeArgs_SPAN, strm') = TypeArgs_NT(strm)
                  val FULL_SPAN = (#1(TypeArgs_SPAN), #2(TypeArgs_SPAN))
                  in
                    ((TypeArgs_RES), FULL_SPAN, strm')
                  end
            fun AtomicType_PROD_3_SUBRULE_1_PRED (strm) = (case (lex(strm))
                   of (Tok.LB, _, strm') => true
                    | _ => false
                  (* end case *))
            val (TypeArgs_RES, TypeArgs_SPAN, strm') = EBNF.optional(AtomicType_PROD_3_SUBRULE_1_PRED, AtomicType_PROD_3_SUBRULE_1_NT, strm')
            val FULL_SPAN = (#1(UID_SPAN), #2(TypeArgs_SPAN))
            in
              (UserCode.AtomicType_PROD_3_ACT (UID_RES, TypeArgs_RES, UID_SPAN : (Lex.pos * Lex.pos), TypeArgs_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.UID(_), _, strm') => AtomicType_PROD_3(strm)
          | (Tok.LP, _, strm') => AtomicType_PROD_1(strm)
          | (Tok.LID(_), _, strm') => AtomicType_PROD_2(strm)
          | _ => fail()
        (* end case *))
      end
fun Param_NT (strm) = let
      fun Param_PROD_1 (strm) = let
            val (TypeParams_RES, TypeParams_SPAN, strm') = TypeParams_NT(strm)
            val FULL_SPAN = (#1(TypeParams_SPAN), #2(TypeParams_SPAN))
            in
              (UserCode.Param_PROD_1_ACT (TypeParams_RES, TypeParams_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun Param_PROD_2 (strm) = let
            val (LP_RES, LP_SPAN, strm') = matchLP(strm)
            val (LID_RES, LID_SPAN, strm') = matchLID(strm')
            val (COLON_RES, COLON_SPAN, strm') = matchCOLON(strm')
            val (Type_RES, Type_SPAN, strm') = Type_NT(strm')
            val (RP_RES, RP_SPAN, strm') = matchRP(strm')
            val FULL_SPAN = (#1(LP_SPAN), #2(RP_SPAN))
            in
              (UserCode.Param_PROD_2_ACT (LP_RES, RP_RES, LID_RES, Type_RES, COLON_RES, LP_SPAN : (Lex.pos * Lex.pos), RP_SPAN : (Lex.pos * Lex.pos), LID_SPAN : (Lex.pos * Lex.pos), Type_SPAN : (Lex.pos * Lex.pos), COLON_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.LP, _, strm') => Param_PROD_2(strm)
          | (Tok.LB, _, strm') => Param_PROD_1(strm)
          | _ => fail()
        (* end case *))
      end
fun SimplePat_NT (strm) = let
      fun SimplePat_PROD_1 (strm) = let
            val (LID_RES, LID_SPAN, strm') = matchLID(strm)
            val FULL_SPAN = (#1(LID_SPAN), #2(LID_SPAN))
            in
              (UserCode.SimplePat_PROD_1_ACT (LID_RES, LID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun SimplePat_PROD_2 (strm) = let
            val (WILD_RES, WILD_SPAN, strm') = matchWILD(strm)
            val FULL_SPAN = (#1(WILD_SPAN), #2(WILD_SPAN))
            in
              (UserCode.SimplePat_PROD_2_ACT (WILD_RES, WILD_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.WILD, _, strm') => SimplePat_PROD_2(strm)
          | (Tok.LID(_), _, strm') => SimplePat_PROD_1(strm)
          | _ => fail()
        (* end case *))
      end
fun TuplePat_NT (strm) = let
      val (LP_RES, LP_SPAN, strm') = matchLP(strm)
      val (SimplePat_RES, SimplePat_SPAN, strm') = SimplePat_NT(strm')
      fun TuplePat_PROD_1_SUBRULE_1_NT (strm) = let
            val (COMMA_RES, COMMA_SPAN, strm') = matchCOMMA(strm)
            val (SimplePat_RES, SimplePat_SPAN, strm') = SimplePat_NT(strm')
            val FULL_SPAN = (#1(COMMA_SPAN), #2(SimplePat_SPAN))
            in
              ((SimplePat_RES), FULL_SPAN, strm')
            end
      fun TuplePat_PROD_1_SUBRULE_1_PRED (strm) = (case (lex(strm))
             of (Tok.COMMA, _, strm') => true
              | _ => false
            (* end case *))
      val (SR_RES, SR_SPAN, strm') = EBNF.closure(TuplePat_PROD_1_SUBRULE_1_PRED, TuplePat_PROD_1_SUBRULE_1_NT, strm')
      val (RP_RES, RP_SPAN, strm') = matchRP(strm')
      val FULL_SPAN = (#1(LP_SPAN), #2(RP_SPAN))
      in
        (UserCode.TuplePat_PROD_1_ACT (LP_RES, RP_RES, SR_RES, SimplePat_RES, LP_SPAN : (Lex.pos * Lex.pos), RP_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), SimplePat_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
          FULL_SPAN, strm')
      end
fun Pat_NT (strm) = let
      fun Pat_PROD_1 (strm) = let
            val (SimplePat_RES, SimplePat_SPAN, strm') = SimplePat_NT(strm)
            val FULL_SPAN = (#1(SimplePat_SPAN), #2(SimplePat_SPAN))
            in
              (UserCode.Pat_PROD_1_ACT (SimplePat_RES, SimplePat_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun Pat_PROD_2 (strm) = let
            val (UID_RES, UID_SPAN, strm') = matchUID(strm)
            fun Pat_PROD_2_SUBRULE_1_NT (strm) = let
                  val (SimplePat_RES, SimplePat_SPAN, strm') = SimplePat_NT(strm)
                  val FULL_SPAN = (#1(SimplePat_SPAN), #2(SimplePat_SPAN))
                  in
                    ((SimplePat_RES), FULL_SPAN, strm')
                  end
            fun Pat_PROD_2_SUBRULE_1_PRED (strm) = (case (lex(strm))
                   of (Tok.WILD, _, strm') => true
                    | (Tok.LID(_), _, strm') => true
                    | _ => false
                  (* end case *))
            val (SimplePat_RES, SimplePat_SPAN, strm') = EBNF.optional(Pat_PROD_2_SUBRULE_1_PRED, Pat_PROD_2_SUBRULE_1_NT, strm')
            val FULL_SPAN = (#1(UID_SPAN), #2(SimplePat_SPAN))
            in
              (UserCode.Pat_PROD_2_ACT (UID_RES, SimplePat_RES, UID_SPAN : (Lex.pos * Lex.pos), SimplePat_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun Pat_PROD_3 (strm) = let
            val (SimplePat1_RES, SimplePat1_SPAN, strm') = SimplePat_NT(strm)
            val (DCOLON_RES, DCOLON_SPAN, strm') = matchDCOLON(strm')
            val (SimplePat2_RES, SimplePat2_SPAN, strm') = SimplePat_NT(strm')
            val FULL_SPAN = (#1(SimplePat1_SPAN), #2(SimplePat2_SPAN))
            in
              (UserCode.Pat_PROD_3_ACT (SimplePat1_RES, SimplePat2_RES, DCOLON_RES, SimplePat1_SPAN : (Lex.pos * Lex.pos), SimplePat2_SPAN : (Lex.pos * Lex.pos), DCOLON_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun Pat_PROD_4 (strm) = let
            val (TuplePat_RES, TuplePat_SPAN, strm') = TuplePat_NT(strm)
            val FULL_SPAN = (#1(TuplePat_SPAN), #2(TuplePat_SPAN))
            in
              ((TuplePat_RES), FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.LP, _, strm') => Pat_PROD_4(strm)
          | (Tok.UID(_), _, strm') => Pat_PROD_2(strm)
          | (Tok.WILD, _, strm') =>
              (case (lex(strm'))
               of (Tok.DARROW, _, strm') => Pat_PROD_1(strm)
                | (Tok.DCOLON, _, strm') => Pat_PROD_3(strm)
                | _ => fail()
              (* end case *))
          | (Tok.LID(_), _, strm') =>
              (case (lex(strm'))
               of (Tok.DARROW, _, strm') => Pat_PROD_1(strm)
                | (Tok.DCOLON, _, strm') => Pat_PROD_3(strm)
                | _ => fail()
              (* end case *))
          | _ => fail()
        (* end case *))
      end
fun MOp_NT (strm) = let
      fun MOp_PROD_1 (strm) = let
            val (TIMES_RES, TIMES_SPAN, strm') = matchTIMES(strm)
            val FULL_SPAN = (#1(TIMES_SPAN), #2(TIMES_SPAN))
            in
              (UserCode.MOp_PROD_1_ACT (TIMES_RES, TIMES_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun MOp_PROD_2 (strm) = let
            val (DIV_RES, DIV_SPAN, strm') = matchDIV(strm)
            val FULL_SPAN = (#1(DIV_SPAN), #2(DIV_SPAN))
            in
              (UserCode.MOp_PROD_2_ACT (DIV_RES, DIV_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun MOp_PROD_3 (strm) = let
            val (MOD_RES, MOD_SPAN, strm') = matchMOD(strm)
            val FULL_SPAN = (#1(MOD_SPAN), #2(MOD_SPAN))
            in
              (UserCode.MOp_PROD_3_ACT (MOD_RES, MOD_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.MOD, _, strm') => MOp_PROD_3(strm)
          | (Tok.TIMES, _, strm') => MOp_PROD_1(strm)
          | (Tok.DIV, _, strm') => MOp_PROD_2(strm)
          | _ => fail()
        (* end case *))
      end
fun AOp_NT (strm) = let
      fun AOp_PROD_1 (strm) = let
            val (PLUS_RES, PLUS_SPAN, strm') = matchPLUS(strm)
            val FULL_SPAN = (#1(PLUS_SPAN), #2(PLUS_SPAN))
            in
              (UserCode.AOp_PROD_1_ACT (PLUS_RES, PLUS_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun AOp_PROD_2 (strm) = let
            val (MINUS_RES, MINUS_SPAN, strm') = matchMINUS(strm)
            val FULL_SPAN = (#1(MINUS_SPAN), #2(MINUS_SPAN))
            in
              (UserCode.AOp_PROD_2_ACT (MINUS_RES, MINUS_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.MINUS, _, strm') => AOp_PROD_2(strm)
          | (Tok.PLUS, _, strm') => AOp_PROD_1(strm)
          | _ => fail()
        (* end case *))
      end
fun ROp_NT (strm) = let
      fun ROp_PROD_1 (strm) = let
            val (EQEQ_RES, EQEQ_SPAN, strm') = matchEQEQ(strm)
            val FULL_SPAN = (#1(EQEQ_SPAN), #2(EQEQ_SPAN))
            in
              (UserCode.ROp_PROD_1_ACT (EQEQ_RES, EQEQ_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun ROp_PROD_2 (strm) = let
            val (LT_RES, LT_SPAN, strm') = matchLT(strm)
            val FULL_SPAN = (#1(LT_SPAN), #2(LT_SPAN))
            in
              (UserCode.ROp_PROD_2_ACT (LT_RES, LT_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun ROp_PROD_3 (strm) = let
            val (LTEQ_RES, LTEQ_SPAN, strm') = matchLTEQ(strm)
            val FULL_SPAN = (#1(LTEQ_SPAN), #2(LTEQ_SPAN))
            in
              (UserCode.ROp_PROD_3_ACT (LTEQ_RES, LTEQ_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.LTEQ, _, strm') => ROp_PROD_3(strm)
          | (Tok.EQEQ, _, strm') => ROp_PROD_1(strm)
          | (Tok.LT, _, strm') => ROp_PROD_2(strm)
          | _ => fail()
        (* end case *))
      end
fun Exp_NT (strm) = let
      fun Exp_PROD_1 (strm) = let
            val (KW_if_RES, KW_if_SPAN, strm') = matchKW_if(strm)
            val (Exp1_RES, Exp1_SPAN, strm') = Exp_NT(strm')
            val (KW_then_RES, KW_then_SPAN, strm') = matchKW_then(strm')
            val (Exp2_RES, Exp2_SPAN, strm') = Exp_NT(strm')
            val (KW_else_RES, KW_else_SPAN, strm') = matchKW_else(strm')
            val (Exp3_RES, Exp3_SPAN, strm') = Exp_NT(strm')
            val FULL_SPAN = (#1(KW_if_SPAN), #2(Exp3_SPAN))
            in
              (UserCode.Exp_PROD_1_ACT (Exp1_RES, Exp2_RES, Exp3_RES, KW_else_RES, KW_then_RES, KW_if_RES, Exp1_SPAN : (Lex.pos * Lex.pos), Exp2_SPAN : (Lex.pos * Lex.pos), Exp3_SPAN : (Lex.pos * Lex.pos), KW_else_SPAN : (Lex.pos * Lex.pos), KW_then_SPAN : (Lex.pos * Lex.pos), KW_if_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun Exp_PROD_2 (strm) = let
            val (RExp_RES, RExp_SPAN, strm') = RExp_NT(strm)
            val FULL_SPAN = (#1(RExp_SPAN), #2(RExp_SPAN))
            in
              (UserCode.Exp_PROD_2_ACT (RExp_RES, RExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.KW_case, _, strm') => Exp_PROD_2(strm)
          | (Tok.LP, _, strm') => Exp_PROD_2(strm)
          | (Tok.LCB, _, strm') => Exp_PROD_2(strm)
          | (Tok.UID(_), _, strm') => Exp_PROD_2(strm)
          | (Tok.LID(_), _, strm') => Exp_PROD_2(strm)
          | (Tok.NUMBER(_), _, strm') => Exp_PROD_2(strm)
          | (Tok.STRING(_), _, strm') => Exp_PROD_2(strm)
          | (Tok.KW_if, _, strm') => Exp_PROD_1(strm)
          | _ => fail()
        (* end case *))
      end
and RExp_NT (strm) = let
      val (LExp_RES, LExp_SPAN, strm') = LExp_NT(strm)
      fun RExp_PROD_1_SUBRULE_1_NT (strm) = let
            val (ROp_RES, ROp_SPAN, strm') = ROp_NT(strm)
            val (LExp_RES, LExp_SPAN, strm') = LExp_NT(strm')
            val FULL_SPAN = (#1(ROp_SPAN), #2(LExp_SPAN))
            in
              (UserCode.RExp_PROD_1_SUBRULE_1_PROD_1_ACT (ROp_RES, LExp_RES, ROp_SPAN : (Lex.pos * Lex.pos), LExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun RExp_PROD_1_SUBRULE_1_PRED (strm) = (case (lex(strm))
             of (Tok.EQEQ, _, strm') => true
              | (Tok.LTEQ, _, strm') => true
              | (Tok.LT, _, strm') => true
              | _ => false
            (* end case *))
      val (SR_RES, SR_SPAN, strm') = EBNF.closure(RExp_PROD_1_SUBRULE_1_PRED, RExp_PROD_1_SUBRULE_1_NT, strm')
      val FULL_SPAN = (#1(LExp_SPAN), #2(SR_SPAN))
      in
        (UserCode.RExp_PROD_1_ACT (SR_RES, LExp_RES, SR_SPAN : (Lex.pos * Lex.pos), LExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
          FULL_SPAN, strm')
      end
and LExp_NT (strm) = let
      val (SExp_RES, SExp_SPAN, strm') = SExp_NT(strm)
      fun LExp_PROD_1_SUBRULE_1_NT (strm) = let
            val (DCOLON_RES, DCOLON_SPAN, strm') = matchDCOLON(strm)
            val (SExp_RES, SExp_SPAN, strm') = SExp_NT(strm')
            val FULL_SPAN = (#1(DCOLON_SPAN), #2(SExp_SPAN))
            in
              (UserCode.LExp_PROD_1_SUBRULE_1_PROD_1_ACT (SExp_RES, DCOLON_RES, SExp_SPAN : (Lex.pos * Lex.pos), DCOLON_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun LExp_PROD_1_SUBRULE_1_PRED (strm) = (case (lex(strm))
             of (Tok.DCOLON, _, strm') => true
              | _ => false
            (* end case *))
      val (SR_RES, SR_SPAN, strm') = EBNF.closure(LExp_PROD_1_SUBRULE_1_PRED, LExp_PROD_1_SUBRULE_1_NT, strm')
      val FULL_SPAN = (#1(SExp_SPAN), #2(SR_SPAN))
      in
        (UserCode.LExp_PROD_1_ACT (SR_RES, SExp_RES, SR_SPAN : (Lex.pos * Lex.pos), SExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
          FULL_SPAN, strm')
      end
and SExp_NT (strm) = let
      val (AExp_RES, AExp_SPAN, strm') = AExp_NT(strm)
      fun SExp_PROD_1_SUBRULE_1_NT (strm) = let
            val (AT_RES, AT_SPAN, strm') = matchAT(strm)
            val (AExp_RES, AExp_SPAN, strm') = AExp_NT(strm')
            val FULL_SPAN = (#1(AT_SPAN), #2(AExp_SPAN))
            in
              (UserCode.SExp_PROD_1_SUBRULE_1_PROD_1_ACT (AT_RES, AExp_RES, AT_SPAN : (Lex.pos * Lex.pos), AExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun SExp_PROD_1_SUBRULE_1_PRED (strm) = (case (lex(strm))
             of (Tok.AT, _, strm') => true
              | _ => false
            (* end case *))
      val (SR_RES, SR_SPAN, strm') = EBNF.closure(SExp_PROD_1_SUBRULE_1_PRED, SExp_PROD_1_SUBRULE_1_NT, strm')
      val FULL_SPAN = (#1(AExp_SPAN), #2(SR_SPAN))
      in
        (UserCode.SExp_PROD_1_ACT (SR_RES, AExp_RES, SR_SPAN : (Lex.pos * Lex.pos), AExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
          FULL_SPAN, strm')
      end
and AExp_NT (strm) = let
      val (MExp_RES, MExp_SPAN, strm') = MExp_NT(strm)
      fun AExp_PROD_1_SUBRULE_1_NT (strm) = let
            val (AOp_RES, AOp_SPAN, strm') = AOp_NT(strm)
            val (MExp_RES, MExp_SPAN, strm') = MExp_NT(strm')
            val FULL_SPAN = (#1(AOp_SPAN), #2(MExp_SPAN))
            in
              (UserCode.AExp_PROD_1_SUBRULE_1_PROD_1_ACT (AOp_RES, MExp_RES, AOp_SPAN : (Lex.pos * Lex.pos), MExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun AExp_PROD_1_SUBRULE_1_PRED (strm) = (case (lex(strm))
             of (Tok.PLUS, _, strm') => true
              | (Tok.MINUS, _, strm') => true
              | _ => false
            (* end case *))
      val (SR_RES, SR_SPAN, strm') = EBNF.closure(AExp_PROD_1_SUBRULE_1_PRED, AExp_PROD_1_SUBRULE_1_NT, strm')
      val FULL_SPAN = (#1(MExp_SPAN), #2(SR_SPAN))
      in
        (UserCode.AExp_PROD_1_ACT (SR_RES, MExp_RES, SR_SPAN : (Lex.pos * Lex.pos), MExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
          FULL_SPAN, strm')
      end
and MExp_NT (strm) = let
      val (ApplyExp_RES, ApplyExp_SPAN, strm') = ApplyExp_NT(strm)
      fun MExp_PROD_1_SUBRULE_1_NT (strm) = let
            val (MOp_RES, MOp_SPAN, strm') = MOp_NT(strm)
            val (ApplyExp_RES, ApplyExp_SPAN, strm') = ApplyExp_NT(strm')
            val FULL_SPAN = (#1(MOp_SPAN), #2(ApplyExp_SPAN))
            in
              (UserCode.MExp_PROD_1_SUBRULE_1_PROD_1_ACT (MOp_RES, ApplyExp_RES, MOp_SPAN : (Lex.pos * Lex.pos), ApplyExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun MExp_PROD_1_SUBRULE_1_PRED (strm) = (case (lex(strm))
             of (Tok.TIMES, _, strm') => true
              | (Tok.DIV, _, strm') => true
              | (Tok.MOD, _, strm') => true
              | _ => false
            (* end case *))
      val (SR_RES, SR_SPAN, strm') = EBNF.closure(MExp_PROD_1_SUBRULE_1_PRED, MExp_PROD_1_SUBRULE_1_NT, strm')
      val FULL_SPAN = (#1(ApplyExp_SPAN), #2(SR_SPAN))
      in
        (UserCode.MExp_PROD_1_ACT (SR_RES, ApplyExp_RES, SR_SPAN : (Lex.pos * Lex.pos), ApplyExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
          FULL_SPAN, strm')
      end
and ApplyExp_NT (strm) = let
      val (AtomicExp_RES, AtomicExp_SPAN, strm') = AtomicExp_NT(strm)
      fun ApplyExp_PROD_1_SUBRULE_1_NT (strm) = let
            val (Argument_RES, Argument_SPAN, strm') = Argument_NT(strm)
            val FULL_SPAN = (#1(Argument_SPAN), #2(Argument_SPAN))
            in
              (UserCode.ApplyExp_PROD_1_SUBRULE_1_PROD_1_ACT (AtomicExp_RES, Argument_RES, AtomicExp_SPAN : (Lex.pos * Lex.pos), Argument_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun ApplyExp_PROD_1_SUBRULE_1_PRED (strm) = (case (lex(strm))
             of (Tok.KW_case, _, strm') => true
              | (Tok.LP, _, strm') => true
              | (Tok.LB, _, strm') => true
              | (Tok.LCB, _, strm') => true
              | (Tok.UID(_), _, strm') => true
              | (Tok.LID(_), _, strm') => true
              | (Tok.NUMBER(_), _, strm') => true
              | (Tok.STRING(_), _, strm') => true
              | _ => false
            (* end case *))
      val (SR_RES, SR_SPAN, strm') = EBNF.closure(ApplyExp_PROD_1_SUBRULE_1_PRED, ApplyExp_PROD_1_SUBRULE_1_NT, strm')
      val FULL_SPAN = (#1(AtomicExp_SPAN), #2(SR_SPAN))
      in
        (UserCode.ApplyExp_PROD_1_ACT (SR_RES, AtomicExp_RES, SR_SPAN : (Lex.pos * Lex.pos), AtomicExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
          FULL_SPAN, strm')
      end
and Argument_NT (strm) = let
      fun Argument_PROD_1 (strm) = let
            val (AtomicExp_RES, AtomicExp_SPAN, strm') = AtomicExp_NT(strm)
            val FULL_SPAN = (#1(AtomicExp_SPAN), #2(AtomicExp_SPAN))
            in
              (UserCode.Argument_PROD_1_ACT (AtomicExp_RES, AtomicExp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun Argument_PROD_2 (strm) = let
            val (TypeArgs_RES, TypeArgs_SPAN, strm') = TypeArgs_NT(strm)
            val FULL_SPAN = (#1(TypeArgs_SPAN), #2(TypeArgs_SPAN))
            in
              (UserCode.Argument_PROD_2_ACT (TypeArgs_RES, TypeArgs_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.LB, _, strm') => Argument_PROD_2(strm)
          | (Tok.KW_case, _, strm') => Argument_PROD_1(strm)
          | (Tok.LP, _, strm') => Argument_PROD_1(strm)
          | (Tok.LCB, _, strm') => Argument_PROD_1(strm)
          | (Tok.UID(_), _, strm') => Argument_PROD_1(strm)
          | (Tok.LID(_), _, strm') => Argument_PROD_1(strm)
          | (Tok.NUMBER(_), _, strm') => Argument_PROD_1(strm)
          | (Tok.STRING(_), _, strm') => Argument_PROD_1(strm)
          | _ => fail()
        (* end case *))
      end
and AtomicExp_NT (strm) = let
      fun AtomicExp_PROD_1 (strm) = let
            val (LID_RES, LID_SPAN, strm') = matchLID(strm)
            val FULL_SPAN = (#1(LID_SPAN), #2(LID_SPAN))
            in
              (UserCode.AtomicExp_PROD_1_ACT (LID_RES, LID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun AtomicExp_PROD_2 (strm) = let
            val (UID_RES, UID_SPAN, strm') = matchUID(strm)
            val FULL_SPAN = (#1(UID_SPAN), #2(UID_SPAN))
            in
              (UserCode.AtomicExp_PROD_2_ACT (UID_RES, UID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun AtomicExp_PROD_3 (strm) = let
            val (NUMBER_RES, NUMBER_SPAN, strm') = matchNUMBER(strm)
            val FULL_SPAN = (#1(NUMBER_SPAN), #2(NUMBER_SPAN))
            in
              (UserCode.AtomicExp_PROD_3_ACT (NUMBER_RES, NUMBER_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun AtomicExp_PROD_4 (strm) = let
            val (STRING_RES, STRING_SPAN, strm') = matchSTRING(strm)
            val FULL_SPAN = (#1(STRING_SPAN), #2(STRING_SPAN))
            in
              (UserCode.AtomicExp_PROD_4_ACT (STRING_RES, STRING_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun AtomicExp_PROD_5 (strm) = let
            val (LP_RES, LP_SPAN, strm') = matchLP(strm)
            val (Exp_RES, Exp_SPAN, strm') = Exp_NT(strm')
            fun AtomicExp_PROD_5_SUBRULE_1_NT (strm) = let
                  val (COMMA_RES, COMMA_SPAN, strm') = matchCOMMA(strm)
                  val (Exp_RES, Exp_SPAN, strm') = Exp_NT(strm')
                  val FULL_SPAN = (#1(COMMA_SPAN), #2(Exp_SPAN))
                  in
                    ((Exp_RES), FULL_SPAN, strm')
                  end
            fun AtomicExp_PROD_5_SUBRULE_1_PRED (strm) = (case (lex(strm))
                   of (Tok.COMMA, _, strm') => true
                    | _ => false
                  (* end case *))
            val (SR_RES, SR_SPAN, strm') = EBNF.closure(AtomicExp_PROD_5_SUBRULE_1_PRED, AtomicExp_PROD_5_SUBRULE_1_NT, strm')
            val (RP_RES, RP_SPAN, strm') = matchRP(strm')
            val FULL_SPAN = (#1(LP_SPAN), #2(RP_SPAN))
            in
              (UserCode.AtomicExp_PROD_5_ACT (LP_RES, RP_RES, SR_RES, Exp_RES, LP_SPAN : (Lex.pos * Lex.pos), RP_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun AtomicExp_PROD_6 (strm) = let
            val (LCB_RES, LCB_SPAN, strm') = matchLCB(strm)
            val (Scope_RES, Scope_SPAN, strm') = Scope_NT(strm')
            val (RCB_RES, RCB_SPAN, strm') = matchRCB(strm')
            val FULL_SPAN = (#1(LCB_SPAN), #2(RCB_SPAN))
            in
              (UserCode.AtomicExp_PROD_6_ACT (LCB_RES, RCB_RES, Scope_RES, LCB_SPAN : (Lex.pos * Lex.pos), RCB_SPAN : (Lex.pos * Lex.pos), Scope_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun AtomicExp_PROD_7 (strm) = let
            val (KW_case_RES, KW_case_SPAN, strm') = matchKW_case(strm)
            val (Exp_RES, Exp_SPAN, strm') = Exp_NT(strm')
            val (KW_of_RES, KW_of_SPAN, strm') = matchKW_of(strm')
            fun AtomicExp_PROD_7_SUBRULE_1_NT (strm) = let
                  val (MatchCase_RES, MatchCase_SPAN, strm') = MatchCase_NT(strm)
                  val FULL_SPAN = (#1(MatchCase_SPAN), #2(MatchCase_SPAN))
                  in
                    ((MatchCase_RES), FULL_SPAN, strm')
                  end
            fun AtomicExp_PROD_7_SUBRULE_1_PRED (strm) = (case (lex(strm))
                   of (Tok.LCB, _, strm') => true
                    | _ => false
                  (* end case *))
            val (MatchCase_RES, MatchCase_SPAN, strm') = EBNF.posclos(AtomicExp_PROD_7_SUBRULE_1_PRED, AtomicExp_PROD_7_SUBRULE_1_NT, strm')
            val (KW_end_RES, KW_end_SPAN, strm') = matchKW_end(strm')
            val FULL_SPAN = (#1(KW_case_SPAN), #2(KW_end_SPAN))
            in
              (UserCode.AtomicExp_PROD_7_ACT (Exp_RES, KW_case_RES, MatchCase_RES, KW_of_RES, KW_end_RES, Exp_SPAN : (Lex.pos * Lex.pos), KW_case_SPAN : (Lex.pos * Lex.pos), MatchCase_SPAN : (Lex.pos * Lex.pos), KW_of_SPAN : (Lex.pos * Lex.pos), KW_end_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.KW_case, _, strm') => AtomicExp_PROD_7(strm)
          | (Tok.LP, _, strm') => AtomicExp_PROD_5(strm)
          | (Tok.NUMBER(_), _, strm') => AtomicExp_PROD_3(strm)
          | (Tok.LID(_), _, strm') => AtomicExp_PROD_1(strm)
          | (Tok.UID(_), _, strm') => AtomicExp_PROD_2(strm)
          | (Tok.STRING(_), _, strm') => AtomicExp_PROD_4(strm)
          | (Tok.LCB, _, strm') => AtomicExp_PROD_6(strm)
          | _ => fail()
        (* end case *))
      end
and MatchCase_NT (strm) = let
      val (LCB_RES, LCB_SPAN, strm') = matchLCB(strm)
      val (Pat_RES, Pat_SPAN, strm') = Pat_NT(strm')
      val (DARROW_RES, DARROW_SPAN, strm') = matchDARROW(strm')
      val (Scope_RES, Scope_SPAN, strm') = Scope_NT(strm')
      val (RCB_RES, RCB_SPAN, strm') = matchRCB(strm')
      val FULL_SPAN = (#1(LCB_SPAN), #2(RCB_SPAN))
      in
        (UserCode.MatchCase_PROD_1_ACT (LCB_RES, Pat_RES, RCB_RES, Scope_RES, DARROW_RES, LCB_SPAN : (Lex.pos * Lex.pos), Pat_SPAN : (Lex.pos * Lex.pos), RCB_SPAN : (Lex.pos * Lex.pos), Scope_SPAN : (Lex.pos * Lex.pos), DARROW_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
          FULL_SPAN, strm')
      end
and Scope_NT (strm) = let
      fun Scope_PROD_1 (strm) = let
            val (ValBind_RES, ValBind_SPAN, strm') = ValBind_NT(strm)
            val (SEMI_RES, SEMI_SPAN, strm') = matchSEMI(strm')
            val (Scope_RES, Scope_SPAN, strm') = Scope_NT(strm')
            val FULL_SPAN = (#1(ValBind_SPAN), #2(Scope_SPAN))
            in
              (UserCode.Scope_PROD_1_ACT (SEMI_RES, ValBind_RES, Scope_RES, SEMI_SPAN : (Lex.pos * Lex.pos), ValBind_SPAN : (Lex.pos * Lex.pos), Scope_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun Scope_PROD_2 (strm) = let
            val (Exp_RES, Exp_SPAN, strm') = Exp_NT(strm)
            fun Scope_PROD_2_SUBRULE_1_NT (strm) = let
                  val (SEMI_RES, SEMI_SPAN, strm') = matchSEMI(strm)
                  val (Scope_RES, Scope_SPAN, strm') = Scope_NT(strm')
                  val FULL_SPAN = (#1(SEMI_SPAN), #2(Scope_SPAN))
                  in
                    ((Scope_RES), FULL_SPAN, strm')
                  end
            fun Scope_PROD_2_SUBRULE_1_PRED (strm) = (case (lex(strm))
                   of (Tok.SEMI, _, strm') => true
                    | _ => false
                  (* end case *))
            val (SR_RES, SR_SPAN, strm') = EBNF.optional(Scope_PROD_2_SUBRULE_1_PRED, Scope_PROD_2_SUBRULE_1_NT, strm')
            val FULL_SPAN = (#1(Exp_SPAN), #2(SR_SPAN))
            in
              (UserCode.Scope_PROD_2_ACT (SR_RES, Exp_RES, SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.KW_case, _, strm') => Scope_PROD_2(strm)
          | (Tok.KW_if, _, strm') => Scope_PROD_2(strm)
          | (Tok.LP, _, strm') => Scope_PROD_2(strm)
          | (Tok.LCB, _, strm') => Scope_PROD_2(strm)
          | (Tok.UID(_), _, strm') => Scope_PROD_2(strm)
          | (Tok.LID(_), _, strm') => Scope_PROD_2(strm)
          | (Tok.NUMBER(_), _, strm') => Scope_PROD_2(strm)
          | (Tok.STRING(_), _, strm') => Scope_PROD_2(strm)
          | (Tok.KW_fun, _, strm') => Scope_PROD_1(strm)
          | (Tok.KW_let, _, strm') => Scope_PROD_1(strm)
          | _ => fail()
        (* end case *))
      end
and ValBind_NT (strm) = let
      fun ValBind_PROD_1 (strm) = let
            val (KW_let_RES, KW_let_SPAN, strm') = matchKW_let(strm)
            val (SimplePat_RES, SimplePat_SPAN, strm') = SimplePat_NT(strm')
            fun ValBind_PROD_1_SUBRULE_1_NT (strm) = let
                  val (COLON_RES, COLON_SPAN, strm') = matchCOLON(strm)
                  val (Type_RES, Type_SPAN, strm') = Type_NT(strm')
                  val FULL_SPAN = (#1(COLON_SPAN), #2(Type_SPAN))
                  in
                    ((Type_RES), FULL_SPAN, strm')
                  end
            fun ValBind_PROD_1_SUBRULE_1_PRED (strm) = (case (lex(strm))
                   of (Tok.COLON, _, strm') => true
                    | _ => false
                  (* end case *))
            val (SR_RES, SR_SPAN, strm') = EBNF.optional(ValBind_PROD_1_SUBRULE_1_PRED, ValBind_PROD_1_SUBRULE_1_NT, strm')
            val (EQ_RES, EQ_SPAN, strm') = matchEQ(strm')
            val (Exp_RES, Exp_SPAN, strm') = Exp_NT(strm')
            val FULL_SPAN = (#1(KW_let_SPAN), #2(Exp_SPAN))
            in
              (UserCode.ValBind_PROD_1_ACT (EQ_RES, SR_RES, Exp_RES, SimplePat_RES, KW_let_RES, EQ_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), SimplePat_SPAN : (Lex.pos * Lex.pos), KW_let_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun ValBind_PROD_2 (strm) = let
            val (KW_fun_RES, KW_fun_SPAN, strm') = matchKW_fun(strm)
            val (LID_RES, LID_SPAN, strm') = matchLID(strm')
            fun ValBind_PROD_2_SUBRULE_1_NT (strm) = let
                  val (Param_RES, Param_SPAN, strm') = Param_NT(strm)
                  val FULL_SPAN = (#1(Param_SPAN), #2(Param_SPAN))
                  in
                    ((Param_RES), FULL_SPAN, strm')
                  end
            fun ValBind_PROD_2_SUBRULE_1_PRED (strm) = (case (lex(strm))
                   of (Tok.LP, _, strm') => true
                    | (Tok.LB, _, strm') => true
                    | _ => false
                  (* end case *))
            val (Param_RES, Param_SPAN, strm') = EBNF.posclos(ValBind_PROD_2_SUBRULE_1_PRED, ValBind_PROD_2_SUBRULE_1_NT, strm')
            val (ARROW_RES, ARROW_SPAN, strm') = matchARROW(strm')
            val (Type_RES, Type_SPAN, strm') = Type_NT(strm')
            val (EQ_RES, EQ_SPAN, strm') = matchEQ(strm')
            val (Exp_RES, Exp_SPAN, strm') = Exp_NT(strm')
            val FULL_SPAN = (#1(KW_fun_SPAN), #2(Exp_SPAN))
            in
              (UserCode.ValBind_PROD_2_ACT (EQ_RES, Exp_RES, LID_RES, Type_RES, ARROW_RES, Param_RES, KW_fun_RES, EQ_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), LID_SPAN : (Lex.pos * Lex.pos), Type_SPAN : (Lex.pos * Lex.pos), ARROW_SPAN : (Lex.pos * Lex.pos), Param_SPAN : (Lex.pos * Lex.pos), KW_fun_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.KW_fun, _, strm') => ValBind_PROD_2(strm)
          | (Tok.KW_let, _, strm') => ValBind_PROD_1(strm)
          | _ => fail()
        (* end case *))
      end
fun ConDef_NT (strm) = let
      val (KW_con_RES, KW_con_SPAN, strm') = matchKW_con(strm)
      val (UID_RES, UID_SPAN, strm') = matchUID(strm')
      fun ConDef_PROD_1_SUBRULE_1_NT (strm) = let
            val (KW_of_RES, KW_of_SPAN, strm') = matchKW_of(strm)
            val (Type_RES, Type_SPAN, strm') = Type_NT(strm')
            val FULL_SPAN = (#1(KW_of_SPAN), #2(Type_SPAN))
            in
              ((Type_RES), FULL_SPAN, strm')
            end
      fun ConDef_PROD_1_SUBRULE_1_PRED (strm) = (case (lex(strm))
             of (Tok.KW_of, _, strm') => true
              | _ => false
            (* end case *))
      val (SR_RES, SR_SPAN, strm') = EBNF.optional(ConDef_PROD_1_SUBRULE_1_PRED, ConDef_PROD_1_SUBRULE_1_NT, strm')
      val FULL_SPAN = (#1(KW_con_SPAN), #2(SR_SPAN))
      in
        (UserCode.ConDef_PROD_1_ACT (SR_RES, UID_RES, KW_con_RES, SR_SPAN : (Lex.pos * Lex.pos), UID_SPAN : (Lex.pos * Lex.pos), KW_con_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
          FULL_SPAN, strm')
      end
fun TypeDef_NT (strm) = let
      fun TypeDef_PROD_1 (strm) = let
            val (KW_data_RES, KW_data_SPAN, strm') = matchKW_data(strm)
            val (UID_RES, UID_SPAN, strm') = matchUID(strm')
            fun TypeDef_PROD_1_SUBRULE_1_NT (strm) = let
                  val (TypeParams_RES, TypeParams_SPAN, strm') = TypeParams_NT(strm)
                  val FULL_SPAN = (#1(TypeParams_SPAN), #2(TypeParams_SPAN))
                  in
                    ((TypeParams_RES), FULL_SPAN, strm')
                  end
            fun TypeDef_PROD_1_SUBRULE_1_PRED (strm) = (case (lex(strm))
                   of (Tok.LB, _, strm') => true
                    | _ => false
                  (* end case *))
            val (TypeParams_RES, TypeParams_SPAN, strm') = EBNF.optional(TypeDef_PROD_1_SUBRULE_1_PRED, TypeDef_PROD_1_SUBRULE_1_NT, strm')
            val (KW_with_RES, KW_with_SPAN, strm') = matchKW_with(strm')
            fun TypeDef_PROD_1_SUBRULE_2_NT (strm) = let
                  val (ConDef_RES, ConDef_SPAN, strm') = ConDef_NT(strm)
                  val FULL_SPAN = (#1(ConDef_SPAN), #2(ConDef_SPAN))
                  in
                    ((ConDef_RES), FULL_SPAN, strm')
                  end
            fun TypeDef_PROD_1_SUBRULE_2_PRED (strm) = (case (lex(strm))
                   of (Tok.KW_con, _, strm') => true
                    | _ => false
                  (* end case *))
            val (ConDef_RES, ConDef_SPAN, strm') = EBNF.posclos(TypeDef_PROD_1_SUBRULE_2_PRED, TypeDef_PROD_1_SUBRULE_2_NT, strm')
            val FULL_SPAN = (#1(KW_data_SPAN), #2(ConDef_SPAN))
            in
              (UserCode.TypeDef_PROD_1_ACT (UID_RES, TypeParams_RES, KW_data_RES, KW_with_RES, ConDef_RES, UID_SPAN : (Lex.pos * Lex.pos), TypeParams_SPAN : (Lex.pos * Lex.pos), KW_data_SPAN : (Lex.pos * Lex.pos), KW_with_SPAN : (Lex.pos * Lex.pos), ConDef_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun TypeDef_PROD_2 (strm) = let
            val (KW_type_RES, KW_type_SPAN, strm') = matchKW_type(strm)
            val (UID_RES, UID_SPAN, strm') = matchUID(strm')
            fun TypeDef_PROD_2_SUBRULE_1_NT (strm) = let
                  val (TypeParams_RES, TypeParams_SPAN, strm') = TypeParams_NT(strm)
                  val FULL_SPAN = (#1(TypeParams_SPAN), #2(TypeParams_SPAN))
                  in
                    ((TypeParams_RES), FULL_SPAN, strm')
                  end
            fun TypeDef_PROD_2_SUBRULE_1_PRED (strm) = (case (lex(strm))
                   of (Tok.LB, _, strm') => true
                    | _ => false
                  (* end case *))
            val (TypeParams_RES, TypeParams_SPAN, strm') = EBNF.optional(TypeDef_PROD_2_SUBRULE_1_PRED, TypeDef_PROD_2_SUBRULE_1_NT, strm')
            val (EQ_RES, EQ_SPAN, strm') = matchEQ(strm')
            val (Type_RES, Type_SPAN, strm') = Type_NT(strm')
            val FULL_SPAN = (#1(KW_type_SPAN), #2(Type_SPAN))
            in
              (UserCode.TypeDef_PROD_2_ACT (EQ_RES, UID_RES, Type_RES, TypeParams_RES, KW_type_RES, EQ_SPAN : (Lex.pos * Lex.pos), UID_SPAN : (Lex.pos * Lex.pos), Type_SPAN : (Lex.pos * Lex.pos), TypeParams_SPAN : (Lex.pos * Lex.pos), KW_type_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.KW_type, _, strm') => TypeDef_PROD_2(strm)
          | (Tok.KW_data, _, strm') => TypeDef_PROD_1(strm)
          | _ => fail()
        (* end case *))
      end
fun Definitions_NT (strm) = let
      fun Definitions_PROD_1 (strm) = let
            val (TypeDef_RES, TypeDef_SPAN, strm') = TypeDef_NT(strm)
            val (SEMI_RES, SEMI_SPAN, strm') = matchSEMI(strm')
            val (Definitions_RES, Definitions_SPAN, strm') = Definitions_NT(strm')
            val FULL_SPAN = (#1(TypeDef_SPAN), #2(Definitions_SPAN))
            in
              (UserCode.Definitions_PROD_1_ACT (SEMI_RES, TypeDef_RES, Definitions_RES, SEMI_SPAN : (Lex.pos * Lex.pos), TypeDef_SPAN : (Lex.pos * Lex.pos), Definitions_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun Definitions_PROD_2 (strm) = let
            val (ValBind_RES, ValBind_SPAN, strm') = ValBind_NT(strm)
            val (SEMI_RES, SEMI_SPAN, strm') = matchSEMI(strm')
            val (Definitions_RES, Definitions_SPAN, strm') = Definitions_NT(strm')
            val FULL_SPAN = (#1(ValBind_SPAN), #2(Definitions_SPAN))
            in
              (UserCode.Definitions_PROD_2_ACT (SEMI_RES, ValBind_RES, Definitions_RES, SEMI_SPAN : (Lex.pos * Lex.pos), ValBind_SPAN : (Lex.pos * Lex.pos), Definitions_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun Definitions_PROD_3 (strm) = let
            val (Exp_RES, Exp_SPAN, strm') = Exp_NT(strm)
            fun Definitions_PROD_3_SUBRULE_1_NT (strm) = let
                  val (SEMI_RES, SEMI_SPAN, strm') = matchSEMI(strm)
                  val (Definitions_RES, Definitions_SPAN, strm') = Definitions_NT(strm')
                  val FULL_SPAN = (#1(SEMI_SPAN), #2(Definitions_SPAN))
                  in
                    ((Definitions_RES), FULL_SPAN, strm')
                  end
            fun Definitions_PROD_3_SUBRULE_1_PRED (strm) = (case (lex(strm))
                   of (Tok.SEMI, _, strm') => true
                    | _ => false
                  (* end case *))
            val (SR_RES, SR_SPAN, strm') = EBNF.optional(Definitions_PROD_3_SUBRULE_1_PRED, Definitions_PROD_3_SUBRULE_1_NT, strm')
            val FULL_SPAN = (#1(Exp_SPAN), #2(SR_SPAN))
            in
              (UserCode.Definitions_PROD_3_ACT (SR_RES, Exp_RES, SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.KW_case, _, strm') => Definitions_PROD_3(strm)
          | (Tok.KW_if, _, strm') => Definitions_PROD_3(strm)
          | (Tok.LP, _, strm') => Definitions_PROD_3(strm)
          | (Tok.LCB, _, strm') => Definitions_PROD_3(strm)
          | (Tok.UID(_), _, strm') => Definitions_PROD_3(strm)
          | (Tok.LID(_), _, strm') => Definitions_PROD_3(strm)
          | (Tok.NUMBER(_), _, strm') => Definitions_PROD_3(strm)
          | (Tok.STRING(_), _, strm') => Definitions_PROD_3(strm)
          | (Tok.KW_data, _, strm') => Definitions_PROD_1(strm)
          | (Tok.KW_type, _, strm') => Definitions_PROD_1(strm)
          | (Tok.KW_fun, _, strm') => Definitions_PROD_2(strm)
          | (Tok.KW_let, _, strm') => Definitions_PROD_2(strm)
          | _ => fail()
        (* end case *))
      end
fun Program_NT (strm) = let
      val (Definitions_RES, Definitions_SPAN, strm') = Definitions_NT(strm)
      val FULL_SPAN = (#1(Definitions_SPAN), #2(Definitions_SPAN))
      in
        (UserCode.Program_PROD_1_ACT (Definitions_RES, Definitions_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
          FULL_SPAN, strm')
      end
in
  (Program_NT)
end
val Program_NT =  fn s => unwrap (Err.launch (eh, lexFn, Program_NT , true) s)

in (Program_NT) end
  in
fun parse lexFn  s = let val (Program_NT) = mk lexFn in Program_NT s end

  end

end
