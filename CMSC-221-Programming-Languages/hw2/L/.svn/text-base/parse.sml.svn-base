structure Parse : PARSE = struct
  exception ParseError of string

  (* Mutually Recursive function that gets the next token in a token list
     producing a tuple (AST.Exp, Tokenlist) *)
  fun buildAST tokList = plusLETExp (getExp tokList)
  and plusLETExp (exp1, toklist) =
      (case toklist
         of Token.Comma::rest => let
                                   val (exp2,tklist) = getExp rest
                                 in
                                   (AST.Plus(exp1,exp2),tklist) 
                                 end 
          | Token.LArrow::rest => let
                                    val (exp2, tklist) = getExp rest
                                    val (exp3, tklist2) = getExp (List.tl tklist)
                                  in
                                    (case exp1 of AST.Var s => (AST.Let(s,exp2,exp3),tklist2)
                                                | _ => raise ParseError "LET exp malformed")
                                  end
          | _  => (exp1,toklist))
  and getExp toklist =
      (case toklist
         of Token.Int i::rest => (AST.Int i,rest)
          | Token.ID s::rest => (AST.Var s,rest)
          | Token.Plus::Token.LParen::rest => let
                                                val (plsExp,tklist) = buildAST(rest)
                                              in 
                                                (case tklist
                                                   of Token.RParen::rst => (plsExp,rst)
                                                    | _ => raise ParseError "expected LParen for Plus expression")
                                              end
          | Token.Let::Token.LParen::rest => let
                                               val (ltExp,tklist) = buildAST(rest)
                                             in
                                               (case tklist
                                                  of Token.RParen::rst => (ltExp,rst)
                                                   | _ => raise ParseError "expected LParen for LET exp")
                                             end
          | _ => raise ParseError "malformed expression")
                                              
                                                  
  fun parse (ts : Token.token list) : AST.exp =
    (case (buildAST ts)
       of (exp, []) => exp
        | _         => raise ParseError "malformed expression")
end





