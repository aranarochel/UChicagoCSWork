structure Ty = struct

  datatype ty
    = Nat
    | Bool

  (* In case anyone is wondering why this datatype isn't called "type",
   * it's because the word "type" is reserved in SML.
   *)

  fun eq (t1:ty, t2) = t1=t2

  val neq = not o eq

  fun tos Nat = "Nat"
    | tos Bool = "Bool"

end
