signature FLANG_LEXER =
  sig
    exception SyntaxError
    val lexer : ((char, 'a) StringCvt.reader)
          -> (FLangTokens.token, 'a) StringCvt.reader

  end;


structure FLangLex : FLANG_LEXER = struct

  exception SyntaxError


  (* scanforID : (char, 'a) reader -> 'a -> char list -> ('a * string) *)
  (* Returns a tuple of the string representation of the ID and the rest of the strm *)
  fun scanforID (getc, strm, id) = 
    (case (getc strm)
       of SOME (c, rest) => if Char.isAlphaNum c orelse c = #"_" orelse c = #"'"
			    then scanforID (getc, rest, c::id)
			    else (strm, implode(rev id))
	| NONE => (strm, implode (rev id))
	(* end case *))


  (* same as above but returns an int instead *)
  fun scanforInt (getc, strm, num) =
    (case (getc strm)
       of SOME (n, rest) => if Char.isDigit n 
                            then scanforInt (getc, rest, n::num)
			    else (strm, valOf(Int.fromString(implode (rev num))))
	| NONE => (strm, valOf(Int.fromString(implode (rev num))))
	(* end case *)) 


  (* like above but returns an error upon not reaching the matching double quote *)
  fun scanforString (getc, strm, sr) =
    (case (getc strm)
       of SOME (c, rest) => if c = #"\"" 
			    then (rest ,implode(rev(sr)))
			    else scanforString (getc, rest, c::sr)
	| NONE => raise SyntaxError
	(* end case *))
    


  (* this function iterates through strm until it reaches the end of comment,
     and returns the rest of the strm after the comment *)
  fun delComment (getc, strm) = 
    (case (getc strm)
       of SOME (#"*", rest) => (case getc(rest)
			          of SOME (#")",rest') => rest'
				   | _ => delComment (getc, rest))
	| SOME (c, rest) => delComment (getc, rest)
	| NONE => raise SyntaxError
	(* end case *)) 


  (* keywordMatch : string -> FLangTokens.Token *)
  (* Checks if the identifier matches any reserved keyword.
     If not then it return either a UID or LID *)
  fun keywordMatch sr = 
    (case sr
       of "case" => FLangTokens.KW_case 
	| "con" => FLangTokens.KW_con
	| "data" => FLangTokens.KW_data
	| "else" => FLangTokens.KW_else
	| "end" => FLangTokens.KW_end
	| "fun" => FLangTokens.KW_fun
	| "if" => FLangTokens.KW_if
	| "let" => FLangTokens.KW_let
	| "of" => FLangTokens.KW_of
	| "then" => FLangTokens.KW_then
	| "type" => FLangTokens.KW_type
	| "with" => FLangTokens.KW_with
	| _      => if Char.isLower(List.hd (explode sr))
		    then FLangTokens.LID (Atom.atom sr)
		    else FLangTokens.UID (Atom.atom sr)
	(* end case *))

  
  fun lexer getc strm = 
    (case (getc (StringCvt.skipWS getc strm))
       of NONE => NONE
	| SOME (#"(", rest) => (case (getc rest)
				  of SOME (#"*",rest') => lexer getc (delComment (getc, rest'))  
				   | _ => SOME (FLangTokens.LP, rest))
	| SOME (#")", rest) => SOME (FLangTokens.RP, rest)
	| SOME (#"[", rest) => SOME (FLangTokens.LB, rest)
	| SOME (#"]", rest) => SOME (FLangTokens.RB, rest)
	| SOME (#"{", rest) => SOME (FLangTokens.LCB, rest)
	| SOME (#"}", rest) => SOME (FLangTokens.RCB, rest)
	| SOME (#"=", rest) => (case (getc rest)
				  of SOME (#"=", rest') => SOME (FLangTokens.EQEQ,rest')
				   | SOME (#">", rest') => SOME (FLangTokens.DARROW,rest')
                                   | _ => SOME (FLangTokens.EQ,rest)
				   (*end case*))
	| SOME (#"<", rest) => (case (getc rest)
				  of SOME (#"=", rest') => SOME (FLangTokens.LTEQ,rest')
				   | _ => SOME (FLangTokens.LT,rest)
				   (*end case*))
	| SOME (#":", rest) => (case (getc rest)
				  of SOME (#":", rest') => SOME (FLangTokens.DCOLON,rest')
				   | _ => SOME (FLangTokens.COLON,rest)
				   (*end case*))
	| SOME (#"@", rest) => SOME (FLangTokens.AT, rest)
	| SOME (#"+", rest) => SOME (FLangTokens.PLUS, rest)
	| SOME (#"-", rest) => (case (getc rest)
				  of SOME (#">", rest') => SOME (FLangTokens.ARROW,rest')
				   | _ => SOME (FLangTokens.MINUS,rest)
				   (*end case*))
	| SOME (#"*", rest) => SOME (FLangTokens.TIMES, rest)
	| SOME (#"/", rest) => SOME (FLangTokens.DIV, rest)
	| SOME (#",", rest) => SOME (FLangTokens.COMMA, rest)
	| SOME (#";", rest) => SOME (FLangTokens.SEMI, rest)
	| SOME (#"_", rest) => SOME (FLangTokens.WILD, rest)
	| SOME (#"\"",rest) => let
				   val (rest', sr) = scanforString (getc, rest, [])
  			       in
                                   SOME (FLangTokens.STRING sr, rest') end
	| SOME (c, rest)    => if Char.isAlpha c then 
			          let
				      val (rest', id) = scanforID (getc, rest, [c])
				      val tok = keywordMatch id
			          in
			              SOME (tok, rest') end
				
			       else if Char.isDigit c then
				  let
				      val (rest', num) = scanforInt (getc, rest, [c])
				  in
				      SOME (FLangTokens.NUMBER (Int.toLarge num), rest') end
			       else raise SyntaxError  
	(* end case *))
	
end  
