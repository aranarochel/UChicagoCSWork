structure Optimize : OPTIMIZE = struct
  (* Optimization pass is not implemented due to buggy Optimize code *)
  fun member (s, exp) =
      (case exp
         of ScopedAST.Var (str,stmp) => s = str
          | ScopedAST.Let(s,stmp,e1,e2) => member(s,e1) orelse member(s,e2)
          | ScopedAST.Plus (e1,e2) => member(s,e1) orelse member (s,e2)
          | ScopedAST.LNeg (e) => member(s,e)
          | ScopedAST.IsZero(e) => member(s,e)
          | ScopedAST.LessThan(e1,e2) => member(s,e1) orelse member(s,e2)
          | ScopedAST.If(e1,e2,e3) => member(s,e1) orelse member(s,e2) orelse member(s,e3)
          | _ => false)


  fun optimize (t : ScopedAST.exp) : ScopedAST.exp = 
      (case t
         of ScopedAST.Let(s,stmp,e1,e2) => if member(s,e2) then ScopedAST.Let(s,stmp,e1,optimize e2)
                                                           else optimize e2
          | ScopedAST.Plus(e1,e2) => ScopedAST.Plus(optimize e1,optimize e2)
          | ScopedAST.LNeg e => ScopedAST.LNeg (optimize e)
          | ScopedAST.IsZero e => ScopedAST.IsZero (optimize e)
          | ScopedAST.LessThan (e1,e2) => ScopedAST.LessThan (optimize e1, optimize e2)
          | ScopedAST.If (e1,e2,e3) => ScopedAST.If (optimize e1, optimize e2, optimize e3)
          | x => x)
end
