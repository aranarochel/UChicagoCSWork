structure Lex = struct

  structure T = Token

  fun lex' cs =
   (case cs
     of [] => NONE
      | #"t":: #"r":: #"u":: #"e"::cs' => SOME (T.True, cs')
      | #"f":: #"a":: #"l":: #"s":: #"e"::cs' => SOME (T.False, cs')
      | #"i":: #"f"::cs' => SOME (T.If, cs')
      | #"t":: #"h":: #"e":: #"n"::cs' => SOME (T.Then, cs')
      | #"e":: #"l":: #"s":: #"e"::cs' => SOME (T.Else, cs')
      | #"0"::cs' => SOME (T.Zero, cs')
      | #"s":: #"u":: #"c":: #"c"::cs' => SOME (T.Succ, cs')
      | #"p":: #"r":: #"e":: #"d"::cs' => SOME (T.Pred, cs')
      | #"i":: #"s":: #"z":: #"e":: #"r":: #"o"::cs' => SOME (T.IsZero, cs')
      | #"("::cs' => SOME (T.LParen, cs')
      | #")"::cs' => SOME (T.RParen, cs')
      | c::cs' => if Char.isSpace c then lex' cs'
                  else raise Fail ("syntax error: " ^ implode cs))

  fun lex s = let
    fun lp ([], acc) = rev acc
      | lp (cs, acc) =
         (case lex' cs
           of NONE => rev acc
            | SOME (t, cs') => lp (cs', t::acc)
         (* end case *))
    in
      lp (explode s, [])
    end

end
