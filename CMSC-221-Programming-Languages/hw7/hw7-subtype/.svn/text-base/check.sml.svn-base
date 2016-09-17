structure Check : CHECK = struct

  val nTests   = ref 0
  val nCorrect = ref 0
  val nExn     = ref 0

  fun ++ r = (r := !r + 1)
  fun z r  = (r := 0)

  fun reset () = List.app z [nTests, nCorrect, nExn]

  datatype 'a result
    = Correct of string
    | Incorrect of 'a * 'a * string
    | Raised of exn * string

  fun expect eq (thunk : unit -> 'a, e, lbl) =
   (let
      val _ = ++nTests
      val r = thunk ()
    in
      if eq (r, e)
      then (++nCorrect; Correct lbl)
      else Incorrect (r, e, lbl)
    end)
    handle ex => (++nExn; Raised (ex, lbl))

  fun boolEq (b:bool, c) = b=c
  fun expectT (thunk, lbl) = expect boolEq (thunk, true, lbl)
  fun expectF (thunk, lbl) = expect boolEq (thunk, false, lbl)

  fun report () = 
    let
      val i = Int.toString o !
      val pr = TextIO.print o String.concat
    in
      pr ["========\n",
	  "test results: ", i nCorrect, "/", i nTests, 
	  " (", i nExn, " exception", if !nExn=1 then "" else "s", ")\n",
	  "========\n"]
    end

end

