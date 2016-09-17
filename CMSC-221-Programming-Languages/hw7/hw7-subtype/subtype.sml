structure Subtype = struct

  exception noLabel

  fun list_assoc (lbl, xs) = 
    (case xs
       of [] => raise noLabel
        | (lb,tp)::xss => if lbl = lb 
                          then tp
                          else list_assoc(lbl,xss))


  fun subtype (ty1, ty2) = 
    (ty1 = ty2) orelse
    (case (ty1, ty2) 
       of (_,Ty.Top) => true
        | (Ty.Ref s1, Ty.Ref t1) => subtype(s1,t1) andalso subtype(t1,s1)
        | (Ty.List s1, Ty.List t1) => subtype(s1,t1)
        | (Ty.Prod(s1,s2), Ty.Prod(t1,t2)) => subtype(s1,t1) andalso subtype(s2,t2)
        | (Ty.Sum(s1,s2), Ty.Sum(t1,t2)) => subtype(s1,t1) andalso subtype(s2,t2)
        | (Ty.Fun(s1,s2), Ty.Fun(t1,t2)) => subtype(t1,s1) andalso subtype(s2,t2)
        | (Ty.Record rec1, Ty.Record rec2) =>
            List.all (fn (lbl,t1) => (let 
                                         val s1 = list_assoc(lbl,rec1)
                                      in 
                                         subtype(s1,t1) end) handle noLabel => false)
                     rec2
	| (_,_) => false) 

end
