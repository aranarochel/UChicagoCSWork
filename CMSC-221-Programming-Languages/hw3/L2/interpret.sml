structure Interpret : INTERPRET = struct
  val interpret = Eval.eval o Scope.scope o Parse.parse o Lex.lex
  (* How awesome is that? *) 
end
