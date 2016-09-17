structure ScopedAST = struct
  type stamp = int
  datatype exp
    = Int of int
    | Var of string * stamp
    | Let of string * stamp * exp * exp
    | Plus of exp * exp
  fun var (x, i) = x ^ ":" ^ Int.toString i
  fun tos (Int n) = Int.toString n
    | tos (Var (x, i)) = var (x, i)
    | tos (Let (x, i, e1, e2)) = 
        "LET(" ^ var(x,i) ^ "<-" ^ tos(e1) ^ "," ^ tos(e2) ^ ")"
    | tos (Plus (e1, e2)) = "+(" ^ tos(e1) ^ "," ^ tos(e2) ^ ")"
end
