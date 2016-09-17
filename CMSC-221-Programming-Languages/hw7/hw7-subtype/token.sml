structure Token = struct

  datatype token 
    = Top
    | Ref
    | List
    | TypeName of string (* capitalized *)
    | Label of string    (* lowercase letters *)
    | LPAR
    | RPAR
    | LBRACE
    | RBRACE
    | STAR
    | PLUS
    | ARROW    
    | COMMA
    | COLON

  fun tos t = 
   (case t
     of Top => "Top"
      | Ref => "Ref"
      | List => "List"
      | TypeName s => "TypeName(" ^ s ^ ")"
      | Label s => "Label(" ^ s ^ ")"
      | LPAR => "LPAR"
      | RPAR => "RPAR"
      | STAR => "STAR"
      | PLUS => "PLUS"
      | ARROW => "ARROW"
      | LBRACE => "LBRACE"
      | RBRACE => "RBRACE"
      | COMMA => "COMMA"
      | COLON => "COLON"
   (* end case *))

  fun eq (t:token,t') = (t=t')

end
