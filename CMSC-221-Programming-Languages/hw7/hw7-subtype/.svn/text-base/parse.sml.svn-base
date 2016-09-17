structure Parse : PARSE = struct

  exception ParseError of string

  structure T = Token

  val toksStr = String.concatWith " " o List.map T.tos
  val err = ParseError o toksStr

  fun nextTy tokens =
    let 
      fun lp (T.Top::ts) = SOME (Ty.Top, ts)
	| lp (T.TypeName(s)::ts) = SOME (Ty.Base s, ts)
	| lp (T.LBRACE::ts) = record ts
	| lp (T.LPAR::ts) = 
	   (case ts
	     of (T.Ref::ts)   => lp1 Ty.Ref ts
	      | (T.List::ts)  => lp1 Ty.List ts
	      | (T.STAR::ts)  => lp2 Ty.Prod ts
	      | (T.PLUS::ts)  => lp2 Ty.Sum ts
	      | (T.ARROW::ts) => lp2 Ty.Fun ts
	      | _ => raise (err ts))
	| lp ts = raise (err ts)
      and record ts = 
        let 
          fun rlp (T.Label(lbl)::T.COLON::ts, acc) =
               (case lp ts
		 of SOME (ty, T.RBRACE::ts') =>
		      SOME (Ty.Record (rev ((lbl,ty)::acc)), ts')
		  | SOME (ty, T.COMMA::ts') => 
		      rlp (ts',(lbl,ty)::acc)
		  | _ => raise (err ts))
	    | rlp (ts, _) = raise (err ts)
	in
	  case ts
	   of T.RBRACE::ts' => SOME (Ty.Record [], ts')
	    | _ => rlp (ts, [])
	end
      and lp1 c ts =
        let val (t', ts') = next1 ts
        in SOME (c t', ts') 
	end
      and lp2 c ts =
        let val (t1, t2, ts') = next2 ts
	in SOME (c (t1, t2), ts')
	end
      and next1 ts = 
       (case lp ts
	 of SOME (t', T.RPAR::ts') => (t', ts')
	  | _ => raise (err ts))
      and next2 ts =
       (case lp ts
	 of SOME (t1, ts1) =>
             (case lp ts1
	       of SOME (t2, T.RPAR::ts2) => (t1, t2, ts2)
		| _ => raise (err ts1))
	  | _ => raise (err ts))
    in
      lp tokens
    end

  fun parse (ts : Token.token list) : Ty.ty =
   (case ts
     of [] => raise ParseError "the null program"
      | _ => 
        (case nextTy ts
	  of SOME (e, []) => e
	   | SOME (e, _) => raise ParseError "extra token(s)"
	   | NONE => raise (err ts)
	(* end case *))
   (* end case *))

end
