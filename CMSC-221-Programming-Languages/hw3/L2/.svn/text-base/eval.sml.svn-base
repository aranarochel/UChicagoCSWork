structure Eval : EVAL = struct
  exception RuntimeError of string
  type value = int
  type env = (ScopedAST.stamp * Value.value) list
  fun eval (e : ScopedAST.exp) : Value.value = let

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
         of ScopedAST.Int i => Value.Int i
          | ScopedAST.True => Value.True
          | ScopedAST.False => Value.False
          | ScopedAST.Var (str,stmp) => fetch_value(stmp,env)
          | ScopedAST.Let (s,st,e1,e2) => let val v = lp(env, e1)
                                          in 
	  				    lp(env@[(st,v)],e2)
   			                  end
	  | ScopedAST.Plus (ex1,ex2) => let
                                          val e1 = lp(env,ex1) 
                                          val e2 = lp(env, ex2)
                                        in
                                          (case (e1,e2) of
                                                (Value.Int n,Value.Int m) => Value.Int (n+m)
                                              | _ => raise RuntimeError "didn't add 2 ints")
                                        end
          | ScopedAST.LNeg e => (case lp(env,e) of
                                      Value.True => Value.False
                                    | Value.False => Value.True
                                    | _ => raise RuntimeError "can only negate booleans")
          | ScopedAST.IsZero e => (case lp(env,e) of
                                        Value.Int x => if x=0 then Value.True else Value.False
                                      | _ => raise RuntimeError "Z(t) needs an int arg")
          | ScopedAST.LessThan (e1,e2) => (case (lp(env,e1),lp(env,e2)) of
                                                (Value.Int n,Value.Int m) => 
                                                   if n < m then Value.True else Value.False
                                               | _ => raise RuntimeError "<(t,t) requires int args")
          | ScopedAST.If (e1,e2,e3) => (case (lp(env,e1),lp(env,e2),lp(env,e3))
                                          of (Value.True, x, _) => x
                                           | (Value.False, _, y) => y
                                           | _ => raise RuntimeError "IF exp requires a boolean conditional"))
    in
      lp([],e)
    end
end
