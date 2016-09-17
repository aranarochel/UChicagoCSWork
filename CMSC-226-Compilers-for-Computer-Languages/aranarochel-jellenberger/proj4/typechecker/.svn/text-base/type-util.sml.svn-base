(* type-util.sml
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *
 * Various utility functions on types.
 *)

structure TypeUtil : sig

    type ty = Type.ty

  (* return the string representation of a type that is suitable for error messages *)
    val toString : ty -> string

  (* convert a type to a string representation.  If the first argument is true, then
   * the unique stamps on type variables and type constructors are shown.  Otherwise,
   * just the user-defined names are used.
   *)
    val fmt : bool -> ty -> string

  (* are two types equal (upto renaming) *)
    val same : ty * ty -> bool

  (* copy a type while replacing bound variables with fresh variables *)
    val copy : ty -> ty

  (* compute a substitution on a list of type variables that makes the first type the same as
   * the second.  Specifically
   *
   *	match (tvs, ty1, ty2)  ==>  SOME(tys)
   *
   * means that
   *
   *	same (apply (tvs, ty1, tys), ty2)
   *
   * return NONE if no such substitution exists.
   * NOTE: we assume that none of the tvs occur in ty2.
   *)
    val match : TyVar.t list * ty * ty -> ty list option

  (* apply a type function to a list of type arguments; note that this process will rename
   * bound variables with fresh variables.
   *)
    val apply : TyVar.t list * ty * ty list -> ty

  (* return the range component of a type or value function; raises Fail if the
   * argument is not a type function or value function.
   *)
    val rangeOf : ty -> ty

  end = struct

    structure TV = TyVar
    structure TC = TyCon
    structure Ty = Type

    datatype ty = datatype Type.ty

    fun fmt showStamps ty = let
	  val tv2s = if showStamps
		then TV.toString
		else TV.nameOf
	  val tc2s = if showStamps
		then TC.toString
		else TC.nameOf
	  fun toS (ty, l) = (case ty
		 of PolyTy(tv::tvs, ty) =>
		      "[" :: tv2s tv
			:: List.foldr (fn (tv, l) => "," :: tv2s tv :: l) ("] " :: toS(ty, l)) tvs
		  | PolyTy([], ty) => "[] " :: toS(ty, l) (* should never happen! *)
		  | VarTy tv => tv2s tv :: l
		  | ConTy(tyc, []) => tc2s tyc :: l
		  | ConTy(tyc, ty::tys) =>
		      tc2s tyc :: "["
			:: toS(ty, List.foldr (fn arg => ", " :: toS arg) ("]" :: l) tys)
		  | FunTy(ty1, ty2) => let
		      val l = " -> " :: toS(ty2, l)
		      in
			case ty1
			 of PolyTy _ => paren (ty1, l)
			  | FunTy _ => paren (ty1, l)
			  | _ => toS(ty1, l)
			(* end case *)
		      end
		  | TupleTy (ty::tys) => let
		      fun toS' (ty as PolyTy _, l) = paren (ty, l)
			| toS' (ty as FunTy _, l) = paren (ty, l)
			| toS' (ty as TupleTy _, l) = paren (ty, l)
			| toS' (ty, l) = toS (ty, l)
		      in
			toS' (ty, List.foldr (fn arg => " * " :: toS' arg) l tys)
		      end
		  | TupleTy [] => "()" :: l (* should never happen! *)
		  | ErrorTy => "<error>" :: l
		(* end case *))
	  and paren (ty, l) = "(" :: toS (ty, ")" :: l)
	  in
	    String.concat (toS (ty, []))
	  end

    val toString = fmt false

  (* are two types equal (upto renaming) *)
    fun same (ty1, ty2) = let
	(* check type equality in the context of a substitution sigma that maps
	 * bound variables in the first type to bound variables in the second type.
	 *)
	  fun same' (sigma, ty1, ty2) = (case (ty1, ty2)
		 of (PolyTy(tvs1, ty1), PolyTy(tvs2, ty2)) => let
		      fun chk ([], [], sigma) = same'(sigma, ty1, ty2)
			| chk (tv1::tvs1, tv2::tvs2, sigma) =
			    chk(tvs1, tvs2, TV.Map.insert(sigma, tv1, tv2))
			| chk _ = false (* different numbers of bound tyvars *)
		      in
			chk (tvs1, tvs2, sigma)
		      end
		  | (VarTy tv1, VarTy tv2) => (case TV.Map.find(sigma, tv1)
		       of SOME tv => TyVar.same(tv, tv2)
			| NONE => TyVar.same(tv1, tv2) (* free type variables *)
		      (* end case *))
		  | (ConTy(tyc1, tys1), ConTy(tyc2, tys2)) =>
		      TC.same(tyc1, tyc2) andalso sameTys (sigma, tys1, tys2)
		  | (FunTy(ty11, ty12), FunTy(ty21, ty22)) =>
		      same'(sigma, ty11, ty21) andalso same'(sigma, ty12, ty22)
		  | (TupleTy tys1, TupleTy tys2) => sameTys (sigma, tys1, tys2)
		  | (ErrorTy, _) => true (* error type is equal to any other type! *)
		  | (_, ErrorTy) => true
		  | _ => false
		(* end case *))
	  and sameTys (sigma, [], []) = true
	    | sameTys (sigma, ty1::tys1, ty2::tys2) =
		same'(sigma, ty1, ty2) andalso sameTys(sigma, tys1, tys2)
	    | sameTys _ = false
	  in
	    same' (TV.Map.empty, ty1, ty2)
	  end

  (* given a mapping from type variables to types, make a fresh copy of a type.  This process
   * renames bound variables with fresh ones.
   *)
    fun copyWithSubst (sigma, ty) = let
	  fun copy (sigma, ty) = (case ty
		 of PolyTy(tvs, ty) => let
		      val tvs' = List.map TV.copy tvs
		      val sigma = ListPair.foldl
			    (fn (tv, tv', sigma) => TV.Map.insert(sigma, tv, VarTy tv'))
			      sigma (tvs, tvs')
		      in
			PolyTy(tvs', copy(sigma, ty))
		      end
		  | VarTy tv => (case TV.Map.find (sigma, tv)
		       of SOME ty => ty
			| _ => ty
		      (* end case *))
		  | ConTy(tyc, tys) => ConTy(tyc, copyList (sigma, tys))
		  | FunTy(ty1, ty2) => FunTy(copy (sigma, ty1), copy (sigma, ty2))
		  | (TupleTy tys) => TupleTy(copyList (sigma, tys))
		  | ErrorTy => ErrorTy
		(* end case *))
	  and copyList (sigma, []) = []
	    | copyList (sigma, ty::tys) = copy(sigma, ty) :: copyList (sigma, tys)
	  in
	    copy (sigma, ty)
	  end

  (* copy a type while replacing bound variables with fresh variables *)
    fun copy ty = copyWithSubst (TV.Map.empty, ty)

    fun match ([], ty1, ty2) = if same(ty1, ty2) then SOME[] else NONE
      | match (tvs, ty1, ty2) = let
	(* substitution from tvs to types; we initially map every variable to NONE *)
	  val subst = let
		val tbl = TV.Tbl.mkTable (32, Fail "subst")
		fun ins tv = TV.Tbl.insert tbl (tv, NONE : ty option)
		in
		  List.app ins tvs;
		  tbl
		end
	(* bind a type variable in the substitution *)
	  fun bind (tv, ty) = TV.Tbl.insert subst (tv, SOME ty)
	  fun match' (sigma, ty1, ty2) = (case (ty1, ty2)
		 of (PolyTy(tvs1, ty1), PolyTy(tvs2, ty2)) => let
		      fun chk ([], [], sigma) = match'(sigma, ty1, ty2)
			| chk (tv1::tvs1, tv2::tvs2, sigma) =
			    chk(tvs1, tvs2, TV.Map.insert(sigma, tv1, tv2))
			| chk _ = false (* different numbers of bound tyvars *)
		      in
			chk ([], [], sigma)
		      end
		  | (VarTy tv1, ty2) => (case TV.Tbl.find subst tv1
		       of NONE => ( (* tv1 is not in the tvs, so just check for equality under renaming *)
			    case ty2
			     of VarTy tv2 => (case TV.Map.find(sigma, tv1)
				   of SOME tv => TyVar.same(tv, tv2)
				    | NONE => TyVar.same(tv1, tv2) (* free type variables *)
				  (* end case *))
			      | _ => false
			    (* end case *))
			| SOME NONE => ( (* first occurrence of tv1 *)
			    bind (tv1, ty2); true)
			| SOME(SOME ty1) => same(ty1, ty2)
		      (* end case *))
		  | (ConTy(tyc1, tys1), ConTy(tyc2, tys2)) =>
		      TC.same(tyc1, tyc2) andalso matchTys (sigma, tys1, tys2)
		  | (FunTy(ty11, ty12), FunTy(ty21, ty22)) =>
		      match'(sigma, ty11, ty21) andalso match'(sigma, ty12, ty22)
		  | (TupleTy tys1, TupleTy tys2) => matchTys (sigma, tys1, tys2)
		  | (ErrorTy, _) => true (* error type is equal to any other type! *)
		  | (_, ErrorTy) => true
		  | _ => false
		(* end case *))
	  and matchTys (sigma, [], []) = true
	    | matchTys (sigma, ty1::tys1, ty2::tys2) =
		match'(sigma, ty1, ty2) andalso matchTys(sigma, tys1, tys2)
	    | matchTys _ = false
	  in
	    if match' (TV.Map.empty, ty1, ty2)
	      then let
	      (* the replacement for a type variable; use Unit for unused variables *)
		fun replace tv = Option.getOpt (TV.Tbl.lookup subst tv, Basis.unitTy)
		in
		  SOME(List.map replace tvs)
		end
	      else NONE
	  end

  (* apply a type function to a list of type arguments; note that this process will rename
   * bound variables with fresh variables.
   *)
    fun apply ([], ty, []) = copy ty
      | apply (params, ty, args) = let
	  val sigma = ListPair.foldlEq
		(fn (tv, ty, sigma) => TV.Map.insert(sigma, tv, ty))
		  TV.Map.empty
		    (params, args)
	  in
	    copyWithSubst (sigma, ty)
	  end
	    handle ListPair.UnequalLengths => raise Fail "TypeUtil.apply: arity mismatch"

  (* return the range component of a type or value function *)
    fun rangeOf (PolyTy(_, ty)) = ty
      | rangeOf (FunTy(_, ty)) = ty
      | rangeOf ty = raise Fail(concat["rangeOf(", toString ty, ")"])

  end

