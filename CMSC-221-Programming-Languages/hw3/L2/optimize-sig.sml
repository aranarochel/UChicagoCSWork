signature OPTIMIZE = sig
  val member : string * ScopedAST.exp -> bool
  val optimize : ScopedAST.exp -> ScopedAST.exp
end
