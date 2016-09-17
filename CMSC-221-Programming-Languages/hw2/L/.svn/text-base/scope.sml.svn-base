structure Scope : SCOPE = struct
  exception UnboundVariable of string
  type env = (string * ScopedAST.stamp) list
  fun scope (t : AST.exp) : ScopedAST.exp = let
    val s = ref 0 (* hint! *)

    fun member (x,ts) =
     (case (x, ts)
        of (x,nil) => false
         | (x, (sr,st)::ts) => x=sr orelse member(x,ts))

   fun fetch_stamp (x,ts) =
    (case (x,ts)
       of (x,nil) => raise Fail "found no stamp"
        | (x, (sr,st)::rest) => if x=sr then st else fetch_stamp(x,rest)) 

    fun lp (env, t) = 
      (case t
         of AST.Int i => ScopedAST.Int i
          | AST.Var sr => (case env of
                                [] => raise UnboundVariable "unbound var"
                              | _ => if member(sr,List.rev(env)) 
                                     then ScopedAST.Var (sr,fetch_stamp(sr,List.rev(env)))
                                     else raise UnboundVariable "unbound variable")
          | AST.Let (sr,e1,e2) => (case env of
                                        [] => ScopedAST.Let (sr,(!s+1),lp(env@[("",(!s+1))],e1),
                                                           lp(env@[(sr,(!s+1))],e2))
                                      | _ => let val (str,stmp) = List.last(env)
                                             in ScopedAST.Let(sr,stmp+1,lp(env@[(sr,stmp+1)],e1),
                                                                   lp(env@[(sr,stmp+1)],e2))
                                             end)
          | AST.Plus (e1,e2) =>  ScopedAST.Plus(lp(env,e1),lp(env,e2)))
    in
      lp ([],t)
    end
end
