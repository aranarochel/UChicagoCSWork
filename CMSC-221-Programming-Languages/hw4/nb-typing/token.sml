structure Token = struct

  datatype token
    = True
    | False
    | If
    | Then
    | Else
    | Zero
    | Succ
    | Pred
    | IsZero
    | LParen
    | RParen

  fun eq (t1:token, t2) = t1=t2

  fun tos True = "True"
    | tos False = "False"
    | tos If = "If"
    | tos Then = "Then"
    | tos Else = "Else"
    | tos Zero = "Zero"
    | tos Succ = "Succ"
    | tos Pred = "Pred"
    | tos IsZero = "IsZero"
    | tos LParen = "LParen"
    | tos RParen = "RParen"

end
