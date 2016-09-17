structure Parse = struct

  structure T = Token

  structure A = AST

  fun tos toks = String.concatWith "," (List.map T.tos toks)

  fun parse' ts =
   (case ts
     of [] => NONE
      | T.LParen::ts' => SOME (parseThen (T.RParen, ts'))
      | T.True::ts' => SOME (A.True, ts')
      | T.False::ts' => SOME (A.False, ts')
      | T.Zero::ts' => SOME (A.Zero, ts')
      | T.Succ::ts' => arg A.Succ ts'
      | T.Pred::ts' => arg A.Pred ts'
      | T.IsZero::ts' => arg A.IsZero ts'
      | T.If::ts' => let
          val (tm1, ts1) = parseThen (T.Then, ts')
          val (tm2, ts2) = parseThen (T.Else, ts1)
          in
           (case parse' ts2
             of SOME (tm3, ts3) => SOME (A.If (tm1, tm2, tm3), ts3)
              | NONE => raise Fail ("parse error: " ^ tos ts)
           (* end case *))
        end
      | _ => raise Fail ("parse error: " ^ tos ts)
   (* end case *))
  and arg c toks =
   (case parse' toks
     of SOME (tm, toks') => SOME (c tm, toks')
      | NONE => raise Fail ("parse error: " ^ tos toks)
   (* end case *))
  and parseThen (tok, toks) = let
    fun err ts = 
	Fail (concat ["parse error: expected ", T.tos tok, " at token" ,
                      if List.length ts = 1 then " " else "s ", tos ts])

    in
      case parse' toks
       of SOME (tm, toks') =>
           (case toks'
             of t::ts => if T.eq (tok, t)
                         then (tm, ts)
                         else raise (err toks')
              | [] => raise (err toks)
           (* end case *))
        | NONE => raise (err toks)
    end

  fun parse [] = raise Fail ("parse error: empty program")
    | parse toks =
       (case parse' toks
         of SOME (tm, []) => tm
          | SOME (tm, ts) => raise Fail ("parse error: extra tokens at end: " ^ tos ts)
          | NONE => raise Fail ("parse error: " ^ tos toks)
       (* end case *))

end

