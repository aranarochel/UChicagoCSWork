structure ScopedAST = struct
  type stamp = int
  datatype exp
    = Int of int
    | Var of string * stamp
    | True
    | False
    | Let of string * stamp * exp * exp
    | Plus of exp * exp
    | LNeg of exp
    | IsZero of exp
    | LessThan of exp * exp
    | If of exp * exp * exp
  fun var (x, i) = x ^ ":" ^ Int.toString i
  fun tos (Int n) = Int.toString n
    | tos (Var (x, i)) = var (x, i)
    | tos (True) = "T"
    | tos (False) = "F"
    | tos (Let (x, i, e1, e2)) = 
        "LET(" ^ var(x,i) ^ "<-" ^ tos(e1) ^ "," ^ tos(e2) ^ ")"
    | tos (Plus (e1, e2)) = "+(" ^ tos(e1) ^ "," ^ tos(e2) ^ ")"
    | tos (LNeg e) = "~(" ^ tos(e) ^ ")"
    | tos (IsZero e) = "Z(" ^ tos(e) ^ ")"
    | tos (LessThan (e1,e2)) = "<(" ^ tos(e1) ^ "," ^ tos(e2) ^ ")"
end
