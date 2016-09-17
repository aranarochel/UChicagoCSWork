structure Parse : PARSE = struct
  exception ParseError of string

  (* Mutually Recursive function that gets the next token in a token list
     producing a tuple (AST.Exp, Tokenlist) *)
  fun buildAST tokList = buildExp (getExp tokList)
  and buildExp (exp1, toklist) =
      (case toklist
         of Token.Comma::rest => 
	    let
              val (exp2,tklist) = getExp rest
            in
              (AST.Plus(exp1,exp2),tklist) 
            end 
          | Token.LArrow::rest => 
	    let
              val (exp2, tklist) = getExp rest
              val (exp3, tklist2) = (case tklist of
                                          Token.Comma::rest => getExp (rest)
                                         | _ => raise ParseError "LET malformed")
            in
              (case exp1 of AST.Var s => (AST.Let(s,exp2,exp3),tklist2)
                  | _ => raise ParseError "LET exp malformed")
            end
          | _  => (exp1,toklist))
  and getExp toklist =
      (case toklist
         of Token.Int i::rest => (AST.Int i,rest)
          | Token.ID s::rest => (AST.Var s,rest)
          | Token.True::rest => (AST.True,rest)
          | Token.False::rest => (AST.False,rest)
          | Token.Plus::Token.LParen::rest => 
	    let
              val (plsExp,tklist) = buildAST(rest)
            in 
              (case tklist
                 of Token.RParen::rst => (plsExp,rst)
                  | _ => raise ParseError "expected LParen for Plus expression")
            end
           
          | Token.Let::Token.LParen::rest => 
	    let
              val (ltExp,tklist) = buildAST(rest)
            in
              (case tklist
                 of Token.RParen::rst => (ltExp,rst)
                  | _ => raise ParseError "expected LParen for LET exp")
            end
          | Token.LessThan::Token.LParen::rest => 
            let
              val (lessExp1,tklist) = getExp rest
              val (lessExp2,tklist2) = (case tklist of
                                             Token.Comma::rest => getExp(rest)
                                           | _ => raise ParseError "<(t) malformed")
            in
              (case tklist2 of
                    Token.RParen::rst => (AST.LessThan(lessExp1,lessExp2),rst)
                  | _ => raise ParseError "malformed <(t) exp")
            end
          | Token.LNeg::Token.LParen::rest => 
            let
              val (negExp,tklist) = getExp rest
            in
              (case tklist of
                    Token.RParen::rst => (AST.LNeg negExp,rst)
                  | _ => raise ParseError "malformed ~(t) exp")
            end
          | Token.IsZero::Token.LParen::rest =>
            let
              val (zeroExp,tklist) = getExp rest
            in (case tklist of
                     Token.RParen::rst => (AST.IsZero zeroExp,rst)
                   | _ => raise ParseError "malformed Z(t) exp")
            end
          | Token.If::Token.LParen::rest =>
            let
              val (ifExp1,tklist) = getExp rest
              val (ifExp2,tklist2) = (case tklist of
                                           Token.Comma::rest => getExp(rest)
                                         | _ => raise ParseError "If() exp malformed")
              val (ifExp3,tklist3) = (case tklist2 of
                                           Token.Comma::rest => getExp(rest)
                                         | _ => raise ParseError "If() exp malformed")
            in
              (case tklist3 of
                    Token.RParen::rst => (AST.If (ifExp1,ifExp2,ifExp3),rst)
                  | _ => raise ParseError "malformed IF() exp")
            end
          | _ => raise ParseError "malformed expression")
                                              
                                                  
  fun parse (ts : Token.token list) : AST.exp =
    (case (buildAST ts)
       of (exp, []) => exp
        | _         => raise ParseError "malformed expression")
end





