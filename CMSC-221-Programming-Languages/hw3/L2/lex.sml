structure Lex : LEX = struct

  exception SyntaxError of string
  
  (* Helper Functions *)
  (* scanforID : char list * char list -> char list * string
     iterates through cs until it meets a white space or end of the char list,
     cons'ing the characters onto id with every iteration and returning it along with
     the char list so far *)
  fun scanforID (cs, id) =
    (case cs
       of c::rest => if Char.isAlphaNum c
                     then scanforID (rest, c::id)
                     else (cs, implode(rev id))
        | [] => (cs, implode (rev id)))

  fun scanforInt (cs, num) = 
   (case cs
      of n::rest => if Char.isDigit n
                    then scanforInt(rest, n::num)
                    else (cs, implode(rev num))
       | [] => (cs, implode (rev num)))
 
  (* hint #1: unless you have a better idea, write this: *)
  fun nextToken (cs : char list) : (Token.token * char list) option =
    (case cs
       of [] => NONE
        | #"T" ::rest => SOME (Token.True,rest)
        | #"F" ::rest => SOME (Token.False,rest)
        | #"~" ::rest => SOME (Token.LNeg,rest)
        | #"Z" ::rest => SOME (Token.IsZero,rest)
        | #"<" :: #"-" ::rest => SOME (Token.LArrow,rest)
        | #"<" ::rest => SOME (Token.LessThan,rest)
        | #"(" ::rest => SOME (Token.LParen,rest)
        | #")" ::rest => SOME (Token.RParen,rest)
        | #"+" ::rest => SOME (Token.Plus,rest)
        | #"," ::rest => SOME (Token.Comma,rest)
        | c::rest => if Char.isSpace c then nextToken rest
                     else if Char.isAlpha c 
			then let 
                          val (css,id) = scanforID (rest,[c])
                        in
                          if id = "LET" then SOME (Token.Let,css)
                          else if id = "IF" then SOME (Token.If,css)
                          else  SOME (Token.ID id,css)
                        end
                     else if Char.isDigit c
                        then let
                          val (css, num) = scanforInt (rest,[c])
                        in 
		          SOME (Token.Int (floor(valOf(Real.fromString num))),css)
                        end
                     else raise SyntaxError("Warning:illegal tokens"))  
                       

  (* -- ponder the type of this function! *)
 
  (* hint #2: use the built-in operation *)
  (*   explode : string -> char list *)
  (* before you start scanning *)

  fun lex (s : string) : Token.token list = let
    fun lx cs =
      (case nextToken(cs)
         of NONE => []
          | SOME (a,rest)  => a::lx(rest))
    in 
     lx (explode s)
    end






end
