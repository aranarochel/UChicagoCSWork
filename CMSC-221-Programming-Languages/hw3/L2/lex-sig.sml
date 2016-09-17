signature LEX = sig
  exception SyntaxError of string
  val scanforID : char list * char list -> char list * string
  val scanforInt : char list * char list -> char list * string
  val nextToken : char list -> (Token.token * char list) option
  val lex : string -> Token.token list
end
