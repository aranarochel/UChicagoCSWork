structure AST = struct

  datatype exp 
    = True
    | False
    | Zero
    | Succ of exp
    | Pred of exp
    | IsZero of exp
    | If of exp * exp * exp

  fun eq (e1:exp, e2) = e1=e2

  fun tos e = let
    fun t True = "True"
      | t False = "False"
      | t Zero = "Zero"
      | t (Succ e') = "Succ" ^ par e'
      | t (Pred e') = "Pred" ^ par e'
      | t (IsZero e') = "IsZero" ^ par e'
      | t (If (e1, e2, e3)) = "If" ^ triple (e1, e2, e3)
    and par e = "(" ^ t e ^ ")"
    and triple (e1, e2, e3) = 
      "(" ^ tos e1 ^ "," ^ tos e2 ^ "," ^ tos e3 ^ ")"
    in
      t e
    end 
		  
  fun unparse e = let
    fun u True = "true"
      | u False = "false"
      | u Zero = "0"
      | u (Succ e') = "succ " ^ uparen e'
      | u (Pred e') = "pred " ^ uparen e'
      | u (IsZero e') = "izero " ^ uparen e'
      | u (If (e1, e2, e3)) = 
	  "if " ^ uparen e1 ^ " then " ^ uparen e2 ^ " else " ^ uparen e3
    and uparen True = "true"
      | uparen False = "false"
      | uparen Zero = "0"
      | uparen e = "(" ^ u e ^ ")"
    in
      u e
    end

end
