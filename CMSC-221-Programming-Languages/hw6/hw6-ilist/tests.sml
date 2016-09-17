structure Tests = struct

  (* abbreviate painfully long module name *)
  structure I = InstrumentedList

  (* a few orders provided for convenience *)
  fun intLT (m:int,n)       = m<n
  fun realLT (x:real,y)     = x<y
  fun stringLT (s:string,t) = s<t

  (* tests follow... *)
  val test0 = Check.expect (fn (u,u') => true) (fn _ => (), (), "unit")

end

val _ = Check.report ()
