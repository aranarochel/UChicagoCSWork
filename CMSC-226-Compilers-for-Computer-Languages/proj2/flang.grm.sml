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

  fun markProgram (span,prog)   = PT.MarkProgram{span=span, tree=prog}
  fun markDefinition (span,def) = PT.MarkDef{span=span, tree=def}
  fun markTyp (span,typ)        = PT.MarkTyp{span=span, tree=typ}
  fun markCon (span,con)        = PT.MarkCon{span=span, tree=con}
  fun markBind (span,bind)      = PT.MarkBind{span=span, tree=bind}
  fun markParam (span,param)    = PT.MarkParam{span=span, tree=param}
  fun markExp (span,exp)        = PT.MarkExp{span=span, tree=exp}
  fun markScope (span,scope)    = PT.MarkScope{span=span, tree=scope}
  fun markCaseMatch (span,cm)   = PT.MarkCaseMatch{span=span, tree=cm}
  fun markPat (span,pat)        = PT.MarkPat{span=span, tree=pat}

fun Program_PROD_1_ACT (SR, Exp, SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markProgram(FULL_SPAN,PT.Program(SR, Exp)))
fun Definition_PROD_1_ACT (EQ, SR, UID, Type, KW_type, EQ_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), UID_SPAN : (Lex.pos * Lex.pos), Type_SPAN : (Lex.pos * Lex.pos), KW_type_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markDefinition(FULL_SPAN,PT.TypeDef(UID,valOf(SR),Type)))
fun Definition_PROD_2_ACT (SR1, SR2, UID, KW_data, KW_with, SR1_SPAN : (Lex.pos * Lex.pos), SR2_SPAN : (Lex.pos * Lex.pos), UID_SPAN : (Lex.pos * Lex.pos), KW_data_SPAN : (Lex.pos * Lex.pos), KW_with_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markDefinition(FULL_SPAN,PT.DataDef(UID,valOf(SR1),SR2)))
fun Definition_PROD_3_ACT (ValBind, ValBind_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markDefinition(FULL_SPAN,PT.ValDef ValBind))
fun ConDef_PROD_1_ACT (SR, UID, KW_con, SR_SPAN : (Lex.pos * Lex.pos), UID_SPAN : (Lex.pos * Lex.pos), KW_con_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markCon(FULL_SPAN,PT.Con(UID,SR)))
fun TypeParams_PROD_1_ACT (LB, RB, SR, LID, LB_SPAN : (Lex.pos * Lex.pos), RB_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), LID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (LID::SR)
fun Type_PROD_1_ACT (Type, ARROW, TypeA, Type_SPAN : (Lex.pos * Lex.pos), ARROW_SPAN : (Lex.pos * Lex.pos), TypeA_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markTyp(FULL_SPAN,PT.FunTyp(TypeA,Type)))
fun TypeA_PROD_1_ACT (UID, UID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markTyp(FULL_SPAN,PT.VarTyp UID))
fun TypeA_PROD_2_ACT (LP, RP, Type, LP_SPAN : (Lex.pos * Lex.pos), RP_SPAN : (Lex.pos * Lex.pos), Type_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (Type)
fun TypeA_PROD_3_ACT (Type, TypeParams, Type_SPAN : (Lex.pos * Lex.pos), TypeParams_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markTyp(FULL_SPAN,PT.PolyTyp(TypeParams,Type)))
fun ValBind_PROD_1_ACT (EQ, SR, Exp, LID, Type, ARROW, KW_fun, EQ_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), LID_SPAN : (Lex.pos * Lex.pos), Type_SPAN : (Lex.pos * Lex.pos), ARROW_SPAN : (Lex.pos * Lex.pos), KW_fun_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markBind(FULL_SPAN,PT.FunBind(LID,SR,Type,Exp)))
fun ValBind_PROD_2_ACT (EQ, SR, Exp, SimplePat, KW_let, EQ_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), SimplePat_SPAN : (Lex.pos * Lex.pos), KW_let_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markBind(FULL_SPAN,PT.ValBind(SimplePat,SR,Exp)))
fun FunParam_PROD_1_ACT (TypeParams, TypeParams_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markParam(FULL_SPAN,PT.TyParam(TypeParams)))
fun FunParam_PROD_2_ACT (LP, RP, LID, Type, COLON, LP_SPAN : (Lex.pos * Lex.pos), RP_SPAN : (Lex.pos * Lex.pos), LID_SPAN : (Lex.pos * Lex.pos), Type_SPAN : (Lex.pos * Lex.pos), COLON_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markParam(FULL_SPAN,PT.ValParam(LID,Type)))
fun AtomicExp_PROD_1_ACT (SR, Exp, KW_case, KW_of, KW_end, SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), KW_case_SPAN : (Lex.pos * Lex.pos), KW_of_SPAN : (Lex.pos * Lex.pos), KW_end_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markExp(FULL_SPAN,PT.CaseExp(Exp,SR)))
fun AtomicExp_PROD_2_ACT (LCB, RCB, Scope, LCB_SPAN : (Lex.pos * Lex.pos), RCB_SPAN : (Lex.pos * Lex.pos), Scope_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markExp(FULL_SPAN,PT.LetExp(Scope)))
fun AtomicExp_PROD_3_ACT (LP, RP, SR, Exp, LP_SPAN : (Lex.pos * Lex.pos), RP_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markExp(FULL_SPAN,PT.TupleExp(Exp::SR)))
fun AtomicA_PROD_1_ACT (NUMBER, NUMBER_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (PT.IntExp NUMBER)
fun AtomicA_PROD_2_ACT (STRING, STRING_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (PT.StrExp STRING)
fun AtomicA_PROD_3_ACT (LID, LID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (PT.VarExp LID)
fun AtomicA_PROD_4_ACT (UID, UID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (PT.ConExp UID)
fun MatchCase_PROD_1_ACT (LCB, Pat, RCB, Scope, DARROW, LCB_SPAN : (Lex.pos * Lex.pos), Pat_SPAN : (Lex.pos * Lex.pos), RCB_SPAN : (Lex.pos * Lex.pos), Scope_SPAN : (Lex.pos * Lex.pos), DARROW_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markCaseMatch(FULL_SPAN,PT.CaseMatch(Pat,Scope)))
fun Scope_PROD_1_ACT (SR, Exp, SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markScope(FULL_SPAN,PT.Scope(SR,Exp)))
fun Pat_PROD_2_ACT (SR, UID, SR_SPAN : (Lex.pos * Lex.pos), UID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markPat(FULL_SPAN,PT.ConPat(UID,SR)))
fun Pat_PROD_3_ACT (LP, RP, SR, SimplePat, LP_SPAN : (Lex.pos * Lex.pos), RP_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), SimplePat_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markPat(FULL_SPAN,PT.TuplePat(SimplePat::SR)))
fun SimplePat_PROD_1_ACT (LID, LID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markPat(FULL_SPAN,PT.VarPat(LID)))
fun SimplePat_PROD_2_ACT (WILD, WILD_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)) = 
  (markPat(FULL_SPAN,PT.WildPat))
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
fun AtomicA_NT (strm) = let
      fun AtomicA_PROD_1 (strm) = let
            val (NUMBER_RES, NUMBER_SPAN, strm') = matchNUMBER(strm)
            val FULL_SPAN = (#1(NUMBER_SPAN), #2(NUMBER_SPAN))
            in
              (UserCode.AtomicA_PROD_1_ACT (NUMBER_RES, NUMBER_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun AtomicA_PROD_2 (strm) = let
            val (STRING_RES, STRING_SPAN, strm') = matchSTRING(strm)
            val FULL_SPAN = (#1(STRING_SPAN), #2(STRING_SPAN))
            in
              (UserCode.AtomicA_PROD_2_ACT (STRING_RES, STRING_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun AtomicA_PROD_3 (strm) = let
            val (LID_RES, LID_SPAN, strm') = matchLID(strm)
            val FULL_SPAN = (#1(LID_SPAN), #2(LID_SPAN))
            in
              (UserCode.AtomicA_PROD_3_ACT (LID_RES, LID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun AtomicA_PROD_4 (strm) = let
            val (UID_RES, UID_SPAN, strm') = matchUID(strm)
            val FULL_SPAN = (#1(UID_SPAN), #2(UID_SPAN))
            in
              (UserCode.AtomicA_PROD_4_ACT (UID_RES, UID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.UID(_), _, strm') => AtomicA_PROD_4(strm)
          | (Tok.STRING(_), _, strm') => AtomicA_PROD_2(strm)
          | (Tok.NUMBER(_), _, strm') => AtomicA_PROD_1(strm)
          | (Tok.LID(_), _, strm') => AtomicA_PROD_3(strm)
          | _ => fail()
        (* end case *))
      end
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
fun Type_NT (strm) = let
      val (TypeA_RES, TypeA_SPAN, strm') = TypeA_NT(strm)
      val (ARROW_RES, ARROW_SPAN, strm') = matchARROW(strm')
      val (Type_RES, Type_SPAN, strm') = Type_NT(strm')
      val FULL_SPAN = (#1(TypeA_SPAN), #2(Type_SPAN))
      in
        (UserCode.Type_PROD_1_ACT (Type_RES, ARROW_RES, TypeA_RES, Type_SPAN : (Lex.pos * Lex.pos), ARROW_SPAN : (Lex.pos * Lex.pos), TypeA_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
          FULL_SPAN, strm')
      end
and TypeA_NT (strm) = let
      fun TypeA_PROD_1 (strm) = let
            val (UID_RES, UID_SPAN, strm') = matchUID(strm)
            val FULL_SPAN = (#1(UID_SPAN), #2(UID_SPAN))
            in
              (UserCode.TypeA_PROD_1_ACT (UID_RES, UID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun TypeA_PROD_2 (strm) = let
            val (LP_RES, LP_SPAN, strm') = matchLP(strm)
            val (Type_RES, Type_SPAN, strm') = Type_NT(strm')
            val (RP_RES, RP_SPAN, strm') = matchRP(strm')
            val FULL_SPAN = (#1(LP_SPAN), #2(RP_SPAN))
            in
              (UserCode.TypeA_PROD_2_ACT (LP_RES, RP_RES, Type_RES, LP_SPAN : (Lex.pos * Lex.pos), RP_SPAN : (Lex.pos * Lex.pos), Type_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun TypeA_PROD_3 (strm) = let
            val (TypeParams_RES, TypeParams_SPAN, strm') = TypeParams_NT(strm)
            val (Type_RES, Type_SPAN, strm') = Type_NT(strm')
            val FULL_SPAN = (#1(TypeParams_SPAN), #2(Type_SPAN))
            in
              (UserCode.TypeA_PROD_3_ACT (Type_RES, TypeParams_RES, Type_SPAN : (Lex.pos * Lex.pos), TypeParams_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.LB, _, strm') => TypeA_PROD_3(strm)
          | (Tok.UID(_), _, strm') => TypeA_PROD_1(strm)
          | (Tok.LP, _, strm') => TypeA_PROD_2(strm)
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
fun FunParam_NT (strm) = let
      fun FunParam_PROD_1 (strm) = let
            val (TypeParams_RES, TypeParams_SPAN, strm') = TypeParams_NT(strm)
            val FULL_SPAN = (#1(TypeParams_SPAN), #2(TypeParams_SPAN))
            in
              (UserCode.FunParam_PROD_1_ACT (TypeParams_RES, TypeParams_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun FunParam_PROD_2 (strm) = let
            val (LP_RES, LP_SPAN, strm') = matchLP(strm)
            val (LID_RES, LID_SPAN, strm') = matchLID(strm')
            val (COLON_RES, COLON_SPAN, strm') = matchCOLON(strm')
            val (Type_RES, Type_SPAN, strm') = Type_NT(strm')
            val (RP_RES, RP_SPAN, strm') = matchRP(strm')
            val FULL_SPAN = (#1(LP_SPAN), #2(RP_SPAN))
            in
              (UserCode.FunParam_PROD_2_ACT (LP_RES, RP_RES, LID_RES, Type_RES, COLON_RES, LP_SPAN : (Lex.pos * Lex.pos), RP_SPAN : (Lex.pos * Lex.pos), LID_SPAN : (Lex.pos * Lex.pos), Type_SPAN : (Lex.pos * Lex.pos), COLON_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.LP, _, strm') => FunParam_PROD_2(strm)
          | (Tok.LB, _, strm') => FunParam_PROD_1(strm)
          | _ => fail()
        (* end case *))
      end
fun Pat_NT (strm) = let
      fun Pat_PROD_1 (strm) = let
            val (SimplePat_RES, SimplePat_SPAN, strm') = SimplePat_NT(strm)
            val FULL_SPAN = (#1(SimplePat_SPAN), #2(SimplePat_SPAN))
            in
              ((SimplePat_RES), FULL_SPAN, strm')
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
            val (SR_RES, SR_SPAN, strm') = EBNF.optional(Pat_PROD_2_SUBRULE_1_PRED, Pat_PROD_2_SUBRULE_1_NT, strm')
            val FULL_SPAN = (#1(UID_SPAN), #2(SR_SPAN))
            in
              (UserCode.Pat_PROD_2_ACT (SR_RES, UID_RES, SR_SPAN : (Lex.pos * Lex.pos), UID_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun Pat_PROD_3 (strm) = let
            val (LP_RES, LP_SPAN, strm') = matchLP(strm)
            val (SimplePat_RES, SimplePat_SPAN, strm') = SimplePat_NT(strm')
            fun Pat_PROD_3_SUBRULE_1_NT (strm) = let
                  val (COMMA_RES, COMMA_SPAN, strm') = matchCOMMA(strm)
                  val (SimplePat_RES, SimplePat_SPAN, strm') = SimplePat_NT(strm')
                  val FULL_SPAN = (#1(COMMA_SPAN), #2(SimplePat_SPAN))
                  in
                    ((SimplePat_RES), FULL_SPAN, strm')
                  end
            fun Pat_PROD_3_SUBRULE_1_PRED (strm) = (case (lex(strm))
                   of (Tok.COMMA, _, strm') => true
                    | _ => false
                  (* end case *))
            val (SR_RES, SR_SPAN, strm') = EBNF.closure(Pat_PROD_3_SUBRULE_1_PRED, Pat_PROD_3_SUBRULE_1_NT, strm')
            val (RP_RES, RP_SPAN, strm') = matchRP(strm')
            val FULL_SPAN = (#1(LP_SPAN), #2(RP_SPAN))
            in
              (UserCode.Pat_PROD_3_ACT (LP_RES, RP_RES, SR_RES, SimplePat_RES, LP_SPAN : (Lex.pos * Lex.pos), RP_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), SimplePat_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.LP, _, strm') => Pat_PROD_3(strm)
          | (Tok.WILD, _, strm') => Pat_PROD_1(strm)
          | (Tok.LID(_), _, strm') => Pat_PROD_1(strm)
          | (Tok.UID(_), _, strm') => Pat_PROD_2(strm)
          | _ => fail()
        (* end case *))
      end
fun Exp_NT (strm) = let
      val (ApplyExp_RES, ApplyExp_SPAN, strm') = ApplyExp_NT(strm)
      val FULL_SPAN = (#1(ApplyExp_SPAN), #2(ApplyExp_SPAN))
      in
        ((ApplyExp_RES), FULL_SPAN, strm')
      end
and ApplyExp_NT (strm) = let
      val (ApplyExpA_RES, ApplyExpA_SPAN, strm') = ApplyExpA_NT(strm)
      val FULL_SPAN = (#1(ApplyExpA_SPAN), #2(ApplyExpA_SPAN))
      in
        ((ApplyExpA_RES), FULL_SPAN, strm')
      end
and ApplyExpA_NT (strm) = let
      fun ApplyExpA_PROD_1 (strm) = let
            val (AtomicExp_RES, AtomicExp_SPAN, strm') = AtomicExp_NT(strm)
            val FULL_SPAN = (#1(AtomicExp_SPAN), #2(AtomicExp_SPAN))
            in
              ((AtomicExp_RES), FULL_SPAN, strm')
            end
      fun ApplyExpA_PROD_2 (strm) = let
            val (AtomicA_RES, AtomicA_SPAN, strm') = AtomicA_NT(strm)
            val FULL_SPAN = (#1(AtomicA_SPAN), #2(AtomicA_SPAN))
            in
              ((AtomicA_RES), FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.UID(_), _, strm') => ApplyExpA_PROD_2(strm)
          | (Tok.LID(_), _, strm') => ApplyExpA_PROD_2(strm)
          | (Tok.NUMBER(_), _, strm') => ApplyExpA_PROD_2(strm)
          | (Tok.STRING(_), _, strm') => ApplyExpA_PROD_2(strm)
          | (Tok.KW_case, _, strm') => ApplyExpA_PROD_1(strm)
          | (Tok.LP, _, strm') => ApplyExpA_PROD_1(strm)
          | (Tok.LCB, _, strm') => ApplyExpA_PROD_1(strm)
          | _ => fail()
        (* end case *))
      end
and AtomicExp_NT (strm) = let
      fun AtomicExp_PROD_1 (strm) = let
            val (KW_case_RES, KW_case_SPAN, strm') = matchKW_case(strm)
            val (Exp_RES, Exp_SPAN, strm') = Exp_NT(strm')
            val (KW_of_RES, KW_of_SPAN, strm') = matchKW_of(strm')
            fun AtomicExp_PROD_1_SUBRULE_1_NT (strm) = let
                  val (MatchCase_RES, MatchCase_SPAN, strm') = MatchCase_NT(strm)
                  val FULL_SPAN = (#1(MatchCase_SPAN), #2(MatchCase_SPAN))
                  in
                    ((MatchCase_RES), FULL_SPAN, strm')
                  end
            fun AtomicExp_PROD_1_SUBRULE_1_PRED (strm) = (case (lex(strm))
                   of (Tok.LCB, _, strm') => true
                    | _ => false
                  (* end case *))
            val (SR_RES, SR_SPAN, strm') = EBNF.posclos(AtomicExp_PROD_1_SUBRULE_1_PRED, AtomicExp_PROD_1_SUBRULE_1_NT, strm')
            val (KW_end_RES, KW_end_SPAN, strm') = matchKW_end(strm')
            val FULL_SPAN = (#1(KW_case_SPAN), #2(KW_end_SPAN))
            in
              (UserCode.AtomicExp_PROD_1_ACT (SR_RES, Exp_RES, KW_case_RES, KW_of_RES, KW_end_RES, SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), KW_case_SPAN : (Lex.pos * Lex.pos), KW_of_SPAN : (Lex.pos * Lex.pos), KW_end_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun AtomicExp_PROD_2 (strm) = let
            val (LCB_RES, LCB_SPAN, strm') = matchLCB(strm)
            val (Scope_RES, Scope_SPAN, strm') = Scope_NT(strm')
            val (RCB_RES, RCB_SPAN, strm') = matchRCB(strm')
            val FULL_SPAN = (#1(LCB_SPAN), #2(RCB_SPAN))
            in
              (UserCode.AtomicExp_PROD_2_ACT (LCB_RES, RCB_RES, Scope_RES, LCB_SPAN : (Lex.pos * Lex.pos), RCB_SPAN : (Lex.pos * Lex.pos), Scope_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun AtomicExp_PROD_3 (strm) = let
            val (LP_RES, LP_SPAN, strm') = matchLP(strm)
            val (Exp_RES, Exp_SPAN, strm') = Exp_NT(strm')
            fun AtomicExp_PROD_3_SUBRULE_1_NT (strm) = let
                  val (COMMA_RES, COMMA_SPAN, strm') = matchCOMMA(strm)
                  val (Exp_RES, Exp_SPAN, strm') = Exp_NT(strm')
                  val FULL_SPAN = (#1(COMMA_SPAN), #2(Exp_SPAN))
                  in
                    ((Exp_RES), FULL_SPAN, strm')
                  end
            fun AtomicExp_PROD_3_SUBRULE_1_PRED (strm) = (case (lex(strm))
                   of (Tok.COMMA, _, strm') => true
                    | _ => false
                  (* end case *))
            val (SR_RES, SR_SPAN, strm') = EBNF.closure(AtomicExp_PROD_3_SUBRULE_1_PRED, AtomicExp_PROD_3_SUBRULE_1_NT, strm')
            val (RP_RES, RP_SPAN, strm') = matchRP(strm')
            val FULL_SPAN = (#1(LP_SPAN), #2(RP_SPAN))
            in
              (UserCode.AtomicExp_PROD_3_ACT (LP_RES, RP_RES, SR_RES, Exp_RES, LP_SPAN : (Lex.pos * Lex.pos), RP_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.LP, _, strm') => AtomicExp_PROD_3(strm)
          | (Tok.KW_case, _, strm') => AtomicExp_PROD_1(strm)
          | (Tok.LCB, _, strm') => AtomicExp_PROD_2(strm)
          | _ => fail()
        (* end case *))
      end
and Scope_NT (strm) = let
      fun Scope_PROD_1_SUBRULE_1_NT (strm) = let
            val (ValBind_RES, ValBind_SPAN, strm') = ValBind_NT(strm)
            val (SEMI_RES, SEMI_SPAN, strm') = matchSEMI(strm')
            val FULL_SPAN = (#1(ValBind_SPAN), #2(SEMI_SPAN))
            in
              ((ValBind_RES), FULL_SPAN, strm')
            end
      fun Scope_PROD_1_SUBRULE_1_PRED (strm) = (case (lex(strm))
             of (Tok.KW_fun, _, strm') => true
              | (Tok.KW_let, _, strm') => true
              | _ => false
            (* end case *))
      val (SR_RES, SR_SPAN, strm') = EBNF.closure(Scope_PROD_1_SUBRULE_1_PRED, Scope_PROD_1_SUBRULE_1_NT, strm)
      val (Exp_RES, Exp_SPAN, strm') = Exp_NT(strm')
      val FULL_SPAN = (#1(SR_SPAN), #2(Exp_SPAN))
      in
        (UserCode.Scope_PROD_1_ACT (SR_RES, Exp_RES, SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
          FULL_SPAN, strm')
      end
and ValBind_NT (strm) = let
      fun ValBind_PROD_1 (strm) = let
            val (KW_fun_RES, KW_fun_SPAN, strm') = matchKW_fun(strm)
            val (LID_RES, LID_SPAN, strm') = matchLID(strm')
            fun ValBind_PROD_1_SUBRULE_1_NT (strm) = let
                  val (FunParam_RES, FunParam_SPAN, strm') = FunParam_NT(strm)
                  val FULL_SPAN = (#1(FunParam_SPAN), #2(FunParam_SPAN))
                  in
                    ((FunParam_RES), FULL_SPAN, strm')
                  end
            fun ValBind_PROD_1_SUBRULE_1_PRED (strm) = (case (lex(strm))
                   of (Tok.LP, _, strm') => true
                    | (Tok.LB, _, strm') => true
                    | _ => false
                  (* end case *))
            val (SR_RES, SR_SPAN, strm') = EBNF.posclos(ValBind_PROD_1_SUBRULE_1_PRED, ValBind_PROD_1_SUBRULE_1_NT, strm')
            val (ARROW_RES, ARROW_SPAN, strm') = matchARROW(strm')
            val (Type_RES, Type_SPAN, strm') = Type_NT(strm')
            val (EQ_RES, EQ_SPAN, strm') = matchEQ(strm')
            val (Exp_RES, Exp_SPAN, strm') = Exp_NT(strm')
            val FULL_SPAN = (#1(KW_fun_SPAN), #2(Exp_SPAN))
            in
              (UserCode.ValBind_PROD_1_ACT (EQ_RES, SR_RES, Exp_RES, LID_RES, Type_RES, ARROW_RES, KW_fun_RES, EQ_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), LID_SPAN : (Lex.pos * Lex.pos), Type_SPAN : (Lex.pos * Lex.pos), ARROW_SPAN : (Lex.pos * Lex.pos), KW_fun_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun ValBind_PROD_2 (strm) = let
            val (KW_let_RES, KW_let_SPAN, strm') = matchKW_let(strm)
            val (SimplePat_RES, SimplePat_SPAN, strm') = SimplePat_NT(strm')
            fun ValBind_PROD_2_SUBRULE_1_NT (strm) = let
                  val (COLON_RES, COLON_SPAN, strm') = matchCOLON(strm)
                  val (Type_RES, Type_SPAN, strm') = Type_NT(strm')
                  val FULL_SPAN = (#1(COLON_SPAN), #2(Type_SPAN))
                  in
                    ((Type_RES), FULL_SPAN, strm')
                  end
            fun ValBind_PROD_2_SUBRULE_1_PRED (strm) = (case (lex(strm))
                   of (Tok.COLON, _, strm') => true
                    | _ => false
                  (* end case *))
            val (SR_RES, SR_SPAN, strm') = EBNF.optional(ValBind_PROD_2_SUBRULE_1_PRED, ValBind_PROD_2_SUBRULE_1_NT, strm')
            val (EQ_RES, EQ_SPAN, strm') = matchEQ(strm')
            val (Exp_RES, Exp_SPAN, strm') = Exp_NT(strm')
            val FULL_SPAN = (#1(KW_let_SPAN), #2(Exp_SPAN))
            in
              (UserCode.ValBind_PROD_2_ACT (EQ_RES, SR_RES, Exp_RES, SimplePat_RES, KW_let_RES, EQ_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), SimplePat_SPAN : (Lex.pos * Lex.pos), KW_let_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.KW_let, _, strm') => ValBind_PROD_2(strm)
          | (Tok.KW_fun, _, strm') => ValBind_PROD_1(strm)
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
fun Definition_NT (strm) = let
      fun Definition_PROD_1 (strm) = let
            val (KW_type_RES, KW_type_SPAN, strm') = matchKW_type(strm)
            val (UID_RES, UID_SPAN, strm') = matchUID(strm')
            fun Definition_PROD_1_SUBRULE_1_NT (strm) = let
                  val (TypeParams_RES, TypeParams_SPAN, strm') = TypeParams_NT(strm)
                  val FULL_SPAN = (#1(TypeParams_SPAN), #2(TypeParams_SPAN))
                  in
                    ((TypeParams_RES), FULL_SPAN, strm')
                  end
            fun Definition_PROD_1_SUBRULE_1_PRED (strm) = (case (lex(strm))
                   of (Tok.LB, _, strm') => true
                    | _ => false
                  (* end case *))
            val (SR_RES, SR_SPAN, strm') = EBNF.optional(Definition_PROD_1_SUBRULE_1_PRED, Definition_PROD_1_SUBRULE_1_NT, strm')
            val (EQ_RES, EQ_SPAN, strm') = matchEQ(strm')
            val (Type_RES, Type_SPAN, strm') = Type_NT(strm')
            val FULL_SPAN = (#1(KW_type_SPAN), #2(Type_SPAN))
            in
              (UserCode.Definition_PROD_1_ACT (EQ_RES, SR_RES, UID_RES, Type_RES, KW_type_RES, EQ_SPAN : (Lex.pos * Lex.pos), SR_SPAN : (Lex.pos * Lex.pos), UID_SPAN : (Lex.pos * Lex.pos), Type_SPAN : (Lex.pos * Lex.pos), KW_type_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun Definition_PROD_2 (strm) = let
            val (KW_data_RES, KW_data_SPAN, strm') = matchKW_data(strm)
            val (UID_RES, UID_SPAN, strm') = matchUID(strm')
            fun Definition_PROD_2_SUBRULE_1_NT (strm) = let
                  val (TypeParams_RES, TypeParams_SPAN, strm') = TypeParams_NT(strm)
                  val FULL_SPAN = (#1(TypeParams_SPAN), #2(TypeParams_SPAN))
                  in
                    ((TypeParams_RES), FULL_SPAN, strm')
                  end
            fun Definition_PROD_2_SUBRULE_1_PRED (strm) = (case (lex(strm))
                   of (Tok.LB, _, strm') => true
                    | _ => false
                  (* end case *))
            val (SR1_RES, SR1_SPAN, strm') = EBNF.optional(Definition_PROD_2_SUBRULE_1_PRED, Definition_PROD_2_SUBRULE_1_NT, strm')
            val (KW_with_RES, KW_with_SPAN, strm') = matchKW_with(strm')
            fun Definition_PROD_2_SUBRULE_2_NT (strm) = let
                  val (ConDef_RES, ConDef_SPAN, strm') = ConDef_NT(strm)
                  val FULL_SPAN = (#1(ConDef_SPAN), #2(ConDef_SPAN))
                  in
                    ((ConDef_RES), FULL_SPAN, strm')
                  end
            fun Definition_PROD_2_SUBRULE_2_PRED (strm) = (case (lex(strm))
                   of (Tok.KW_con, _, strm') => true
                    | _ => false
                  (* end case *))
            val (SR2_RES, SR2_SPAN, strm') = EBNF.posclos(Definition_PROD_2_SUBRULE_2_PRED, Definition_PROD_2_SUBRULE_2_NT, strm')
            val FULL_SPAN = (#1(KW_data_SPAN), #2(SR2_SPAN))
            in
              (UserCode.Definition_PROD_2_ACT (SR1_RES, SR2_RES, UID_RES, KW_data_RES, KW_with_RES, SR1_SPAN : (Lex.pos * Lex.pos), SR2_SPAN : (Lex.pos * Lex.pos), UID_SPAN : (Lex.pos * Lex.pos), KW_data_SPAN : (Lex.pos * Lex.pos), KW_with_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      fun Definition_PROD_3 (strm) = let
            val (ValBind_RES, ValBind_SPAN, strm') = ValBind_NT(strm)
            val FULL_SPAN = (#1(ValBind_SPAN), #2(ValBind_SPAN))
            in
              (UserCode.Definition_PROD_3_ACT (ValBind_RES, ValBind_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
                FULL_SPAN, strm')
            end
      in
        (case (lex(strm))
         of (Tok.KW_fun, _, strm') => Definition_PROD_3(strm)
          | (Tok.KW_let, _, strm') => Definition_PROD_3(strm)
          | (Tok.KW_type, _, strm') => Definition_PROD_1(strm)
          | (Tok.KW_data, _, strm') => Definition_PROD_2(strm)
          | _ => fail()
        (* end case *))
      end
fun Program_NT (strm) = let
      fun Program_PROD_1_SUBRULE_1_NT (strm) = let
            val (Definition_RES, Definition_SPAN, strm') = Definition_NT(strm)
            val (SEMI_RES, SEMI_SPAN, strm') = matchSEMI(strm')
            val FULL_SPAN = (#1(Definition_SPAN), #2(SEMI_SPAN))
            in
              ((Definition_RES), FULL_SPAN, strm')
            end
      fun Program_PROD_1_SUBRULE_1_PRED (strm) = (case (lex(strm))
             of (Tok.KW_data, _, strm') => true
              | (Tok.KW_fun, _, strm') => true
              | (Tok.KW_let, _, strm') => true
              | (Tok.KW_type, _, strm') => true
              | _ => false
            (* end case *))
      val (SR_RES, SR_SPAN, strm') = EBNF.closure(Program_PROD_1_SUBRULE_1_PRED, Program_PROD_1_SUBRULE_1_NT, strm)
      val (Exp_RES, Exp_SPAN, strm') = Exp_NT(strm')
      val FULL_SPAN = (#1(SR_SPAN), #2(Exp_SPAN))
      in
        (UserCode.Program_PROD_1_ACT (SR_RES, Exp_RES, SR_SPAN : (Lex.pos * Lex.pos), Exp_SPAN : (Lex.pos * Lex.pos), FULL_SPAN : (Lex.pos * Lex.pos)),
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
