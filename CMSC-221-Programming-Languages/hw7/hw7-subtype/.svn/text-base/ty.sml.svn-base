structure Ty = struct

  datatype ty
    = Top
    | Base of string
    | Record of (string * ty) list
    | Ref of ty
    | List of ty
    | Prod of ty * ty
    | Sum of ty * ty
    | Fun of ty * ty

  fun tos ty =
    let
      val spaces = String.concatWith " "
      val commas = String.concatWith ","
      fun lp Top = "Top"
	| lp (Base s) = s
	| lp (Record ps) = record ps
	| lp (Ref t) = lp' ("Ref", [t])
	| lp (List t) = lp' ("List", [t])
	| lp (Prod (t1, t2)) = lp' ("*", [t1,t2])
	| lp (Sum (t1, t2)) = lp' ("+", [t1,t2])
	| lp (Fun (t1, t2)) = lp' ("->", [t1,t2])
      and lp' (tag, ts) = 
        concat ["(", spaces (tag::(map lp ts)), ")"]
      and record ps = concat ["{", commas (map r ps), "}"]
      and r (lbl, t) = concat [lbl, ":", lp t]
    in
      lp ty
    end

  fun eq (t:ty, u) = t=u

end
