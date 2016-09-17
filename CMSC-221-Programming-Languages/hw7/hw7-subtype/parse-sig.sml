signature PARSE = sig
  exception ParseError of string
  val parse : Token.token list -> Ty.ty
end
