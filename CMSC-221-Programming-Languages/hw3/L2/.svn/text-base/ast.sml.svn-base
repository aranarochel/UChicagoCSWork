structure AST = struct
  datatype exp
    = Int of int
    | Var of string
    | True
    | False
    | Let of string * exp * exp
    | Plus of exp * exp
    | LNeg of exp
    | IsZero of exp
    | LessThan of exp * exp
    | If of exp * exp * exp 
  fun tos (Int n) = Int.toString n
    | tos (Var x) = x
    | tos (True) = "true"
    | tos (False) = "false"
    | tos (Let (x, e1, e2)) = 
        "LET(" ^ x ^ "<-" ^ tos(e1) ^ "," ^ tos(e2) ^ ")"
    | tos (Plus (e1, e2)) = "+(" ^ tos(e1) ^ "," ^ tos(e2) ^ ")"
end
