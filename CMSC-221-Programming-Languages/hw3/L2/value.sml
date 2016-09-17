structure Value = struct
  datatype value
    = Int of int
    | True
    | False
  fun tos (Int n) = Int.toString n
    | tos True = "T"
    | tos False ="F"
  fun eq (t:value,u) = (t=u)
end
