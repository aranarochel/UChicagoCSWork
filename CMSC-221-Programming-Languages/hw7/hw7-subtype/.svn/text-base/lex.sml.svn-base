structure Lex : LEX = struct

  exception SyntaxError of string
  exception Bug of string

  structure T = Token
 
  (* gather : ('a -> pred) -> 'a list -> ('a list * 'a list)
   * Gather items from the *front* of the list, per the predicate.
   * Split the list according to the predicate like so:
   * gather isOdd [1,3,4,5,6] --> ([1,3],[4,5,6]) 
   *)
  fun gather test xs = let
    fun lp ([], acc) = (rev acc, [])
      | lp (h::t, acc) = if test(h) 
			 then lp (t, h::acc) 
			 else (rev acc, h::t)
    in
      lp (xs, [])
    end

  fun buildString (c, cs) = 
    let val (ds, ds') = gather Char.isLower cs
    in (implode (c::ds), ds')
    end

  (* lexical scanning implementation *)

  fun nextToken (cs : char list) : (Token.token * char list) option = let
    fun tok (cs : char list) = 
     (case cs
       of [] => NONE
	| #"T" :: #"o" :: #"p" :: t => SOME (T.Top, t)
	| #"R" :: #"e" :: #"f" :: t => SOME (T.Ref, t)
        | #"L" :: #"i" :: #"s" :: #"t" :: t => SOME (T.List, t)
	| #"(" :: t => SOME (T.LPAR, t)
	| #")" :: t => SOME (T.RPAR, t)
	| #"{" :: t => SOME (T.LBRACE, t)
	| #"}" :: t => SOME (T.RBRACE, t)
	| #"*" :: t => SOME (T.STAR, t)
	| #"+" :: t => SOME (T.PLUS, t)
	| #"-" :: #">" :: t => SOME (T.ARROW, t)
	| #"," :: t => SOME (T.COMMA, t)
	| #":" :: t => SOME (T.COLON, t)
	| _ => tok' cs
     (* end case *))
    and tok' cs =
     (case cs
       of h::t => if Char.isAlpha h then 
		    let 
                      val (s, cs') = buildString (h, t)
		      val k = if Char.isUpper h
			      then T.TypeName
			      else T.Label
		    in 
                      SOME (k s, cs')
		    end
		  else if Char.isSpace h then tok t 
		  else raise SyntaxError (implode cs)
	| _ => raise Bug "in tok'"
     (* end case *))
    in  
      tok cs
    end

  fun untilNone f xs =
    let
      fun lp ([], acc) = List.rev acc
	| lp (xs, acc) =
           (case f xs
	     of NONE => List.rev acc
	      | SOME (y, xs') => lp (xs', y::acc)
	   (* end case *))
    in
      lp (xs, [])
    end

  val lex = untilNone nextToken o explode

end
