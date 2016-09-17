structure Labels : LABELS = struct

  exception DuplicateLabel of string

  fun dups [] = false
    | dups ((a,b)::xs) = (List.exists (fn (x,y) => (a=x)) xs) orelse (dups xs)

  fun apply (f,xs) =
   (case xs of
         [] => []
       | (lbl,tp)::xss => (lbl,(f tp))::(apply(f,xss)));


  fun check ty = 
    (case ty
       of Ty.Top => Ty.Top
        | Ty.Base s => Ty.Base s
        | Ty.Record (recs) => if dups(recs) 
                              then raise DuplicateLabel "found duplicate label"
                              else Ty.Record(apply(check,recs))
        | Ty.Ref (rf) => Ty.Ref(check rf)
        | Ty.List (lst) => Ty.List(check lst)
        | Ty.Prod (p1,p2) => Ty.Prod(check p1, check p2)
        | Ty.Sum (s1,s2) => Ty.Sum(check s1,check s2)
        | Ty.Fun (f1,f2) => Ty.Fun(check f1,check f2))  

end
