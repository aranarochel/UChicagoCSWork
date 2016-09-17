(* type-checker.sml
 *
 * CMSC22610 Winter 2015
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *)

signature TYPE_CHECKER =
  sig

    val chkProgram : Error.err_stream * ParseTree.program -> AST.program

  end

structure TypeChecker : TYPE_CHECKER =
  struct

    structure PT = ParseTree
    structure B = Basis
    structure DC = DataCon
    structure Ty = Type
    structure TC = TyCon
    structure TV = TyVar
    structure C = Context
    structure TU = TypeUtil
    structure AMap = AtomMap

  (* the type of an AST expression *)
    fun typeOf (AST.Exp(_, ty)) = ty

  (* AST terms for when we encounter an error and there is nothing sensible to return *)
    val errorTy = Ty.ErrorTy
    val errorExp = AST.Exp(AST.TupleExp[], Ty.ErrorTy)
    val errorPat = AST.TuplePat[]
    fun errorVar x = Var.new(x, Ty.ErrorTy)	(* variable with error type *)

  (* helper for building LetExp expressions *)
    fun letExp (bind, e) = AST.Exp(AST.LetExp(bind, e), typeOf e)

  (***** Error reporting *****)
    datatype token
      = NL | S of string | A of Atom.atom
      | V of Var.t | TY of Ty.ty

    fun err (cxt, toks) = let
	  fun tok2str NL = "\n"
	    | tok2str (S s) = s
	    | tok2str (A a) = Atom.toString a
	    | tok2str (V x) = Var.nameOf x
	    | tok2str (TY ty) = TU.fmt true ty
	  in
	    C.error(cxt, List.map tok2str toks)
	  end

  (* extend an environment with fresh type variables *)
    fun newTyVars' (place, cxt, tvEnv, tvs) = let
	  fun extend (tv::tvs, tvEnv, tvs') = let
		val tv' = TV.new tv
		val tvs' = tv' :: tvs'
		in
		  if AMap.inDomain(tvEnv, tv)
		    then (
		      err (cxt, [S "duplicate type variable '", A tv, S "' in ", S place, NL]);
		      extend (tvs, tvEnv, tvs'))
		    else extend (tvs, AMap.insert(tvEnv, tv, tv'), tvs')
		end
	    | extend ([], tvEnv, tvs') = (tvEnv, List.rev tvs')
	  in
	    extend (tvs, tvEnv, [])
	  end

    fun newTyVars (place, cxt, tvs) = newTyVars' (place, cxt, AMap.empty, tvs)

  (* parts of the argument domain that are not yet covered by the match-case rules *)
    datatype missing_coverage
      = DataCons of DC.t list	(* constructors that haven't been covered yet *)
      | NonDataCon		(* for non-data types (e.g., tuples, integers) *)
      | Nothing			(* all values have been covered *)

    fun isExhaustive Nothing = true
      | isExhaustive _ = false

  (* update missing coverage based on a pattern; returns the updated missing coverage paired
   * with true if the pattern was redundant and false otherwise.
   *)
    fun updateCoverage (missing, pat) = let
	  fun dconPat (dc, DataCons cons) = let
		fun find ([], _) = (missing, true) (* data constructor not found ==> redundant pattern *)
		  | find ([dc'], []) = if DC.same(dc, dc')
		      then (Nothing, false)
		      else (missing, true) (* data constructor not found ==> redundant pattern *)
		  | find (dc'::cons, prefix) = if DC.same(dc, dc')
		      then (DataCons(List.revAppend(prefix, cons)), false)
		      else find (cons, dc'::prefix)
		in
		  find (cons, [])
		end
	    | dconPat (dc, Nothing) = (missing, true)
	    | dconPat (dc, _) = raise Fail "impossible: type error"
	(* the pattern is exhaustive *)
	  fun exhaustive Nothing = (missing, true)
	    | exhaustive _ = (Nothing, false)
	  in
	    case pat
	     of AST.ConAppPat(dc, _, _) => dconPat(dc, missing)
	      | AST.ConPat(dc, _) => dconPat(dc, missing)
	      | AST.ListPat _ =>  dconPat(Basis.listCons, missing)
	      | AST.TuplePat _ => exhaustive missing
	      | AST.VarPat _ => exhaustive missing
	    (* end case *)
	  end

  (* check the well-formedness of a type *)
    fun chkTyp (cxt, typ) = (case typ
	   of PT.MarkTyp m => chkTyp (C.mark (cxt, m))
	    | PT.PolyTyp(tvs, typ) => let
		val (tvEnv, tvs) = newTyVars ("type abstraction", cxt, tvs)
		val cxt = C.mergeTVEnv (cxt, tvEnv)
		in
		  Ty.PolyTy(tvs, chkTyp (cxt, typ))
		end
	    | PT.VarTyp tv => (case C.findTyVar (cxt, tv)
		 of SOME tv => Ty.VarTy tv
		  | NONE => (
		      err (cxt, [S "type variable '", A tv, S "' is undefined\n"]);
		      errorTy)
		(* end case *))
	    | PT.ConTyp(tyc, typs) => let
		val tys = chkTyps (cxt, typs)
		val arity = List.length typs
		in
		  case C.findTyCon(cxt, tyc)
		   of SOME(C.TyDef(tvs, ty)) => (* type abbreviation *)
			if (List.length tvs = arity)
			  then TU.apply(tvs, ty, chkTyps (cxt, typs))
			  else (
			    err (cxt, [
				S "arity mismatch in application of '", A tyc,
				S "'; expected ", S(Int.toString(List.length tvs)),
				S " but given ", S(Int.toString arity), NL
			      ]);
			    errorTy)
		    | SOME(C.TyCon tyc') => (* type constructor *)
			if (arity = TC.arityOf tyc')
			  then Ty.ConTy(tyc', tys)
			  else (
			    if (arity < TC.arityOf tyc')
			      then err (cxt, [
				  S "too few arguments to type constructor '", A tyc,
				  S "'; expected ", S(Int.toString(TC.arityOf tyc')),
				  S " but given ", S(Int.toString arity), NL
				])
			      else err (cxt, [
				  S "too many arguments to type constructor '", A tyc,
				  S "'; expected ", S(Int.toString(TC.arityOf tyc')),
				  S " but given ", S(Int.toString arity), NL
				]);
			    errorTy)
		    | NONE => (
			err (cxt, [S "type constructor '", A tyc, S "' is undefined\n"]);
		      (* check argument types too *)
			List.app (fn typ => ignore(chkTyp(cxt, typ))) typs;
			errorTy)
		  (* end case *)
		end
	    | PT.FunTyp(typ1, typ2) =>
		Ty.FunTy(chkTyp (cxt, typ1), chkTyp (cxt, typ2))
	    | PT.TupleTyp typs =>
		Ty.TupleTy(chkTyps (cxt, typs))
	  (* end case *))

  (* check each type in a list of types *)
    and chkTyps (cxt, typs) = List.map (fn typ => chkTyp (cxt, typ)) typs

  (* type check a simple pattern; the arguments are a string describing the place where
   * the pattern appears (for error messages), the context, the type of the pattern,
   * the pattern, and the variable environment being created by the bindings.
   *)
    fun chkSimplePat (place, cxt, ty, pat, vEnv) = let
	  fun chk (cxt, PT.MarkPat m) = chk (C.mark (cxt, m))
	    | chk (cxt, PT.VarPat x) = let
		val x' = Var.new(x, ty)
		in
		  if AMap.inDomain(vEnv, x)
		    then (
		      err (cxt, [S "duplicate variable '", A x, S "' in ", S place, NL]);
		    (* bind x to an AST variable that has ErrorTy as its type to avoid cascading errors *)
		      (AMap.insert(vEnv, x, errorVar x), x'))
		    else (AMap.insert(vEnv, x, x'), x')
		end
	    | chk (cxt, PT.WildPat) = (vEnv, Var.wild ty)
	    | chk (_, p) = (
		PrintParseTree.dump PrintParseTree.pat p;
		raise Fail "impossible: not a simple pattern")
	  in
	    chk (cxt, pat)
	  end

  (* type check a pattern, returning a pair of a value environment and a AST pattern *)
    fun chkPat (cxt, expectedTy, pat) = let
	  fun chk (cxt, pat) = (case pat
		 of PT.MarkPat m => let
		      val (cxt, pat) = C.mark (cxt, m)
		      in
			chkPat (cxt, expectedTy, pat)
		      end
		  | PT.ConPat(dc, optPat) => if Atom.same(dc, BasisNames.listCons)
		      then let
			fun mismatch () = (
			      err (cxt, [
				  S "incompatible type for list '::' pattern\n",
				  S "  argument type: ", TY expectedTy, NL
				]);
			      (AMap.empty, errorPat))
			val SOME(PT.TuplePat[hdPat, tlPat]) = optPat
			in
			  case expectedTy
			   of Ty.ConTy(tyc, [elemTy]) =>
				if TC.same(tyc, Basis.listTyc)
				  then let
				    val (vEnv, hd) = chkSimplePat ("pattern", cxt, elemTy, hdPat, AMap.empty)
				    val (vEnv, tl) = chkSimplePat ("pattern", cxt, expectedTy, tlPat, vEnv)
				    in
				      (vEnv, AST.ListPat(elemTy, hd, tl))
				    end
				  else mismatch ()
			    | _ => mismatch ()
			  (* end case *)
			end
		      else (case C.findDCon(cxt, dc)
			 of SOME dc' => let
			      val (tvs, optArgTy, resTy) = DC.sigOf dc'
			      fun mismatch () = (
				    err (cxt, [
					S "incompatible type for '", A dc, S "' in pattern\n",
					S "  argument type: ", TY expectedTy, NL
				      ]);
				    (AMap.empty, errorPat))
			      in
				case (optArgTy, optPat)
				 of (SOME argTy, SOME p) => (case TU.match (tvs, resTy, expectedTy)
				       of SOME tys => let
					    val argTy = TU.apply(tvs, argTy, tys)
					    val (vEnv, x) = chkSimplePat ("pattern", cxt, argTy, p, AMap.empty)
					    in
					      (vEnv, AST.ConAppPat(dc', tys, x))
					    end
					| NONE => mismatch()
				      (* end case *))
				  | (NONE, NONE) => (case TU.match (tvs, resTy, expectedTy)
				       of SOME tys => (AMap.empty, AST.ConPat(dc', tys))
					| NONE => mismatch()
				      (* end case *))
				  | (SOME _, _) => (
				      err (cxt, [
					  S "expected argument pattern for data constructor '",
					  A dc, S "'\n"
					]);
				      (AMap.empty, errorPat))
				  | (NONE, _) => (
				      err (cxt, [
					  S "unexpected argument pattern for nullary data constructor '",
					  A dc, S "'\n"
					]);
				      (AMap.empty, errorPat))
				(* end case *)
			      end
			  | NONE => (
			      err (cxt, [S "type constructor '", A dc, S "' is undefined\n"]);
			      (AMap.empty, errorPat))
			(* end case *))
		  | PT.TuplePat pats => (case expectedTy
		       of Ty.TupleTy tys => let
			    fun chk ([], [], vEnv, xs) = finish (vEnv, xs)
			      | chk (p::ps, ty::tys, vEnv, xs) = let
				  val (vEnv, x) = chkSimplePat ("tuple pattern", cxt, ty, p, vEnv)
				  in
				    chk (ps, tys, vEnv, x::xs)
				  end
			      | chk ([], tys, vEnv, xs) = (
				  err (cxt, [
				      S (Int.toString(List.length tys)),
				      S " too few patterns in tuple pattern\n"
				    ]);
				  (vEnv, errorPat))
			      | chk (ps, [], vEnv, xs) = (
				  err (cxt, [
				      S (Int.toString(List.length ps)),
				      S " too many patterns in tuple pattern\n"
				    ]);
				  (vEnv, errorPat))
			    and finish (vEnv, xs) = (vEnv, AST.TuplePat(List.rev xs))
			    in
			      chk (pats, tys, AMap.empty, [])
			    end
			| ty => (
			    err (cxt, [
				S "expected pattern with type '", TY ty,
				S "', but found tuple pattern\n"
			      ]);
			    (AMap.empty, errorPat))
		      (* end case *))
		  | pat => let
		      val (vEnv, x) = chkSimplePat ("pattern", cxt, expectedTy, pat, AMap.empty)
		      in
			(vEnv, AST.VarPat x)
		      end
		(* end case *))
	  in
	    chk (cxt, pat)
	  end

  (* type check an expression *)
    fun chkExp (cxt, exp) : AST.exp = (case exp
	   of PT.MarkExp m => chkExp (C.mark (cxt, m))
	    | PT.IfExp(e1, e2, e3) => let
		val e1 = chkExp (cxt, e1)
		val e2 = chkExp (cxt, e2)
		val e3 = chkExp (cxt, e3)
		in
		(* check that e1 has boolean type *)
		  if not (TU.same(typeOf e1, B.boolTy))
		    then err(cxt, [
			S "expected Boolean type for conditional, but found ", TY(typeOf e1), NL
		      ])
		    else ();
		(* check that e2 and e3 have the same type *)
		  if not (TU.same(typeOf e2, typeOf e3))
		    then err (cxt, [
			S "arms of conditional must have the same type\n",
			S "  then branch: ", TY(typeOf e1), NL,
			S "  else branch: ", TY(typeOf e2), NL
		      ])
		    else ();
		  AST.Exp(AST.IfExp(e1, e2, e3), typeOf e2)
		end
	    | PT.BinExp(e1, rator, e2) => if Atom.same(rator, BasisNames.listCons)
		then let (* special case for list cons *)
		  val e1 = chkExp (cxt, e1)
		  val e2 = chkExp (cxt, e2)
		(* listCons is a polymorphic function type *)
		  val listConsTy as Ty.PolyTy(tvs, ty) = DC.typeOf Basis.listCons
		(* instantiate the type and deconstruct *)
		  val instTy as Ty.FunTy(argTy as Ty.TupleTy[hdTy, tlTy], resTy) =
			TU.apply(tvs, ty, [typeOf e1])
		(* specialize "::" to the element type *)
		  val consExp = AST.Exp(
			AST.TyAppExp(
			  AST.Exp(AST.ConExp Basis.listCons, listConsTy),
			  [typeOf e1]),
			instTy)
		  in
		    if not (TU.same(typeOf e1, hdTy))
		      then err (cxt, [
			  S "lhs of list '::' has wrong type\n",
			  S "  expected: ", TY hdTy, NL,
			  S "  found:    ", TY(typeOf e1), NL
			])
		      else ();
		    if not (TU.same(typeOf e2, tlTy))
		      then err (cxt, [
			  S "rhs of list '::' has wrong type\n",
			  S "  expected: ", TY tlTy, NL,
			  S "  found:    ", TY(typeOf e2), NL
			])
		      else ();
		  (* build application expression: rator(e1, e2) *)
		    AST.Exp(
		      AST.AppExp(
		        consExp,
			AST.Exp(AST.TupleExp[e1, e2], argTy)),
		      resTy)
		  end
		else let
		  val e1 = chkExp (cxt, e1)
		  val e2 = chkExp (cxt, e2)
		  val SOME rator' = C.findVar (cxt, rator)
		(* note that all operators are monomorphic functions of two arguments! *)
		  val ratorTy as Ty.FunTy(argTy as Ty.TupleTy[lhsTy, rhsTy], resultTy) =
			Var.typeOf rator'
		  in
		    if not (TU.same(typeOf e1, lhsTy))
		      then err (cxt, [
			  S "lhs of binary operator '", A rator, S "' has wrong type\n",
			  S "  expected: ", TY lhsTy, NL,
			  S "  found:    ", TY(typeOf e1), NL
			])
		      else ();
		    if not (TU.same(typeOf e2, rhsTy))
		      then err (cxt, [
			  S "rhs of binary operator '", A rator, S "' has wrong type\n",
			  S "  expected: ", TY rhsTy, NL,
			  S "  found:    ", TY(typeOf e2), NL
			])
		      else ();
		  (* build application expression: rator(e1, e2) *)
		    AST.Exp(
		      AST.AppExp(
		        AST.Exp(AST.VarExp rator', ratorTy),
			AST.Exp(AST.TupleExp[e1, e2], argTy)),
		      resultTy)
		  end
	    | PT.AppExp(e1, e2) => let
		val e1 = chkExp (cxt, e1)
		val e2 = chkExp (cxt, e2)
		in
		  case typeOf e1
		   of Type.FunTy(domTy, rngTy) =>
			if not (TU.same(typeOf e2, domTy))
			  then (
			    err (cxt, [
				S "argument type mismatch in function application\n",
				S "  expected: ", TY domTy, NL,
				S "  found:    ", TY(typeOf e2), NL
			      ]);
			    errorExp)
			  else AST.Exp(AST.AppExp(e1, e2), rngTy)
		    | Ty.ErrorTy => errorExp  (* avoid cascading errors *)
		    | ty => (
			err (cxt, [
			    S "lhs of application is not a function type\n",
			    S "  found: ", TY ty, NL
			  ]);
			errorExp)
		  (* end case *)
		end
	    | PT.TyAppExp(e, typs) => let
		val e = chkExp (cxt, e)
		val tys = List.map (fn typ => chkTyp(cxt, typ)) typs
		in
		  case typeOf e
		   of Ty.PolyTy(tvs, ty) =>
			if (List.length tvs = List.length tys)
			  then AST.Exp(AST.TyAppExp(e, tys), TU.apply(tvs, ty, tys))
			  else (
			    err (cxt, [
				S "arity mismatch in application of type function '",
				TY(typeOf e), S "\n"
			      ]);
			    errorExp)
		    | Ty.ErrorTy => errorExp  (* avoid cascading errors *)
		    | ty => (
			err (cxt, [
			    S "lhs of type application is not a type function\n",
			    S "  found: ", TY ty, NL
			  ]);
			errorExp)
		  (* end case *)
		end
	    | PT.VarExp x => (case C.findVar (cxt, x)
		 of SOME x' => AST.Exp(AST.VarExp x', Var.typeOf x')
		  | NONE => (
		      err(cxt, [S "variable '", A x, S "' is undefined\n"]);
		      errorExp)
		(* end case *))
	    | PT.ConExp dc => (case C.findDCon (cxt, dc)
		 of SOME dc' => AST.Exp(AST.ConExp dc', DC.typeOf dc')
		  | NONE => (
		      err(cxt, [S "data constructor '", A dc, S "' is undefined\n"]);
		      errorExp)
		(* end case *))
	    | PT.IntExp n => (
		(* VM integers must be in the range -2^30..2^20-1 *)
		if (n < ~1073741824) orelse (1073741823 < n)
		  then err(cxt, [S "integer literal '", S(IntInf.toString n), S "' is too large\n"])
		  else ();
		AST.Exp(AST.IntExp n, B.intTy))
	    | PT.StrExp s => AST.Exp(AST.StrExp s, B.stringTy)
	    | PT.TupleExp es => let
		val es = List.map (fn e => chkExp(cxt, e)) es
		in
		  AST.Exp(AST.TupleExp es, Ty.TupleTy(List.map typeOf es))
		end
	    | PT.CaseExp(e, rules) => chkCase (cxt, e, rules)
	    | PT.LetExp scope => chkScope (cxt, scope)
	  (* end case *))

    and chkScope (cxt, PT.MarkScope m) : AST.exp = chkScope (C.mark (cxt, m))
      | chkScope (cxt, PT.Scope(binds, e)) = let
	  fun chk (cxt, []) = chkExp(cxt, e)
	    | chk (cxt, bind::binds) = let
		val (vEnv, bind) = chkValBind (cxt, bind)
		val e = chk (C.mergeVEnv (cxt, vEnv), binds)
		in
		  letExp (bind, e)
		end
	  in
	    chk (cxt, binds)
	  end

    and chkValBind (cxt, bind) = (case bind
	   of PT.MarkBind m => chkValBind (C.mark (cxt, m))
	    | PT.FunBind(f, params, retTyp, body) => let
		fun chkSig ([], tvEnv, vEnv, params) = let
		      val cxt = C.mergeVEnv (C.mergeTVEnv (cxt, tvEnv), vEnv)
		      val ty = chkTyp (cxt, retTyp)
		      fun mkFunSig (AST.TyParam tvs, ty) = Ty.PolyTy(tvs, ty)
			| mkFunSig (AST.ValParam x, ty) = Ty.FunTy(Var.typeOf x, ty)
		      in
			(cxt, List.rev params, List.foldl mkFunSig ty params, ty)
		      end
		  | chkSig (param::rest, tvEnv, vEnv, params) = let
		      fun chkParam (cxt, PT.MarkParam m) = chkParam (C.mark (cxt, m))
			| chkParam (cxt, PT.TyParam tvs) = let
			    val (tvEnv, tvs) = newTyVars' ("function parameter", cxt, tvEnv, tvs)
			    in
			      chkSig (rest, tvEnv, vEnv, AST.TyParam tvs :: params)
			    end
			| chkParam (cxt, PT.ValParam(x, typ)) = let
			    val ty = chkTyp (C.mergeTVEnv (cxt, tvEnv), typ)
			    val x' = Var.new (x, ty)
			    val vEnv = if AMap.inDomain(vEnv, x) orelse Atom.same(x, f)
				then (
				  err (cxt, [
				      S "duplicate parameter '", A x,
				      S "' for function '", A f, S "'\n"
				    ]);
				  AMap.insert(vEnv, x, errorVar x))
				else AMap.insert(vEnv, x, x')
			    in
			      chkSig (rest, tvEnv, vEnv, AST.ValParam x' :: params)
			    end
		      in
			chkParam (cxt, param)
		      end
		val (cxt', params, fnSig, retTy) = chkSig (params, AMap.empty, AMap.empty, [])
		val f' = Var.new (f, fnSig)
		val vEnv = AMap.singleton(f, f')
		val body = chkExp (C.mergeVEnv(cxt', vEnv), body)
		in
		  if List.all (fn (AST.TyParam _) => true | (AST.ValParam _) => false) params
		    then err (cxt, [
			S "function '", A f, S "' does not have any value parameters\n"
		      ])
		    else ();
		  if not (TU.same (typeOf body, retTy))
		    then err (cxt, [
			S "type mismatch in return type of function '", A f, S "'\n",
			S "  expected: ", TY retTy, NL,
			S "  found:    ", TY(typeOf body), NL
		      ])
		    else ();
		  (vEnv, AST.FunBind(f', params, body))
		end
	    | PT.ValBind(lhs, optTyp, rhs) => let
		val rhs = chkExp (cxt, rhs)
	      (* get the expected type for the lhs pattern *)
		val ty = (case optTyp
		       of NONE => typeOf rhs
			| SOME typ => let
			    val ty' = chkTyp(cxt, typ)
			    in
			      if not (TU.same(typeOf rhs, ty'))
				then err (cxt, [
				    S "type mismatch in rhs of let binding\n",
				    S "  expected: ", TY ty', NL,
				    S "  found:    ", TY(typeOf rhs), NL
				  ])
				else ();
			      ty'
			    end
		      (* end case *))
		val (vEnv, x) = chkSimplePat ("let binding", cxt, ty, lhs, AMap.empty)
		in
		  (vEnv, AST.ValBind(x, rhs))
		end
	    | PT.ExpBind e => let
	      (* an expression, which is shorthand for "let _ : Unit = e" *)
		val e = chkExp(cxt, e)
		in
		  if not (TU.same (typeOf e, B.unitTy))
		    then err (cxt, [
			S "expected Unit type for expression, but found ", TY(typeOf e), NL
		      ])
		    else ();
		  (AMap.empty, AST.ValBind(Var.wild B.unitTy, e))
		end
	  (* end case *))

  (* check a case expression *)
    and chkCase (cxt, e, rules) = let
	  val e = chkExp(cxt, e)
	  val objTy = typeOf e
	(* check a rule in a match case; objTy is the type of the value being matched
	 * and retTy is the return type of the match case.  We track coverage with
	 * two paraameters: missing, which is the missing coverage, and redundant,
	 * which is a flag that is set to true once a redundant pattern has been
	 * detected (and reported).
	 *)
	  fun chkRule (cxt, PT.MarkCaseMatch m, retTy, missing, redundant) = let
		val (cxt, rule) = C.mark (cxt, m)
		in
		  chkRule (cxt, rule, retTy, missing, redundant)
		end
	    | chkRule (cxt, PT.CaseMatch(p, scope), retTy, missing, redundant) = let
		val (vEnv, p) = chkPat (cxt, objTy, p)
		val cxt = C.mergeVEnv (cxt, vEnv)
		val e = chkScope (cxt, scope)
		val (missing, redundant) = if not (TU.same (typeOf e, retTy))
		      then (
			err (cxt, [
			    S "result type of match-case rule does not match previous rules\n",
			    S "  expected: ", TY retTy, NL,
			    S "  found:    ", TY(typeOf e), NL
			  ]);
			(missing, redundant))
		      else (case updateCoverage (missing, p)
			 of (missing, false) => (missing, redundant)
			  | (missing', true) => if isExhaustive missing andalso redundant
			      then (missing', true) (* avoid duplicate error messages *)
			      else (
				err (cxt, [S "redundant pattern in match case\n"]);
				(missing', true))
			(* end case *))
		in
		  if not (TU.same (typeOf e, retTy))
		    then err (cxt, [
			S "result type of match-case rule does not match previous rules\n",
			S "  expected: ", TY retTy, NL,
			S "  found:    ", TY(typeOf e), NL
		      ])
		    else ();
		  (AST.Rule(p, e), typeOf e, missing, redundant)
		end
	(* check the rules *)
	  fun chkRules (retTy, [], missing, _, rules') = (
		if not(isExhaustive missing)
		  then err (cxt, [S "non-exhaustive case match\n"])
		  else ();
		AST.Exp(AST.CaseExp(e, List.rev rules'), retTy))
	    | chkRules (retTy, rule::rules, missing, redundant, rules') = let
		val (rule', retTy', missing, redundant) = chkRule (cxt, rule, retTy, missing, redundant)
	      (* make the first non-error return type the expected return type *)
		val retTy = (case retTy of Ty.ErrorTy => retTy' | _ => retTy)
		in
		  chkRules (retTy, rules, missing, redundant, rule'::rules')
		end
	(* determine the required coverage based on the argument type *)
	  val (missing, redundant) = (case objTy
		 of Ty.ConTy(Ty.DataTyc{cons, ...}, _) => (DataCons(!cons), false)
		  | Ty.ErrorTy => (Nothing, true)  (* we don't report redundant patterns when argument is an error *)
		  | _ => (NonDataCon, false)
		(* end case *))
	  in
	    chkRules (Ty.ErrorTy, rules, missing, redundant, [])
	  end

  (* type check a list of data constructor definitions *)
    fun chkConList (cxt, tyc, conDefs) = let
	  fun chk (cxt, PT.MarkCon m, dcEnv) = let
		val (cxt, conDef) = C.mark(cxt, m)
		in
		  chk (cxt, conDef, dcEnv)
		end
	    | chk (cxt, PT.Con(dc, optTyp), dcEnv) = let
		val optTy = Option.map (fn typ => chkTyp (cxt, typ)) optTyp
		val dc' = DC.new (tyc, dc, optTy)
		in
		  if AMap.inDomain(dcEnv, dc)
		    then err (cxt, [S "duplicate constructor '", A dc, S "'\n"])
		    else ();
		  AMap.insert (dcEnv, dc, dc')
		end
	  in
	    List.foldl (fn (conDef, dcEnv) => chk (cxt, conDef, dcEnv)) AMap.empty conDefs
	  end

    fun chkProg (cxt, PT.MarkProgram m) = chkProg (C.mark (cxt, m))
      | chkProg (cxt, PT.Program(defs, body)) = let
	  fun chkDefs (cxt, []) = chkExp (cxt, body)
	    | chkDefs (cxt0, def::defs) = let
		fun chkDef (cxt, PT.MarkDef m) = chkDef (C.mark (cxt, m))
		  | chkDef (cxt, PT.TypeDef(tyc, tvs, typ)) = let
		      val (tvEnv, tvs) = newTyVars ("type definition", cxt, tvs)
		      val ty = chkTyp (C.mergeTVEnv (cxt, tvEnv), typ)
		      in
			chkDefs (
			  C.bindTyCon (cxt, tyc, C.TyDef(tvs, ty)),
			  defs)
		      end
		  | chkDef (cxt, PT.DataDef(tyc, tvs, dcons)) = let
		      val (tvEnv, tvs) = newTyVars ("data-type definition", cxt, tvs)
		      val tyc' = TC.new (tyc, tvs)
		      val cxt = C.bindTyCon (cxt, tyc, C.TyCon tyc')
		      val dcEnv = chkConList (C.mergeTVEnv (cxt, tvEnv), tyc', dcons)
		      in
			letExp (AST.DataBind tyc',
			  chkDefs (C.mergeDCEnv (cxt, dcEnv), defs))
		      end
		  | chkDef (cxt, PT.ValDef bind) = let
		      val (vEnv, bind) = chkValBind (cxt, bind)
		      in
			letExp (bind, chkDefs(C.mergeVEnv(cxt0, vEnv), defs))
		      end
		in
		  chkDef (cxt0, def)
		end
	  in
	    chkDefs (cxt, defs)
	  end

    fun chkProgram (errStrm, prog) = AST.Program (chkProg (C.new errStrm, prog))

  end
