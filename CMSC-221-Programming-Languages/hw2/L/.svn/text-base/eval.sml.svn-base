structure Eval : EVAL = struct
  type value = int
  type env = (ScopedAST.stamp * value) list
  fun eval (e : ScopedAST.exp) : value = let

    fun member (x,es) =
     (case (x,es)
        of (x,nil) => false
         | (x,(stmp,vl)::es) => x=stmp orelse member(x,es))

    fun fetch_value (x,es) =
     (case (x,es)
        of (x,nil) => raise Fail "found no value"
         | (x,(stmp,vl)::rest) => if x=stmp then vl else fetch_value(x,rest))     

    fun lp (env, e) = 
      (case e
         of ScopedAST.Int i => i
          | ScopedAST.Var (str,stmp) => fetch_value(stmp,env)
          | ScopedAST.Let (s,st,e1,e2) => let val v = lp(env, e1)
                                          in 
	  				    lp(env@[(st,v)],e2)
   			                  end
	  | ScopedAST.Plus (ex1,ex2) => lp(env,ex1) + lp(env, ex2))
    in
      lp([],e)
    end
end
