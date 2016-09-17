signature CHECK = sig

  datatype 'a result
    = Correct of string
    | Incorrect of 'a * 'a * string
    | Raised of exn * string

  val expect : ('a * 'a -> bool) -> 
	       ((unit -> 'a) * 'a * string) ->
	       'a result

  val expectT : ((unit -> bool) * string) -> bool result
  val expectF : ((unit -> bool) * string) -> bool result

  val reset  : unit -> unit
  val report : unit -> unit

end
