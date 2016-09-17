structure Token = struct
  datatype token 
    = Let
    | LArrow  
    | LParen
    | RParen
    | Plus
    | Comma
    | ID of string
    | Int of int
  fun tos Let = "Let"
    | tos LArrow = "LArrow"
    | tos LParen = "LParen"
    | tos RParen = "RParen"
    | tos Plus = "Plus"
    | tos Comma = "Comma"
    | tos (ID x) = "ID(" ^ x ^ ")"
    | tos (Int n) = "Int(" ^ Int.toString n ^ ")"
end
