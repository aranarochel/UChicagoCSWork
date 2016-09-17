structure Tests = struct
(* Test mechanisms courtesy of Prof.Shaw (what a guy!) *)


  fun strEq (s:string, t) = s=t

  val ty : string -> Ty.ty = Labels.check o Parse.parse o Lex.lex

  fun dupLabel (s, b, msg) =
    let
      fun thunk () = (ty s; "passed")
                      handle Labels.DuplicateLabel "failed" => "failed"
    in
      Check.expect strEq (thunk, b, msg)
    end

  infix <:
  fun string1 <: string2 = fn () =>
    let
      val r = Labels.check o Parse.parse o Lex.lex
      val t1 = r string1
      val t2 = r string2
    in
      Subtype.subtype (t1, t2)
    end



  val _ = Check.reset ()

  (* tests here...*)
  val t1 = Check.expectT (fn _ => 1=1, "truth")
  val t2 = Check.expectF (fn _ => 1=2, "falsehood")
  val t3 = Check.expect Ty.eq (fn _ => Ty.Base "N", Ty.Base "N", "N=N")

  val t4 = dupLabel ("{x:N,x:B}", "x", "dup")
  val t5 = dupLabel ("{x:N,y:B}", "passed", "notdup")
  val t6 = dupLabel ("(Ref {x:N,y:N})", "passed", "notdup")
  val t7 = dupLabel ("(List {x:T,y:N,x:N})", "x", "dup")  
  val t8 = dupLabel ("{x:T,y:{x:T,x:B}}","failed","dup")

  local
    fun t j l = Check.expectT (j, l)
    fun f j l = Check.expectF (j, l)
  in
    val tests = [
      t ("Top" <: "Top") "top refl",
      t ("Nat" <: "Top") "base sub top",
      f ("Top" <: "Nat") "top not sub base",
      t ("{x:N}" <: "{x:N}") "record refl",
      t ("{x:N,y:N}" <: "{x:N}") "rec-width",
      t ("{x:{a:N,b:N}}" <: "{x:{a:N}}") "rec-depth",
      t ("{x:N,y:B}" <: "{y:B,x:N}") "rec-perm",
      f ("{x:N,y:B}" <: "{y:N,x:B}") "!rec-perm",
      t ("(Ref Nat)" <: "(Ref Nat)") "S-Ref",
      t ("(List {x:Nat,y:Bool})" <: "(List {x:Nat})") "S-List",
      t ("(* N Y)" <: "(* Top Top)") "S-Prod",
      f ("(+ Top Top)" <: "(+ N N)") "!S-Sum",
      t ("(-> Top {x:K,y:{a:N,b:O}})" <: "(-> N {y:{b:O}})") "S-Arrow"
      (* ... add more tests here ... *)
    ]
  end

  val _ = Check.report ()

end

