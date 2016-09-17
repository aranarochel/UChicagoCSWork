(* basis.sml
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 *
 * CMSC 22610 Sample code (Winter 2015)
 *
 * Type constructors, data constructors, and variables defined in the Basis.
 *)

structure Basis : sig

  (* predefined type constructors *)
    val boolTyc : TyCon.t
    val intTyc : TyCon.t
    val listTyc : TyCon.t
    val stringTyc : TyCon.t
    val unitTyc : TyCon.t

  (* predefined types *)
    val boolTy : Type.ty
    val intTy : Type.ty
    val listTy : Type.ty -> Type.ty
    val stringTy : Type.ty
    val unitTy : Type.ty

  (* operators *)
    val eql : Var.t
    val lte : Var.t
    val lt : Var.t
    val concat : Var.t
    val add : Var.t
    val subtract : Var.t
    val multiply : Var.t
    val divide : Var.t
    val modulo : Var.t

  (* pre-defined data constructors *)
    val boolTrue : DataCon.t
    val boolFalse : DataCon.t
    val listCons : DataCon.t
    val listNil : DataCon.t
    val unitUnit : DataCon.t

  (* predefined variables *)
    val argc : Var.t
    val arg : Var.t
    val fail : Var.t
    val ignore : Var.t
    val neg : Var.t
    val not : Var.t
    val print : Var.t
    val size : Var.t
    val sub : Var.t

  end = struct

    structure BN = BasisNames

  (* type variable used in the definition of the list tyc and dcons. *)
    val listTyParam = TyVar.new(Atom.atom "a")
    val listTyParam' = Type.VarTy listTyParam

  (* predefined type constructors *)
    val boolTyc		= TyCon.new(BN.bool, [])
    val intTyc		= TyCon.integerTyc
    val listTyc		= TyCon.new(BN.list, [listTyParam])
    val stringTyc	= TyCon.stringTyc
    val unitTyc		= TyCon.new(BN.unit, [])

  (* predefined types *)
    val boolTy		= Type.ConTy(boolTyc, [])
    val intTy		= Type.ConTy(intTyc, [])
    fun listTy ty	= Type.ConTy(listTyc, [ty])
    val stringTy	= Type.ConTy(stringTyc, [])
    val unitTy		= Type.ConTy(unitTyc, [])

  (* operators *)
    local
      fun binOpTy (ty1, ty2, ty3) = Type.FunTy(Type.TupleTy[ty1, ty2], ty3)
    in
    val eql =		Var.prim (BN.eql, binOpTy(intTy, intTy, boolTy))
    val lte =		Var.prim (BN.lte, binOpTy(intTy, intTy, boolTy))
    val lt =		Var.prim (BN.lt, binOpTy(intTy, intTy, boolTy))
    val concat =	Var.prim (BN.concat, binOpTy(stringTy, stringTy, stringTy))
    val add =		Var.prim (BN.add, binOpTy(intTy, intTy, intTy))
    val subtract =	Var.prim (BN.subtract, binOpTy(intTy, intTy, intTy))
    val multiply =	Var.prim (BN.multiply, binOpTy(intTy, intTy, intTy))
    val divide =	Var.prim (BN.divide, binOpTy(intTy, intTy, intTy))
    val modulo =	Var.prim (BN.modulo, binOpTy(intTy, intTy, intTy))
    end (* local *)

  (* pre-defined data constructors *)
    val boolTrue =	DataCon.new (boolTyc, BN.boolTrue, NONE)
    val boolFalse =	DataCon.new (boolTyc, BN.boolFalse, NONE)
    val listCons =	DataCon.new (
			  listTyc,
			  BN.listCons,
			  SOME(Type.TupleTy[listTyParam', listTy listTyParam']))
    val listNil =	DataCon.new (listTyc, BN.listNil, NONE)
    val unitUnit =	DataCon.new (unitTyc, BN.unitUnit, NONE)

  (* finish predefined data-type constructors *)
    val _ = (TyCon.finish boolTyc; TyCon.finish listTyc; TyCon.finish unitTyc)

  (* predefined variables *)
    local
      fun absTy mk = let
	    val tv = TyVar.new(Atom.atom "a")
	    in
	      Type.PolyTy([tv], mk(Type.VarTy tv))
	    end
    in
    val argc =		Var.prim(BN.argc, Type.FunTy(unitTy, intTy))
    val arg =		Var.prim(BN.arg, Type.FunTy(intTy, stringTy))
    val fail =		Var.prim(BN.fail, absTy (fn t => Type.FunTy(stringTy, t)))
    val ignore =	Var.prim(BN.ignore, absTy (fn t => Type.FunTy(t, unitTy)))
    val neg =		Var.prim(BN.neg, Type.FunTy(intTy, intTy))
    val not =		Var.prim(BN.not, Type.FunTy(boolTy, boolTy))
    val print =		Var.prim(BN.print, Type.FunTy(stringTy, unitTy))
    val size =		Var.prim(BN.size, Type.FunTy(stringTy, intTy))
    val sub =		Var.prim(BN.sub, Type.FunTy(Type.TupleTy[stringTy, intTy], intTy))
    end (* local *)

  end
